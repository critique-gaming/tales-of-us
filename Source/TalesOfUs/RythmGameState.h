#pragma once

#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/GameStateBase.h"
#include "LevelInfo.h"

#include "RythmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRythmSimpleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceEndDialogueDelegate, FText, DialogueLine);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateEndVisuals, const struct FSlateBrush&, Brush);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceOptionDialogueDelegate, const struct FDialogueItem&, DialogueItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateOptionVisuals, const struct FSlateBrush&, LeftBrush, const struct FSlateBrush&, RightBrush);

UCLASS(Blueprintable, BlueprintType)
class ARythmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FRythmSimpleDelegate OnJump;

	UPROPERTY()
	FRythmSimpleDelegate OnLevelChange;

	UPROPERTY()
	FRythmSimpleDelegate OnHideEndLevel;

	UPROPERTY() 
	FAdvanceOptionDialogueDelegate OnAdvanceOptionDialogue;

	UPROPERTY()
	FAdvanceEndDialogueDelegate OnAdvanceEndLevelDialogue;

	UPROPERTY()
	FUpdateEndVisuals OnLevelEnd;

	UPROPERTY()
	FUpdateOptionVisuals OnOptionChange;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(EditAnywhere)
	TMap<FName, FLevelInfo> LevelInfoMap;

	FLevelInfo SelectedLevel;

public:
	UFUNCTION()
	void AdvanceOptionsDialogue();
	
	UFUNCTION()
	void AdvanceEndLevelDialogue();

	UFUNCTION()
	void AdvanceLevel(FName LevelId);

	UFUNCTION()
	void HandleEndOfLevel();

private:
	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY()
	int32 OptionDialogueIndex = 0;
	
	UPROPERTY()
	int32 EndLevelDialogueIndex = 0;
	
protected:
	void BeginPlay() override;

private:
	void TestUI();
};
