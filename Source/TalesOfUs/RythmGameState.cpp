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
	float Score = Obstacles.IsEmpty() ? 1.0f : ((float)CorrectAnswers / Obstacles.Num());

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

	GlobalCorrectAnswers += CorrectAnswers;
	GlobalObstacleCount += Obstacles.Num();

	Phase = ERythmGamePhase::Result;
	EndLevelDialogueIndex = 0;
	OnLevelEnd.Broadcast(LevelResult);

	AdvanceEndLevelDialogue();
}

void ARythmGameState::EndGame()
{
	if (bHasFinished) return;
	bHasFinished = true;

	float Score = GlobalObstacleCount == 0 ? 1.0f : ((float)GlobalCorrectAnswers / GlobalObstacleCount);

	TArray<float> Tresholds;
	GlobalResultMap.GenerateKeyArray(Tresholds);
	Tresholds.Sort();

	UE_LOG(LogTemp, Display, TEXT("Score %f"), Score);

	LevelResult = nullptr;
	for (int32 Index = Tresholds.Num() - 1; Index >= 0; Index -= 1) {
		float Treshold = Tresholds[Index];
		if (Score <= Treshold) {
			LevelResult = &GlobalResultMap[Treshold];
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
	if (Phase != ERythmGamePhase::Result && Phase != ERythmGamePhase::Intro) return;
	if (LevelResult == nullptr) return;
	if (EndLevelDialogueIndex == -1) return;

	if (EndLevelDialogueIndex >= LevelResult->TextLines.Num()) {
		EndLevelDialogueIndex = -1;

		// TODO: The OnHideEndLevel should trigger a fade out animation
		// TODO: After that animation finishes it should call back into the GameState to update the options menu
		OnHideEndLevel.Broadcast();
		return;
	}

	FText TextLine = LevelResult->TextLines[EndLevelDialogueIndex];
	EndLevelDialogueIndex++;

	OnAdvanceEndLevelDialogue.Broadcast(TextLine);
}

void ARythmGameState::ShowOption()
{
	if (Phase == ERythmGamePhase::Intro) {
		Phase = ERythmGamePhase::Level;
		OnLevelBegin.Broadcast();
		return;
	}

	if (!SelectedLevel->ChoiceId.IsNone()) {
		ShowChoiceDialog(SelectedLevel->ChoiceId);
	} else {
		EndGame();
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
		FName RightId = CurrentChoice->SecondChoiceLevel;

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
	FName LoadedLevel;
	if (SelectedLevel != nullptr) LoadedLevel = SelectedLevel->MapId;

	SelectedLevel = &LevelInfoMap[LevelId];

	for (AObstacleActor* Obstacle : Obstacles) {
		Obstacle->Reset();
	}

	Phase = ERythmGamePhase::Idle;

	if (LoadedLevel != SelectedLevel->MapId) {
		Obstacles.Empty();
		LevelEndActor = nullptr;

		OnLoadingLevelChange.Broadcast(true);
		SwapSublevels(LoadedLevel, SelectedLevel->MapId);
	} else {
		FinishAdvanceLevel();
	}
}

void ARythmGameState::FinishAdvanceLevel()
{
	OnLoadingLevelChange.Broadcast(false);

	Phase = ERythmGamePhase::Intro;

	OnLevelChange.Broadcast();

	if (SelectedLevel->Intro.TextLines.IsEmpty()) {
		Phase = ERythmGamePhase::Level;
		OnLevelBegin.Broadcast();
		return;
	}

	LevelResult = &SelectedLevel->Intro;
	EndLevelDialogueIndex = 0;
	OnLevelIntro.Broadcast(LevelResult);

	AdvanceEndLevelDialogue();
}

void ARythmGameState::Jump()
{
	/* switch (Phase) { */
	/* 	case ERythmGamePhase::Result: */
	/* 		AdvanceEndLevelDialogue(); */
	/* 		break; */
	/* 	case ERythmGamePhase::Choice: */
	/* 		AdvanceOptionsDialogue(); */
	/* 		break; */
	/* } */
	OnJump.Broadcast();
}
