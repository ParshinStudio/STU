// ShootThemUpGame


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
//#include "Dev/STUFireDamageType.h"
//#include "Dev/STUIceDamageType.h"
#include "Math/UnrealMathUtility.h"
// Allow work with FMath
#include "Engine/World.h"
#include "TimerManager.h"
// Allow work with timer and world time
#include "Camera/CameraComponent.h"
#include "STUGameModeBase.h"
#include "Perception/AISense_Damage.h"

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
		Killed(InstigatedBy); // Send Controller of class which  called OnTakeAnyDamage
		OnDeath.Broadcast();
	}
	// !!! Broadcast to all signed actors that event called if IsDead true
	else if (AutoHeal) 
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	// If alive, check Autoheal true, Get timer from world and set timer with params
	// (HealTimerHandle(Timer body), actor which is timer inside, call function inside, Update rate(calling function every...) , Replay?, Delay before start
	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
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
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;
	Health = NextHealth;
	// Modify Health but not allow to increase more than MaxHealth
	OnHealthChanged.Broadcast(Health, HealthDelta);
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

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;
	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}