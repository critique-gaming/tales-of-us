#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "Blueprint/UserWidget.h"

#include "MainUI.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UMainUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UCanvasPanel* MainCanvas;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UEndLevel* EndLevel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOption* Option;

// Properties
public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UDialogueTextBlock> DialogueTextBlock;

    UPROPERTY(BlueprintReadOnly)
    class ARythmGameState* GameState;

public:
    void ShowEndLevelDialogue(FText Text);
    void ShowOptionDialogue(FText Text);

private:
    UFUNCTION()
    void NativeOnInitialized() override;
};