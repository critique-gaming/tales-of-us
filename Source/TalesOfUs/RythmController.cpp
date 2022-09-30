#include "RythmController.h"
#include "RythmGameState.h"

void ARythmController::BeginPlay()
{
	Super::BeginPlay();

	if (ARythmGameState* GameState = Cast<ARythmGameState>(GetWorld()->GetGameState())) {
		GameState->OnJump.AddDynamic(this, &ARythmController::HandleJump);
	}
}

void ARythmController::HandleJump()
{
	UE_LOG(LogTemp, Display, TEXT("Jump"));
}
