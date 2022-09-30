#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/Pawn.h"

#include "RythmPawn.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ARythmPawn : public APawn
{
	GENERATED_BODY()

public:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void HandleJump();
};
