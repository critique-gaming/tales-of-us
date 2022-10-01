#include "RythmGameState.h"
#include "TalesOfUs/UI/MainUI.h"
#include "ObstacleActor.h"

void ARythmGameState::BeginPlay()
{
	Super::BeginPlay();

	check(MainUIClass != nullptr);

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI != nullptr);

	MainUI->AddToViewport();

	AdvanceLevel(FName(TEXT("TestLevel")));
}

void ARythmGameState::EndLevel()
{
	int32 CorrectAnswers = 0;
	for (AObstacleActor* Obstacle : Obstacles) {
		if (Obstacle->bWasSquished == Obstacle->bShouldSquish) {
			CorrectAnswers += 1;
		}
	}
	float Score = (float)CorrectAnswers / Obstacles.Num();

	TArray<float> Tresholds;
	SelectedLevel->ResultMap.GenerateKeyArray(Tresholds);
	Tresholds.Sort();

	LevelResult = nullptr;
	for (float Treshold : Tresholds) {
		if (Score <= Treshold) {
			LevelResult = &SelectedLevel->ResultMap[Treshold];
		}
	}

	OnLevelEnd.Broadcast(LevelResult);
}

void ARythmGameState::AdvanceEndLevelDialogue()
{
	if (EndLevelDialogueIndex >= LevelResult->TextLines.Num()) {
		// TODO: The OnHideEndLevel should trigger a fade out animation
		// TODO: After that animation finishes it should call back into the GameState to update the options menu
		OnHideEndLevel.Broadcast();
		return;
	}

	OnAdvanceEndLevelDialogue.Broadcast(LevelResult->TextLines[EndLevelDialogueIndex]);
	EndLevelDialogueIndex++;
	return;
}

void ARythmGameState::ShowOption()
{
	OnOptionChange.Broadcast(SelectedLevel->Choice.FirstCharacter, SelectedLevel->Choice.SecondCharacter);
}

void ARythmGameState::AdvanceOptionsDialogue()
{
	if (OptionDialogueIndex > SelectedLevel->Choice.DialogueLines.Num()) { return; }

	if (OptionDialogueIndex == SelectedLevel->Choice.DialogueLines.Num()) {
		const FText& LeftText = SelectedLevel->Choice.FirstChoiceText;
		FName LeftId = SelectedLevel->Choice.FirstChoiceLevel;
		const FText& RightText = SelectedLevel->Choice.SecondChoiceText;
		FName RightId = SelectedLevel->Choice.FirstChoiceLevel;

		OnButtonUpdate.Broadcast(LeftText, LeftId, RightText, RightId);
		OptionDialogueIndex++;
		return;
	}

	OnAdvanceOptionDialogue.Broadcast(SelectedLevel->Choice.DialogueLines[OptionDialogueIndex]);
	OptionDialogueIndex++;
}

void ARythmGameState::AdvanceLevel(FName LevelId)
{
	EndLevelDialogueIndex = 0;
	OptionDialogueIndex = 0;
	SelectedLevel = &LevelInfoMap[LevelId];

	OnLevelChange.Broadcast();

	// TODO: Prepare Level i guess
}
