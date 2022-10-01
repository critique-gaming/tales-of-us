#include "ObstacleActor.h"
#include "Kismet/GameplayStatics.h"

void AObstacleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AObstacleActor::Squish()
{
	if (SquishSFX != nullptr) {
		UGameplayStatics::PlaySound2D(this, SquishSFX);
	}
	UE_LOG(LogTemp, Display, TEXT("Squish"));
}
