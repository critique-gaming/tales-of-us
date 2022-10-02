#include "Option.h"
#include "TalesOfUs/RythmGameState.h"
#include "TalesOfUs/Choice.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"
#include "TimerManager.h"

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
    GameState->AdvanceOptionsDialogue();
    return FReply::Handled();
}

void UOption::AddDialogueLine(const FDialogueItem& DialogueItem)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

	FText CharacterName = DialogueItem.OwnerId == 0 ? ShortLeftName : ShortRightName;
    NewTextBlock->TextBlock->SetText(DialogueItem.LineText);
    NewTextBlock->NameTextBlock->SetText(CharacterName);
	NewTextBlock->NameTextBlock->SetVisibility(ESlateVisibility::Visible);
    NewTextBlock->TextBlock->SetJustification(DialogueItem.OwnerId == 0 ? ETextJustify::Left : ETextJustify::Right);
    NewTextBlock->NameTextBlock->SetJustification(DialogueItem.OwnerId == 0 ? ETextJustify::Left : ETextJustify::Right);
	NewTextBlock->FadeIn();
    ContentBox->AddChild(NewTextBlock);

    // TODO: Animations for images and textblocks
}

void UOption::UpdateVisuals(const FDialogueCharacter& LeftCharacter, const FDialogueCharacter& RightCharacter)
{
    LeftOptionImage->SetBrush(LeftCharacter.CharacterImage);
    RightOptionImage->SetBrush(RightCharacter.CharacterImage);
    LeftName->SetText(LeftCharacter.Name);
    RightName->SetText(RightCharacter.Name);
	ShortLeftName = LeftCharacter.ShortName.IsEmpty() ? LeftCharacter.Name : LeftCharacter.ShortName;
	ShortRightName = RightCharacter.ShortName.IsEmpty() ? RightCharacter.Name : RightCharacter.ShortName;

	ContentBox->ClearChildren();
	PlayAnimation(InAnimation);
}

void UOption::UpdateButtons(const FText& LeftChoiceText, FName _LeftLevelId, const FText& RightChoiceText, FName _RightLevelId)
{
    LeftOptionText->SetText(LeftChoiceText);
    RightOptionText->SetText(RightChoiceText);

    LeftLevelId = _LeftLevelId;
    RightLevelId = _RightLevelId;

	PlayAnimation(ButtonShow);
}

void UOption::Hide()
{
	PlayAnimation(OutAnimation);
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
