#pragma once

#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/GameStateBase.h"

#include "RythmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRythmSimpleDelegate);


UCLASS()
class ARythmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FRythmSimpleDelegate OnJump;

protected:
	void BeginPlay() override;
};
