#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "LevelResult.generated.h"

USTRUCT(BlueprintType)
struct FLevelResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    struct FSlateBrush CharacterBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FText> TextLines;
};