// ShootThemUpGame


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = STUUTils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	// Make object from pointer
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = STUUTils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
	// Return UIData to Blueprint to have UIData for weapon in hands
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = STUUTils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
	// Return UIData to Blueprint to have UIData for weapon in hands
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = STUUTils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
		// When new pawn, OnNewPawn calling (delegate in engine controller)
	}
	OnNewPawn(GetOwningPlayerPawn()); // First time need to call manually to get new pawn
}
void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();
	}
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = STUUTils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
	if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this)) // Check that delegate signed 
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged); 
		// Sign on delegate in health component
	}

}

/* FOR USING WITHOUT STUUtils template
USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
	// Get pointer to component
	const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
	// Make object from pointer
	return WeaponComponent;
}

USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
	// Get pointer to component
	const auto HealthComponent = Cast<USTUHealthComponent>(Component);
	// Make object from pointer
	return HealthComponent;
}
*/