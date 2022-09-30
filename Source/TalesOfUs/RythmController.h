#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "RythmController.generated.h"

USTRUCT()
struct FLegState {
	GENERATED_BODY()

	FVector CurrentPosition;
	FVector LastPosition;

	UPROPERTY()
	class UTween* Tween = nullptr;

	UPROPERTY()
	AActor* Actor = nullptr;

	int32 Index = 0;
};

UCLASS(Blueprintable, BlueprintType)
class ARythmController : public AActor {
	GENERATED_BODY()

public:
	ARythmController();

	void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* LiftSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* DropSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* AdvanceSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* StumbleSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* IdleBeatSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BeatsPerMinute = 60.0f;

	UPROPERTY(EditAnywhere)
	float OvershootTolerance = 0.25; // Percent of beat interval

	UPROPERTY(EditAnywhere)
	float UndershootTolerance = 0.25; // Percent of beat interval

	UPROPERTY(EditAnywhere)
	float QuantizeTolerance = 0.2; // Seconds

private:
	UPROPERTY()
	TArray<FLegState> LegStates;

	float TimeToBeat = 0.0f;
	float BeatInterval = 0.0f;
	bool bPerformedActionThisBeat = false;
	bool bQueuedAction = false;
	int32 LiftedLegIndex = -1;
	int32 NextLegToLift = 0;
	FVector CameraOffset;
	FVector CameraTarget;

	UFUNCTION()
	void HandleJump();

	void LiftUpLeg(int32 LegIndex, float Duration);
	void DropLeg(int32 LegIndex, float Duration);
	void PerformAction(float Duration);
	void Stumble();
	void AdvanceLeg(int32 LegIndex, float Duration);
	void PerformIdleAction(float Duration);
	void CancelAnimation(bool bComplete);
	void ApplyLegAnimation(FLegState& LegState);

protected:
	void Tick(float DeltaTime) override;
};
