#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "RythmController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ARythmController : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Legs;

	void BeginPlay() override;

private:
	UFUNCTION()
	void HandleJump();
};
