#include "Option.h"
#include "TalesOfUs/RythmGameState.h"
#include "TalesOfUs/Choice.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"

#define LOCTEXT_NAMESPACE "Option"

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

    UTextBlock* CharacterName = DialogueItem.OwnerId == 0 ? LeftName : RightName;
    FText Result = FText::Format(
        LOCTEXT("DialogueText", "{Name}: {TextLine}"),
        {
            {TEXT("Name"), CharacterName->GetText()},
            {TEXT("TextLine"), DialogueItem.LineText}
        }
    );

    NewTextBlock->TextBlock->SetText(Result);
    NewTextBlock->TextBlock->SetJustification(DialogueItem.OwnerId == 0 ? ETextJustify::Left : ETextJustify::Right);
    ContentBox->AddChild(NewTextBlock);

    // TODO: Animations for images and textblocks
}

void UOption::UpdateVisuals(const FDialogueCharacter& LeftCharacter, const FDialogueCharacter& RightCharacter)
{
    SetVisibility(ESlateVisibility::Visible);
    LeftOptionImage->SetBrush(LeftCharacter.CharacterImage);
    RightOptionImage->SetBrush(RightCharacter.CharacterImage);
    LeftName->SetText(LeftCharacter.Name);
    RightName->SetText(RightCharacter.Name);

    // TODO: Popin animations
}

void UOption::Hide()
{
    SetVisibility(ESlateVisibility::Hidden);
}

#undef LOCTEXT_NAMESPACE
