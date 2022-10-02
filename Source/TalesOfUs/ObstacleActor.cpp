#include "ObstacleActor.h"
#include "Kismet/GameplayStatics.h"
#include "RythmGameState.h"
#include "TweenerSubsystem.h"
#include "Tween.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"

AObstacleActor::AObstacleActor()
{
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRoot"));
	RootComponent = DummyRoot;

	VFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFXComponent"));
	VFXComponent->SetupAttachment(DummyRoot);
}

void AObstacleActor::BeginPlay()
{
	Super::BeginPlay();

	if (ARythmGameState* GameState = Cast<ARythmGameState>(GetWorld()->GetGameState())) {
		GameState->Obstacles.Add(this);
	}

	VFXComponent->SetAsset(SquishVFX);
}

void AObstacleActor::Squish()
{
	if (bWasSquished) return;
	bWasSquished = true;

	if (SquishSFX != nullptr) {
		UGameplayStatics::PlaySound2D(this, SquishSFX);
	}

	if (SquishVFX != nullptr) {
		VFXComponent->Activate();
	}

	FVector NewScale = GetActorScale3D();
	NewScale.Z = 0.0f;
	UTweenerSubsystem* TweenSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UTweenerSubsystem>();
	UTween* NewTween = UTween::ActorScaleTo(this, NewScale, false,  0.25, EEaseType::QuarticEaseOut, ELoopType::None, 0, 0.0f, GetWorld());
	TweenSubsystem->StartTween(NewTween);

	UE_LOG(LogTemp, Display, TEXT("Squish"));
}

void AObstacleActor::Reset()
{
	Super::Reset();
	bWasSquished = false;
}
