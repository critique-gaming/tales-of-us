#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "Blueprint/UserWidget.h"

#include "Option.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UOption : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* LeftOptionImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* RightOptionImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UScrollBox* ScrollPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* ContentBox;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* LeftOptionButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* RightOptionButton;

    UPROPERTY(BlueprintReadOnly)
    class ARythmGameState* GameState;

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UDialogueTextBlock> DialogueTextBlockClass;

public:
    void AddDialogueLine(FText Tex, int32 OwnerId);

protected:
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
