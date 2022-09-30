#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "LevelResult.h"
#include "Choice.h"

#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<float, FLevelResult> ResultMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FChoice Choice;
};