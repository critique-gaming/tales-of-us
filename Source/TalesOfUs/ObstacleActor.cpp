#include "ObstacleActor.h"
#include "Kismet/GameplayStatics.h"
#include "RythmGameState.h"

void AObstacleActor::BeginPlay()
{
	Super::BeginPlay();

	if (ARythmGameState* GameState = Cast<ARythmGameState>(GetWorld()->GetGameState())) {
		GameState->Obstacles.Add(this);
	}
}

void AObstacleActor::Squish()
{
	if (bWasSquished) return;
	bWasSquished = true;

	if (SquishSFX != nullptr) {
		UGameplayStatics::PlaySound2D(this, SquishSFX);
	}
	UE_LOG(LogTemp, Display, TEXT("Squish"));
}

void AObstacleActor::Reset()
{
	Super::Reset();
	bWasSquished = false;
}
