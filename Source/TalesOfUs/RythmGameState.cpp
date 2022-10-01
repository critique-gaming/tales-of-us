#include "RythmGameState.h"
#include "TalesOfUs/UI/MainUI.h"

void ARythmGameState::BeginPlay()
{
	Super::BeginPlay();

	check(MainUIClass != nullptr);

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI != nullptr);

	MainUI->AddToViewport();

	// TODO: Delete
	TestUI();
}

void ARythmGameState::TestUI()
{
	AdvanceLevel(FName(TEXT("TestLevel")));
	HandleEndOfLevel();
}

void ARythmGameState::HandleEndOfLevel()
{
	OnLevelEnd.Broadcast(SelectedLevel.ResultMap[0.0f].CharacterBrush);
}

void ARythmGameState::AdvanceEndLevelDialogue()
{
	if (EndLevelDialogueIndex >= SelectedLevel.ResultMap[0.0f].TextLines.Num()) {
		// TODO: The OnHideEndLevel should trigger a fade out animation
		// TODO: After that animation finishes it should call back into the GameState to update the options menu		
		OnHideEndLevel.Broadcast();
		return;
	}

	OnAdvanceEndLevelDialogue.Broadcast(SelectedLevel.ResultMap[0.0f].TextLines[EndLevelDialogueIndex]);
	EndLevelDialogueIndex++;
	return;
}

void ARythmGameState::ShowOption()
{
	OnOptionChange.Broadcast(SelectedLevel.Choice.FirstCharacter, SelectedLevel.Choice.SecondCharacter);
}

void ARythmGameState::AdvanceOptionsDialogue()
{
	if (OptionDialogueIndex >= SelectedLevel.Choice.DialogueLines.Num()) { return; }

	OnAdvanceOptionDialogue.Broadcast(SelectedLevel.Choice.DialogueLines[OptionDialogueIndex]);
	OptionDialogueIndex++;
}

void ARythmGameState::AdvanceLevel(FName LevelId)
{
	EndLevelDialogueIndex = 0;
	OptionDialogueIndex = 0;
	SelectedLevel = LevelInfoMap[LevelId];

	OnLevelChange.Broadcast();

	// TODO: Prepare Level i guess
}
