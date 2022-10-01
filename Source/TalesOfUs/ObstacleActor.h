#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "ObstacleActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AObstacleActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* SquishSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldSquish = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWasSquished = false;

	void BeginPlay() override;
	void Squish();
};
