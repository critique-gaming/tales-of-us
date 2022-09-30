#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "LevelResult.generated.h"

USTRUCT(BlueprintType)
struct FLevelResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UTexture2D* CharacterImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FText> TextLines;
};