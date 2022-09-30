#include "MainUI.h"
#include "DialogueTextBlock.h"
#include "Option.h"
#include "EndLevel.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "TalesOfUs/RythmGameState.h"

void UMainUI::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    GameState = Cast<ARythmGameState>(GetWorld()->GetGameState());
    check(GameState != nullptr);

    EndLevel->GameState = GameState;
    Option->GameState = GameState;

    GameState->OnAdvanceEndLevelDialogue.AddDynamic(EndLevel, &UEndLevel::AddDialogueLine);
    GameState->OnAdvanceOptionDialogue.AddDynamic(Option, &UOption::AddDialogueLine);

    EndLevel->SetVisibility(ESlateVisibility::Hidden);
	Option->SetVisibility(ESlateVisibility::Hidden);
}





