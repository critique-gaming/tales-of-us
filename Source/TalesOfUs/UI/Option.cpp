#include "Option.h"
#include "TalesOfUs/RythmGameState.h"
#include "TalesOfUs/Choice.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"

#define LOCTEXT_NAMESPACE "Option"

void UOption::NativeOnInitialized()
{
    LeftOptionButton->SetVisibility(ESlateVisibility::Hidden);
    RightOptionButton->SetVisibility(ESlateVisibility::Hidden);

    LeftOptionButton->OnClicked.AddDynamic(this, &UOption::MakeLeftChoice);
    RightOptionButton->OnClicked.AddDynamic(this, &UOption::MakeRightChoice);
}

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

void UOption::UpdateButtons(const FText& LeftChoiceText, FName _LeftLevelId, const FText& RightChoiceText, FName _RightLevelId)
{
    LeftOptionText->SetText(LeftChoiceText);
    RightOptionText->SetText(RightChoiceText);
    LeftOptionButton->SetVisibility(ESlateVisibility::Visible);
    RightOptionButton->SetVisibility(ESlateVisibility::Visible);

    LeftLevelId = _LeftLevelId;
    RightLevelId = _RightLevelId;
}

void UOption::Hide()
{
    SetVisibility(ESlateVisibility::Hidden);
    LeftOptionButton->SetVisibility(ESlateVisibility::Hidden);
    LeftOptionButton->SetVisibility(ESlateVisibility::Hidden);
}

void UOption::MakeLeftChoice()
{
    GameState->AdvanceLevel(LeftLevelId);
}

void UOption::MakeRightChoice()
{
    GameState->AdvanceLevel(RightLevelId);
}

#undef LOCTEXT_NAMESPACE
