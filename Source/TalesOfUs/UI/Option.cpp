#include "Option.h"
#include "DialogueTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

FReply UOption::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // TODO: Call something in the game state
    return FReply::Handled();
}

void UOption::AddDialogueLine(FText Text, int32 OwnerId)
{
    UDialogueTextBlock* NewTextBlock = CreateWidget<UDialogueTextBlock>(GetWorld(), DialogueTextBlockClass);
    check(NewTextBlock != nullptr);

    NewTextBlock->TextBlock->SetText(Text);
    NewTextBlock->TextBlock->SetJustification(OwnerId == 0 ? ETextJustify::Left : ETextJustify::Right);
    ContentBox->AddChild(NewTextBlock);
}

