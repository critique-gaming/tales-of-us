#include "EndLevel.h"
#include "DialogueTextBlock.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UEndLevel::AddDialogueLine(FText Text)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

    NewTextBlock->TextBlock->SetText(Text);
    ContentBox->AddChild(NewTextBlock);
}

FReply UEndLevel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // TODO: Call back into the game state
    return FReply::Handled();
}

