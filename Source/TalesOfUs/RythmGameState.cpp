#include "RythmGameState.h"
#include "TalesOfUs/UI/MainUI.h"
#include "ObstacleActor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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
	}), 0.01, false);
}

void ARythmGameState::EndLevel()
{
	check(Phase == ERythmGamePhase::Level);

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

	UE_LOG(LogTemp, Display, TEXT("Score %f"), Score);

	LevelResult = nullptr;
	for (int32 Index = Tresholds.Num() - 1; Index >= 0; Index -= 1) {
		float Treshold = Tresholds[Index];
		if (Score <= Treshold) {
			LevelResult = &SelectedLevel->ResultMap[Treshold];
			UE_LOG(LogTemp, Display, TEXT("Met threshold %f"), Treshold);
		}
	}

	Phase = ERythmGamePhase::Result;
	EndLevelDialogueIndex = 0;
	OnLevelEnd.Broadcast(LevelResult);

	AdvanceEndLevelDialogue();
}

void ARythmGameState::AdvanceEndLevelDialogue()
{
	if (Phase != ERythmGamePhase::Result) return;
	if (EndLevelDialogueIndex >= LevelResult->TextLines.Num()) {
		// TODO: The OnHideEndLevel should trigger a fade out animation
		// TODO: After that animation finishes it should call back into the GameState to update the options menu
		Phase = ERythmGamePhase::Idle;
		OnHideEndLevel.Broadcast();
		return;
	}

	FText TextLine = LevelResult->TextLines[EndLevelDialogueIndex];
	EndLevelDialogueIndex++;

	OnAdvanceEndLevelDialogue.Broadcast(TextLine);
}

void ARythmGameState::ShowOption()
{
	if (!SelectedLevel->ChoiceId.IsNone()) {
		ShowChoiceDialog(SelectedLevel->ChoiceId);
	}
}

void ARythmGameState::ShowChoiceDialog(FName ChoiceId)
{
	Phase = ERythmGamePhase::Choice;

	OptionDialogueIndex = 0;
	CurrentChoice = &Choices[ChoiceId];

	OnOptionChange.Broadcast(CurrentChoice->FirstCharacter, CurrentChoice->SecondCharacter);

	AdvanceOptionsDialogue();
}

void ARythmGameState::AdvanceOptionsDialogue()
{
	if (Phase != ERythmGamePhase::Choice) return;

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

	FDialogueItem& TextLine = CurrentChoice->DialogueLines[OptionDialogueIndex];
	OptionDialogueIndex += 1;

	OnAdvanceOptionDialogue.Broadcast(TextLine);
}

void ARythmGameState::AdvanceLevel(FName LevelId)
{
	Phase = ERythmGamePhase::Level;

	FName LoadedLevel;
	if (SelectedLevel != nullptr) LoadedLevel = SelectedLevel->MapId;

	SelectedLevel = &LevelInfoMap[LevelId];

	for (AObstacleActor* Obstacle : Obstacles) {
		Obstacle->Reset();
	}

	if (LoadedLevel != SelectedLevel->MapId) {
		Obstacles.Empty();
		LevelEndActor = nullptr;

		if (!LoadedLevel.IsNone()) {
			FLatentActionInfo LatentInfo;
			UGameplayStatics::UnloadStreamLevel(this, SelectedLevel->MapId, LatentInfo, true);
		}
		if (!SelectedLevel->MapId.IsNone()) {
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevel(this, SelectedLevel->MapId, true, true, LatentInfo);
		}
	}

	OnLevelChange.Broadcast();

	// TODO: Prepare Level i guess
}

void ARythmGameState::Jump()
{
	switch (Phase) {
		case ERythmGamePhase::Result:
			AdvanceEndLevelDialogue();
			break;
		case ERythmGamePhase::Choice:
			AdvanceOptionsDialogue();
			break;
	}
	OnJump.Broadcast();
}
