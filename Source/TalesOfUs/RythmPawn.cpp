#include "RythmPawn.h"
#include "RythmGameState.h"

void ARythmPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ARythmPawn::HandleJump);
}

void ARythmPawn::HandleJump()
{
	if (ARythmGameState* GameState = Cast<ARythmGameState>(GetWorld()->GetGameState())) {
		GameState->Jump();
	}
}
