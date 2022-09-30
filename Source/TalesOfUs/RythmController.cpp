#include "RythmController.h"
#include "RythmGameState.h"

#include "TweenerSubsystem.h"
#include "Tween.h"
#include "Kismet/GameplayStatics.h"

ARythmController::ARythmController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARythmController::BeginPlay()
{
	Super::BeginPlay();

	if (ARythmGameState* GameState = Cast<ARythmGameState>(GetWorld()->GetGameState())) {
		GameState->OnJump.AddDynamic(this, &ARythmController::HandleJump);
	}

	for (AActor* Actor : Legs) {
		FLegState LegState;
		LegState.CurrentPosition = Actor->GetActorLocation();
		LegState.LastPosition = LegState.CurrentPosition;
		LegState.Actor = Actor;
		LegState.Index = LegStates.Num();
		LegStates.Add(LegState);
	}

	CameraTarget = Camera->GetActorLocation();
	CameraOffset = CameraTarget - LegStates[0].Actor->GetActorLocation();

	BeatInterval = 60.0 / BeatsPerMinute;
	TimeToBeat = BeatInterval;
}

void ARythmController::CancelAnimation(bool bComplete)
{
	for (FLegState& LegState : LegStates) {
		if (LegState.Tween != nullptr && LegState.Tween->IsActive()) {
			LegState.Tween->Stop(bComplete);
			LegState.CurrentPosition = bComplete ? LegState.CurrentPosition : LegState.LastPosition;
			LegState.LastPosition = LegState.CurrentPosition;
			if (LegState.Actor != nullptr) {
				LegState.Actor->SetActorLocation(LegState.CurrentPosition);
			}
		}
		LegState.Tween = nullptr;
	}
}

void ARythmController::ApplyLegAnimation(FLegState& LegState)
{
	UTweenerSubsystem* TweenerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UTweenerSubsystem>();
	LegState.Tween = UTween::ActorLocationTo(LegState.Actor, LegState.CurrentPosition, false, BeatInterval * 0.5, EEaseType::QuarticEaseOut, ELoopType::None, 0, 0.0f, GetWorld());
	LegState.Tween->CompleteNonDynamic.BindWeakLambda(this, [this, &LegState]() {
		LegState.LastPosition = LegState.CurrentPosition;
	});
	TweenerSubsystem->StartTween(LegState.Tween);

	FVector FirstLegPosition(INFINITY, 0.0f, 0.0f);
	for (FLegState& ItLegState : LegStates) {
		FVector LegPosition = ItLegState.CurrentPosition;
		if (ItLegState.Index == LiftedLegIndex) {
			LegPosition.Z -= 100.0f;
		}
		if (LegPosition.X < FirstLegPosition.X) {
			FirstLegPosition = LegPosition;
		}
	}

	CameraTarget = FirstLegPosition + CameraOffset;
}

void ARythmController::LiftUpLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	LiftedLegIndex = LegIndex;

	FLegState& LegState = LegStates[LegIndex];
	LegState.CurrentPosition.Z += 100.0f;
	ApplyLegAnimation(LegState);

	UGameplayStatics::PlaySound2D(this, LiftSFX);
}

void ARythmController::DropLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	LiftedLegIndex = -1;

	if (false) { // If we squish something
		// Call Squish on that something
	} else {
		UGameplayStatics::PlaySound2D(this, DropSFX);
	}

	FLegState& LegState = LegStates[LegIndex];
	LegState.CurrentPosition.Z -= 100.0f;
	ApplyLegAnimation(LegState);

	UGameplayStatics::PlaySound2D(this, DropSFX);
}

void ARythmController::AdvanceLeg(int32 LegIndex, float Duration)
{
	CancelAnimation(true);

	FLegState& LegState = LegStates[LegIndex];
	LegState.CurrentPosition.X += 100.0f;
	ApplyLegAnimation(LegState);

	UGameplayStatics::PlaySound2D(this, AdvanceSFX, 1.0f, 1.0f, Duration - BeatInterval);
}

void ARythmController::PerformIdleAction(float Duration)
{
	if (LiftedLegIndex >= 0) {
		AdvanceLeg(LiftedLegIndex, Duration);
	} else {
		CancelAnimation(true);
		UGameplayStatics::PlaySound2D(this, IdleBeatSFX, 1.0f, 1.0f, Duration - BeatInterval);
	}
}


void ARythmController::PerformAction(float Duration)
{
	if (LiftedLegIndex >= 0) {
		DropLeg(LiftedLegIndex, Duration);
	} else {
		LiftUpLeg(NextLegToLift, Duration);

		NextLegToLift += 1;
		if (NextLegToLift >= Legs.Num()) {
			NextLegToLift = 0;
		}
	}
}

void ARythmController::Stumble()
{
	UE_LOG(LogTemp, Display, TEXT("Stumble"));
	UGameplayStatics::PlaySound2D(this, StumbleSFX);
}

void ARythmController::HandleJump()
{
	if ((TimeToBeat / BeatInterval) <= UndershootTolerance) {
		bPerformedActionThisBeat = true;
		if (TimeToBeat <= QuantizeTolerance) {
			bQueuedAction = true;
		} else {
			PerformAction(TimeToBeat + BeatInterval);
		}
	} else if (((BeatInterval - TimeToBeat) / BeatInterval) <= OvershootTolerance) {
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

	TimeToBeat -= DeltaTime;
	float Offshoot = (BeatInterval - TimeToBeat) / BeatInterval;

	while (TimeToBeat <= 0.0f) {
		BeatInterval = 60.0 / BeatsPerMinute;
		TimeToBeat += BeatInterval;

		if (bQueuedAction) {
			bQueuedAction = false;
			PerformAction(TimeToBeat);
		}
		if (!bPerformedActionThisBeat) {
			PerformIdleAction(TimeToBeat);
		}
		bPerformedActionThisBeat = false;
	}

	Camera->SetActorLocation(LowPassFilter(Camera->GetActorLocation(), CameraTarget, 1.0f, DeltaTime));
}
