#include "Option.h"
#include "TalesOfUs/RythmGameState.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

FReply UOption::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // TODO: Call something in the game state
    GameState->AdvanceOptionsDialogue();
    return FReply::Handled();
}

void UOption::AddDialogueLine(const FDialogueItem& DialogueItem)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

    NewTextBlock->TextBlock->SetText(DialogueItem.LineText);
    NewTextBlock->TextBlock->SetJustification(DialogueItem.OwnerId == 0 ? ETextJustify::Left : ETextJustify::Right);
    ContentBox->AddChild(NewTextBlock);

    // TODO: Animations for images and textblocks
}

void UOption::UpdateVisuals(const FSlateBrush& LeftBrush, const FSlateBrush& RightBrush)
{
    SetVisibility(ESlateVisibility::Visible);
    LeftOptionImage->SetBrush(LeftBrush);
    RightOptionImage->SetBrush(RightBrush);

    // TODO: Popin animations
}

void UOption::Hide()
{
    SetVisibility(ESlateVisibility::Hidden);
}
