// ShootThemUpGame


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"
// Allow work with char values

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed(); // Create float and get actor MaxSpeed from component
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner()); // Get pawn owner
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed; 
	// Get IsRunning from player and check statement, return match parameter
	// If IsRunning True return MaxSpeed * RunModifier, else MaxSpeed
}
