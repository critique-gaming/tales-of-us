#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "Blueprint/UserWidget.h"

#include "DialogueTextBlock.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UDialogueTextBlock : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* TextBlock;
};