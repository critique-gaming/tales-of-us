#pragma once
#include "TalesOfUs/TalesOfUs.h"
#include "LevelResult.h"

#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<float, FLevelResult> ResultMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ChoiceId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MapId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BeatsPerMinute = 130.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BeatStartOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* Music;
};
