// ShootThemUpGame

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
// Show owner


USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}


void USTUWeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	// Get owner of character
	if (!Character) return;
	CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
	// Spawn weapon
	if (!CurrentWeapon) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	// Create attachment rules
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	// Attach component to skeletal mesh with rules to socket
	CurrentWeapon->SetOwner(GetOwner());
	// Set Owner to have it in BaseWeapon
	
}

void USTUWeaponComponent::StartFire() 
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();
	// Calling Fire in Base Weapon
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	// Calling Fire in Base Weapon
}