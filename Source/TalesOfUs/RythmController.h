#pragma once
#include "TalesOfUs/TalesOfUs.h"

#include "RythmController.generated.h"

USTRUCT()
struct FLegState {
	GENERATED_BODY()

	bool bIsLifted = false;

	bool bHasRaycastOffset = false;
	FVector RaycastOffset;

	FVector LiftPosition;
	FVector InitialPosition;
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
	class USoundBase* TransitionMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* LiftSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* DropSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* AdvanceSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* StumbleSFX;

	UPROPERTY(EditAnywhere)
	float OvershootTolerance = 0.25; // Percent of beat interval

	UPROPERTY(EditAnywhere)
	float UndershootTolerance = 0.25; // Percent of beat interval

	UPROPERTY(EditAnywhere)
	float MinOvershootTolerance = 0.15; // Seconds

	UPROPERTY(EditAnywhere)
	float MinUndershootTolerance = 0.15; // Seconds

	UPROPERTY(EditAnywhere)
	FVector LiftedOffset = {0.0f, 0.0f, 200.0f};

	UPROPERTY(EditAnywhere)
	float QuantizeTolerance = 0.2; // Seconds

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TimeSignature = 8;

public:
	UPROPERTY(BlueprintReadOnly)
	float TimeToBeat = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float BeatInterval = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	int32 BeatIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsEnabled = false;

	UPROPERTY(BlueprintReadOnly)
	int32 LiftedLegIndex = -1;

private:
	UPROPERTY()
	TArray<FLegState> LegStates;


	bool bPerformedActionThisBeat = false;
	bool bQueuedAction = false;
	FVector CameraOffset;
	FVector CameraTarget;

	UPROPERTY()
	class ARythmGameState* GameState = nullptr;

	const struct FLevelInfo* QueuedLevelToStart = nullptr;

	UPROPERTY()
	class UAudioComponent* MusicComponent = nullptr;

	UFUNCTION()
	void HandleJump();

	void HandleLevelEnd(struct FLevelResult* LevelResult);

	UFUNCTION()
	void HandleLevelChange();

	void LiftUpLeg(int32 LegIndex, float Duration);
	void LiftUpLastLeg(float Duration);
	void DropLeg(int32 LegIndex, float Duration);
	void PerformAction(float Duration);
	void Stumble();
	void AdvanceLeg(int32 LegIndex, float Duration);
	void PerformIdleAction(float Duration);
	void CancelAnimation(bool bComplete);
	void ApplyLegAnimation(FLegState& LegState);
	void UpdateCamera();

	void QueueStartLevel(const struct FLevelInfo& LevelInfo);
	void StartLevel(const struct FLevelInfo& LevelInfo);
	void StopLevel();

	bool IsLegOverlapping(const FLegState& LegState);

protected:
	void Tick(float DeltaTime) override;
};
