// ShootThemUpGame


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

#include "Components/WidgetComponent.h"
#include "UI/STUHealthWidget.h"
#include "Components/STUHealthComponent.h"


ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) :Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) 
// Custom constructor with params (WeaponComponent replace)
{
	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // When in already created in world
	AutoPossessAI = EAutoPossessAI::Disabled; // When spawn from code (For PlayerState creation)

	AIControllerClass = ASTUAIController::StaticClass();
	// Replacing to blueprint class in editor 
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	} // Smooth rotation when mooving
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
	// For visibility
}

void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthWidgetComponent);
}

void ASTUAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto STUController = Cast<AAIController>(Controller);
	if (STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
		// Clean BT
	}
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USTUHealthWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;
	HealthBarWidget->SetHealthPercent(CharHealthComponent->GetHealthPercent());
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
	HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}