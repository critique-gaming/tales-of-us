#include "EndLevel.h"
#include "TalesOfUs/RythmGameState.h"
#include "TalesOfUs/LevelResult.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "TimerManager.h"

void UEndLevel::AddDialogueLine(FText Text)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

    NewTextBlock->TextBlock->SetText(Text);
	NewTextBlock->FadeIn();
    ContentBox->AddChild(NewTextBlock);
}

void UEndLevel::UpdateVisuals(FLevelResult* LevelResult)
{
    SetVisibility(ESlateVisibility::Visible);
	if (LevelResult != nullptr) {
		ResultImage->SetBrush(LevelResult->CharacterBrush);
	}

	ContentBox->ClearChildren();
	PlayAnimation(InAnimation);
}

FReply UEndLevel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    GameState->AdvanceEndLevelDialogue();
    return FReply::Handled();
}

void UEndLevel::FinishEndPresentation()
{
	PlayAnimation(OutAnimation);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]() {
		SetVisibility(ESlateVisibility::Hidden);
		GameState->ShowOption();
	}), OutAnimation->GetEndTime(), false);
}

void UEndLevel::Hide()
{
    SetVisibility(ESlateVisibility::Hidden);
}

