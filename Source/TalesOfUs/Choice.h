#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "Choice.generated.h"

USTRUCT(BlueprintType)
struct FDialogueItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 OwnerId = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText LineText;
};

USTRUCT(BlueprintType)
struct FDialogueCharacter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UTexture2D* Image = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Id = 0;
};

USTRUCT(BlueprintType)
struct FChoice
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FDialogueCharacter FirstCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FDialogueCharacter SecondCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName FirstChoiceLevel;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SecondChoiceLevel;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueItem> DialogueLines;
};
