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
    check(DialogueTextBlock != nullptr);
}

void UMainUI::ShowEndLevelDialogue(FText Text)
{
    
}

void UMainUI::ShowOptionDialogue(FText Text)
{

}






