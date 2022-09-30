#pragma once

#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/GameStateBase.h"
#include "LevelInfo.h"

#include "RythmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRythmSimpleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceEndDialogueDelegate, FText, DialogueLine);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAdvanceOptionDialogueDelegate, FText, DialogueLine, int32, OwnerId);


UCLASS(Blueprintable, BlueprintType)
class ARythmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FRythmSimpleDelegate OnJump;

	UPROPERTY() 
	FAdvanceOptionDialogueDelegate OnAdvanceOptionDialogue;

	UPROPERTY()
	FAdvanceEndDialogueDelegate OnAdvanceEndLevelDialogue;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(EditAnywhere)
	TMap<FName, FLevelInfo> LevelInfoMap;

private:
	UPROPERTY()
	UMainUI* MainUI;

protected:
	void BeginPlay() override;
};
