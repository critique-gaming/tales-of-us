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
    class UImage* Background;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UEndLevel* EndLevel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOption* Option;

// Properties
public:
    UPROPERTY(BlueprintReadOnly)
    class ARythmGameState* GameState;

private:
    UFUNCTION()
    void NativeOnInitialized() override;
};