#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "Blueprint/UserWidget.h"

#include "EndLevel.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UEndLevel : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* ResultImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UScrollBox* ScrollPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* ContentBox;
    
public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UDialogueTextBlock> DialogueTextBlockClass;

    UPROPERTY(BlueprintReadOnly)
    class ARythmGameState* GameState;

public:
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    void AddDialogueLine(FText Text);
};
