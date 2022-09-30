#include "RythmGameState.h"
#include "TalesOfUs/UI/MainUI.h"

void ARythmGameState::BeginPlay()
{
	Super::BeginPlay();

	check(MainUIClass != nullptr);

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI != nullptr);

	MainUI->AddToViewport();
}

