// ShootThemUpGame


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
// Allow to work with OnTakeAnyDamage
//#include "Dev/STUFireDamageType.h"
//#include "Dev/STUIceDamageType.h"
// Include Damage Types Classes
#include "Math/UnrealMathUtility.h"
// Allow work with FMath
#include "Engine/World.h"
#include "TimerManager.h"
// Allow work with timer and world time


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	// Will not use Tick in this component
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);
	//Set Start Health to Max

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
	// !!! Bind engine function to pawn (when TakeDamage call, this OnTakeAnyDamage call)
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
// Declare functuion body
{
	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
	// Check that isdead not true or damage not < 0 or world is not exist
	SetHealth(Health - Damage);
	// Decrease Health in function
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	// Clear heal timer when damaged
	
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	// !!! Broadcast to all signed actors that event called if IsDead true
	else if (AutoHeal) 
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	// If alive, check Autoheal true, Get timer from world and set timer with params
	// (HealTimerHandle(Timer body), actor which is timer inside, call function inside, Update rate(calling function every...) , Replay?, Delay before start
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld()) // Avoid bug when call operator ==
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
	// Timer calling every HealUpdateTime in secs by engine function
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	// Modify Health but not allow to increase more than MaxHealth
	OnHealthChanged.Broadcast(Health);
	// !!!Broadcast to all signed actors with param
}
bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) return false;
	SetHealth(Health + HealthAmount);
	return true;
}
bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}