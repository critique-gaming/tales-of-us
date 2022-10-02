#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "Blueprint/UserWidget.h"
#include "TalesOfUs/Choice.h"

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
    class UTextBlock* LeftName;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* RightName;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* ContentBox;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* LeftOptionButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* RightOptionButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* LeftOptionText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* RightOptionText;

    UPROPERTY(BlueprintReadOnly)
    class ARythmGameState* GameState;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* InAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* OutAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* ButtonShow = nullptr;

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UDialogueTextBlock> DialogueTextBlockClass;

public:
    UFUNCTION()
    void UpdateVisuals(const FDialogueCharacter& LeftCharacter, const FDialogueCharacter& RightCharacter);

    UFUNCTION()
    void UpdateButtons(const FText& LeftChoiceText, FName _LeftLevelId, const FText& RightChoiceText, FName _RightLevelId);

    UFUNCTION()
    void AddDialogueLine(const FDialogueItem& DialogueItem);

    UFUNCTION()
    void Hide();

protected:
    void NativeOnInitialized() override;
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY()
	FText ShortLeftName;

	UPROPERTY()
	FText ShortRightName;

    // TODO: These should be kept on a button class but i'm too lazy to do that now
    UPROPERTY()
    FName LeftLevelId;

    UPROPERTY()
    FName RightLevelId;

private:
    UFUNCTION()
    void MakeLeftChoice();

    UFUNCTION()
    void MakeRightChoice();
};
