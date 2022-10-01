#include "EndLevel.h"
#include "TalesOfUs/RythmGameState.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

void UEndLevel::AddDialogueLine(FText Text)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

    NewTextBlock->TextBlock->SetText(Text);
    ContentBox->AddChild(NewTextBlock);
}

void UEndLevel::UpdateVisuals(const FSlateBrush& Brush)
{
    SetVisibility(ESlateVisibility::Visible);
    ResultImage->SetBrush(Brush);
    // TODO: Appear animation
}

FReply UEndLevel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // TODO: Call back into the game state
    GameState->AdvanceEndLevelDialogue();
    return FReply::Handled();
}

void UEndLevel::FinishEndPresentation()
{
    // TODO: Fade out animation
    GameState->ShowOption();
    SetVisibility(ESlateVisibility::Hidden);
}

void UEndLevel::Hide()
{
    SetVisibility(ESlateVisibility::Hidden);
}

