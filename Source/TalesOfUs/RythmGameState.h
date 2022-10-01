#pragma once

#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/GameStateBase.h"
#include "LevelInfo.h"

#include "RythmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRythmSimpleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceEndDialogueDelegate, FText, DialogueLine);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateEndVisuals, FLevelResult*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceOptionDialogueDelegate, const struct FDialogueItem&, DialogueItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateOptionVisuals, const struct FDialogueCharacter&, LeftCharacter, const struct FDialogueCharacter&, RightCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUpdateOptionButtons, const FText&, LeftText, FName, LeftId, const FText&, RightText, FName, RightId);

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

	FUpdateEndVisuals OnLevelEnd;

	UPROPERTY()
	FUpdateOptionVisuals OnOptionChange;

	UPROPERTY()
	FUpdateOptionButtons OnButtonUpdate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(EditAnywhere)
	TMap<FName, FLevelInfo> LevelInfoMap;

	FLevelInfo* SelectedLevel = nullptr;
	FLevelResult* LevelResult = nullptr;

public:
	UFUNCTION()
	void AdvanceOptionsDialogue();

	UFUNCTION()
	void AdvanceEndLevelDialogue();

	UFUNCTION()
	void AdvanceLevel(FName LevelId);

	UFUNCTION(BlueprintCallable)
	void EndLevel();

	UFUNCTION()
	void ShowOption();

private:
	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY()
	int32 OptionDialogueIndex = 0;

	UPROPERTY()
	int32 EndLevelDialogueIndex = 0;

public:
	UPROPERTY()
	TArray<class AObstacleActor*> Obstacles;

	UPROPERTY(BlueprintReadWrite)
	AActor* LevelEndActor;

protected:
	void BeginPlay() override;
};
