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
	class UNiagaraSystem* SquishVFX;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* VFXComponent;

	UPROPERTY(EditAnywhere)
	class USceneComponent* DummyRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldSquish = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWasSquished = false;

	void BeginPlay() override;
	void Squish();

	void Reset() override;

private:
	AObstacleActor();
};
