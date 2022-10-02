#pragma once

#include "TalesOfUs/TalesOfUs.h"

#include "GameFramework/GameStateBase.h"
#include "LevelInfo.h"
#include "Choice.h"

#include "RythmGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRythmSimpleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceEndDialogueDelegate, FText, DialogueLine);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateEndVisuals, FLevelResult*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvanceOptionDialogueDelegate, const struct FDialogueItem&, DialogueItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateOptionVisuals, const struct FDialogueCharacter&, LeftCharacter, const struct FDialogueCharacter&, RightCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUpdateOptionButtons, const FText&, LeftText, FName, LeftId, const FText&, RightText, FName, RightId);

UENUM(BlueprintType)
enum class ERythmGamePhase : uint8
{
	Idle,
	Intro,
	Level,
	Result,
	Choice,
	Outro
};

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
	FRythmSimpleDelegate OnLevelBegin;

	UPROPERTY()
	FRythmSimpleDelegate OnHideEndLevel;

	UPROPERTY()
	FRythmSimpleDelegate OnBeat;

	UPROPERTY()
	FAdvanceOptionDialogueDelegate OnAdvanceOptionDialogue;

	UPROPERTY()
	FAdvanceEndDialogueDelegate OnAdvanceEndLevelDialogue;

	FUpdateEndVisuals OnLevelEnd;

	FUpdateEndVisuals OnLevelIntro;

	UPROPERTY()
	FUpdateOptionVisuals OnOptionChange;

	UPROPERTY()
	FUpdateOptionButtons OnButtonUpdate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(EditAnywhere)
	TMap<FName, FLevelInfo> LevelInfoMap;

	UPROPERTY(EditAnywhere)
	TMap<FName, FChoice> Choices;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<float, FLevelResult> GlobalResultMap;

	UPROPERTY(EditAnywhere)
	FName StartingChoiceId;

	UPROPERTY(EditAnywhere)
	FName StartingLevelId;

	ERythmGamePhase Phase = ERythmGamePhase::Idle;

	FChoice * CurrentChoice = nullptr;
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

	UFUNCTION()
	void ShowChoiceDialog(FName ChoiceId);

	void Jump();

public:
    UPROPERTY(BlueprintReadOnly)
	int32 GlobalCorrectAnswers = 0;

    UPROPERTY(BlueprintReadOnly)
	int32 GlobalObstacleCount = 0;

private:
	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY()
	int32 OptionDialogueIndex = 0;

	UPROPERTY()
	int32 EndLevelDialogueIndex = 0;

	UPROPERTY()
	bool bHasFinished = false;

	void EndGame();

public:
	UPROPERTY()
	TArray<class AObstacleActor*> Obstacles;

	UPROPERTY(BlueprintReadWrite)
	AActor* LevelEndActor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	class ARythmController* RythmController = nullptr;

protected:
	void BeginPlay() override;
};
