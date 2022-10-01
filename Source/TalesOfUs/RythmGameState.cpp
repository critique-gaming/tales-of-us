#include "RythmGameState.h"
#include "TalesOfUs/UI/MainUI.h"
#include "ObstacleActor.h"
#include "TimerManager.h"

void ARythmGameState::BeginPlay()
{
	Super::BeginPlay();

	check(MainUIClass != nullptr);

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI != nullptr);

	MainUI->AddToViewport();

	FTimerHandle StartTimer;
	GetWorld()->GetTimerManager().SetTimer(StartTimer, FTimerDelegate::CreateWeakLambda(this, [this]() {
		if (!StartingChoiceId.IsNone()) {
			ShowChoiceDialog(StartingChoiceId);
		} else if (!StartingLevelId.IsNone()) {
			AdvanceLevel(StartingLevelId);
		}
	}), 0.5, false);
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
	if (!SelectedLevel->ChoiceId.IsNone()) {
		ShowChoiceDialog(SelectedLevel->ChoiceId);
	}
}

void ARythmGameState::ShowChoiceDialog(FName ChoiceId)
{
	CurrentChoice = &Choices[ChoiceId];
	OnOptionChange.Broadcast(CurrentChoice->FirstCharacter, CurrentChoice->SecondCharacter);
}

void ARythmGameState::AdvanceOptionsDialogue()
{
	if (OptionDialogueIndex > CurrentChoice->DialogueLines.Num()) { return; }

	if (OptionDialogueIndex == CurrentChoice->DialogueLines.Num()) {
		const FText& LeftText = CurrentChoice->FirstChoiceText;
		FName LeftId = CurrentChoice->FirstChoiceLevel;
		const FText& RightText = CurrentChoice->SecondChoiceText;
		FName RightId = CurrentChoice->FirstChoiceLevel;

		OnButtonUpdate.Broadcast(LeftText, LeftId, RightText, RightId);
		OptionDialogueIndex++;
		return;
	}

	OnAdvanceOptionDialogue.Broadcast(CurrentChoice->DialogueLines[OptionDialogueIndex]);
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
