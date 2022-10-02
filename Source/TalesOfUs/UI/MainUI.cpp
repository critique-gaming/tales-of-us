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

    GameState->OnLevelEnd.AddUObject(EndLevel, &UEndLevel::UpdateVisuals);
    GameState->OnLevelIntro.AddUObject(EndLevel, &UEndLevel::UpdateVisuals);
    GameState->OnLevelChange.AddDynamic(EndLevel, &UEndLevel::Hide);
    GameState->OnHideEndLevel.AddDynamic(EndLevel, &UEndLevel::FinishEndPresentation); // TODO: Change function to call back into gamestate to reveal options
    GameState->OnAdvanceEndLevelDialogue.AddDynamic(EndLevel, &UEndLevel::AddDialogueLine);

    GameState->OnOptionChange.AddDynamic(Option, &UOption::UpdateVisuals);
    GameState->OnLevelChange.AddDynamic(Option, &UOption::Hide);
    GameState->OnAdvanceOptionDialogue.AddDynamic(Option, &UOption::AddDialogueLine);
    GameState->OnButtonUpdate.AddDynamic(Option, &UOption::UpdateButtons);

    EndLevel->SetVisibility(ESlateVisibility::Hidden);
	Option->SetVisibility(ESlateVisibility::Hidden);
}







