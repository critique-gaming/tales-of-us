#include "RythmController.h"
#include "RythmGameState.h"
#include "LevelInfo.h"
#include "ObstacleActor.h"

#include "TweenerSubsystem.h"
#include "Tween.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

#define GroundTrace ECollisionChannel::ECC_GameTraceChannel1
#define ObstacleTrace ECollisionChannel::ECC_GameTraceChannel2

ARythmController::ARythmController()
{
	PrimaryActorTick.bCanEverTick = true;
}

static const FVector VeryUp(0.0f, 0.0f, 9999.0f);
static const FVector VeryDown(0.0f, 0.0f, -9999.0f);

void ARythmController::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ARythmGameState>(GetWorld()->GetGameState());
	GameState->RythmController = this;

	if (GameState != nullptr) {
		GameState->OnJump.AddDynamic(this, &ARythmController::HandleJump);
		GameState->OnLevelChange.AddDynamic(this, &ARythmController::HandleLevelChange);
		GameState->OnLevelEnd.AddUObject(this, &ARythmController::HandleLevelEnd);
	}

	for (AActor* Actor : Legs) {
		FLegState LegState;
		LegState.CurrentPosition = Actor->GetActorLocation();
		LegState.LastPosition = LegState.CurrentPosition;
		LegState.InitialPosition = LegState.CurrentPosition;
		LegState.Actor = Actor;
		LegState.Index = LegStates.Num();
		LegStates.Add(LegState);
	}

	CameraTarget = Camera->GetActorLocation();
	CameraOffset = CameraTarget - LegStates[0].Actor->GetActorLocation();

	if (TransitionMusic != nullptr) {
		MusicComponent = UGameplayStatics::SpawnSound2D(this, TransitionMusic);
	}
}

void ARythmController::CancelAnimation(bool bComplete)
{
	for (FLegState& LegState : LegStates) {
		if (LegState.Tween != nullptr && LegState.Tween->IsActive()) {
			LegState.Tween->Stop(bComplete);
			LegState.CurrentPosition = bComplete ? LegState.CurrentPosition : LegState.LastPosition;
			LegState.LastPosition = LegState.CurrentPosition;
			if (LegState.Actor != nullptr) {
				LegState.Actor->SetActorLocation(LegState.bIsLifted ? LegState.CurrentPosition + LiftedOffset : LegState.CurrentPosition);
			}
		}
		LegState.Tween = nullptr;
	}
}

void ARythmController::UpdateCamera()
{
	FVector FirstLegPosition(INFINITY, 0.0f, 0.0f);
	float AverageZ = 0.0f;
	for (FLegState& ItLegState : LegStates) {
		FVector LegPosition = ItLegState.CurrentPosition;
		AverageZ += LegPosition.Z;
		if (LegPosition.X < FirstLegPosition.X) {
			FirstLegPosition = LegPosition;
		}
	}
	FirstLegPosition.Z = AverageZ / LegStates.Num();

	CameraTarget = FirstLegPosition + CameraOffset;
}

void ARythmController::ApplyLegAnimation(FLegState& LegState)
{
	FVector Position = LegState.bIsLifted ? LegState.CurrentPosition + LiftedOffset : LegState.CurrentPosition;

	UTweenerSubsystem* TweenerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UTweenerSubsystem>();
	LegState.Tween = UTween::ActorLocationTo(LegState.Actor, Position, false, BeatInterval * 0.5, EEaseType::QuarticEaseOut, ELoopType::None, 0, 0.0f, GetWorld());
	LegState.Tween->CompleteNonDynamic.BindWeakLambda(this, [this, &LegState]() {
		LegState.LastPosition = LegState.CurrentPosition;
	});
	TweenerSubsystem->StartTween(LegState.Tween);

	UpdateCamera();
}

void ARythmController::LiftUpLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	LiftedLegIndex = LegIndex;

	FLegState& LegState = LegStates[LegIndex];

	if (!LegState.bHasRaycastOffset) {
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, LegState.CurrentPosition + VeryUp, LegState.CurrentPosition + VeryDown, GroundTrace);
		if (HitResult.IsValidBlockingHit()) {
			LegState.RaycastOffset = HitResult.Location - LegState.CurrentPosition;
		} else {
			LegState.RaycastOffset = FVector();
		}
	}

	LegState.bIsLifted = true;
	LegState.LiftPosition = LegState.CurrentPosition;
	ApplyLegAnimation(LegState);

	UGameplayStatics::PlaySound2D(this, LiftSFX);
}

bool ARythmController::IsLegOverlapping(const FLegState& LegState)
{
	bool bIsOverlapping = false;
	for (FLegState& ItLegState : LegStates) {
		if (&ItLegState != &LegState && FMath::Abs(ItLegState.CurrentPosition.X - LegState.CurrentPosition.X) <= 10.0f) {
			bIsOverlapping = true;
		}
	}
	return bIsOverlapping;
}

void ARythmController::DropLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	FLegState& LegState = LegStates[LegIndex];

	if (IsLegOverlapping(LegState)) {
		Stumble();
	} else {
		LiftedLegIndex = -1;
		LegState.bIsLifted = false;
		ApplyLegAnimation(LegState);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, LegState.CurrentPosition + VeryUp, LegState.CurrentPosition + VeryDown, ObstacleTrace);
		if (AObstacleActor* HitActor = Cast<AObstacleActor>(HitResult.GetActor())) {
			HitActor->Squish();
		} else {
			if (DropSFX != nullptr) {
				UGameplayStatics::PlaySound2D(this, DropSFX);
			}
		}
	}

	if (GameState != nullptr && GameState->LevelEndActor != nullptr) {
		bool bHasFinished = true;
		for (FLegState& ItLegState : LegStates) {
			if (ItLegState.CurrentPosition.X < GameState->LevelEndActor->GetActorLocation().X) {
				bHasFinished = false;
			}
		}
		if (bHasFinished) {
			GameState->EndLevel();
		}
	}
}

void ARythmController::AdvanceLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	FLegState& LegState = LegStates[LegIndex];
	do {
		LegState.CurrentPosition.X += 100.0f;
	} while (IsLegOverlapping(LegState));

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, LegState.CurrentPosition + VeryUp, LegState.CurrentPosition + VeryDown, GroundTrace);
	if (HitResult.IsValidBlockingHit()) {
		LegState.CurrentPosition = HitResult.Location - LegState.RaycastOffset;
	}

	ApplyLegAnimation(LegState);

	if (AdvanceSFX != nullptr) {
		UGameplayStatics::PlaySound2D(this, AdvanceSFX, 1.0f, 1.0f, Duration - BeatInterval);
	}
}

void ARythmController::PerformIdleAction(float Duration)
{
	if (BeatIndex == 0) {
		if (LiftedLegIndex == -1) {
			LiftUpLastLeg(Duration);
		}
	} else if (BeatIndex == TimeSignature - 1) {
		if (LiftedLegIndex >= 0) {
			DropLeg(LiftedLegIndex, Duration);
		}
	} else {
		if (LiftedLegIndex >= 0) {
			AdvanceLeg(LiftedLegIndex, Duration);
		}
	}
}

void ARythmController::LiftUpLastLeg(float Duration)
{
	int32 LegToLift = 0;
	float MinX = INFINITY;
	for (int32 Index = 0; Index < Legs.Num(); Index += 1) {
		float X = LegStates[Index].CurrentPosition.X;
		if (X < MinX) {
			MinX = X;
			LegToLift = Index;
		}
	}

	LiftUpLeg(LegToLift, Duration);
}


void ARythmController::PerformAction(float Duration)
{
	if (LiftedLegIndex >= 0) {
		bPerformedActionThisBeat = true;
		DropLeg(LiftedLegIndex, Duration);
	}
}

void ARythmController::Stumble()
{
	UE_LOG(LogTemp, Display, TEXT("Stumble"));
	if (StumbleSFX != nullptr) {
		UGameplayStatics::PlaySound2D(this, StumbleSFX);
	}

	if (LiftedLegIndex >= 0) {
		FLegState& LegState = LegStates[LiftedLegIndex];
		LegState.CurrentPosition = LegState.LastPosition = LegState.LiftPosition;
		ApplyLegAnimation(LegState);
	}
}

void ARythmController::HandleJump()
{
	if (!bIsEnabled) return;
	if (TimeToBeat <= FMath::Min(BeatInterval * 0.5, FMath::Max(UndershootTolerance * BeatInterval, MinUndershootTolerance))) {
		if (TimeToBeat <= QuantizeTolerance) {
			bQueuedAction = true;
		} else {
			PerformAction(TimeToBeat + BeatInterval);
		}
	} else if (BeatInterval - TimeToBeat <= FMath::Min(BeatInterval * 0.5, FMath::Max(OvershootTolerance * BeatInterval, MinOvershootTolerance))) {
		CancelAnimation(false);
		PerformAction(BeatInterval);
	} else {
		Stumble();
	}
}

template <typename T>
T LowPassFilter(T OldValue, T NewValue, float RC, float DeltaTime) {
	float Alpha = DeltaTime / (DeltaTime + RC);
	return OldValue + Alpha * (NewValue - OldValue);
}

void ARythmController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEnabled) {
		TimeToBeat -= DeltaTime;
		float Offshoot = (BeatInterval - TimeToBeat) / BeatInterval;

		while (TimeToBeat <= 0.0f) {
			if (QueuedLevelToStart) {
				StartLevel(*QueuedLevelToStart);
				QueuedLevelToStart = nullptr;
				break;
			}

			if (GameState != nullptr) {
				GameState->OnBeat.Broadcast();
			}

			TimeToBeat += BeatInterval;
			BeatIndex = (BeatIndex + 1) % TimeSignature;

			if (bQueuedAction) {
				bQueuedAction = false;
				PerformAction(TimeToBeat);
			}
			if (!bPerformedActionThisBeat) {
				PerformIdleAction(TimeToBeat);
			}
			bPerformedActionThisBeat = false;
		}
	}

	Camera->SetActorLocation(LowPassFilter(Camera->GetActorLocation(), CameraTarget, 1.0f, DeltaTime));
}

void ARythmController::StartLevel(const FLevelInfo& LevelInfo)
{
	BeatInterval = 60.0f / LevelInfo.BeatsPerMinute;
	BeatIndex = -1;
	bIsEnabled = true;

	// 1s for buffering
	TimeToBeat += FMath::Min(BeatInterval, LevelInfo.BeatStartOffset + 1.0f);

	if (MusicComponent) {
		MusicComponent->FadeOut(TimeToBeat, 0.0f);
		MusicComponent = nullptr;
	}
	if (LevelInfo.Music != nullptr) {
		MusicComponent = UGameplayStatics::SpawnSound2D(this, LevelInfo.Music, 1.0f, 1.0f, TimeToBeat - LevelInfo.BeatStartOffset);
	}
}

void ARythmController::QueueStartLevel(const struct FLevelInfo& LevelInfo)
{
	if (bIsEnabled) {
		QueuedLevelToStart = &LevelInfo;
	} else {
		StartLevel(LevelInfo);
	}
}

void ARythmController::HandleLevelChange()
{
	check(GameState != nullptr);

	CancelAnimation(true);

	LiftedLegIndex = -1;
	for (FLegState& LegState : LegStates) {
		LegState.Actor->SetActorLocation(LegState.InitialPosition);
		LegState.CurrentPosition = LegState.LastPosition = LegState.InitialPosition;
		LegState.bIsLifted = false;
		LegState.bHasRaycastOffset = false;
	}

	UpdateCamera();
	Camera->SetActorLocation(CameraTarget);

	QueueStartLevel(*GameState->SelectedLevel);
}

void ARythmController::StopLevel()
{
	bIsEnabled = false;
	TimeToBeat = 0.0f;
	if (MusicComponent) {
		MusicComponent->FadeOut(BeatInterval, 0.0f);
		MusicComponent = nullptr;
	}

	if (TransitionMusic != nullptr) {
		MusicComponent = UGameplayStatics::SpawnSound2D(this, TransitionMusic);
	}
}

void ARythmController::HandleLevelEnd(struct FLevelResult* LevelResult)
{
	StopLevel();
}
