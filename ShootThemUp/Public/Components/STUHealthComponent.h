// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
// !!! Declare of delegate

UCLASS( ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUHealthComponent();


	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;
	// !!! Declare of delegate
	
protected:

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealUpdateTime = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealDelay = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealModifier = 1.0f;

	virtual void BeginPlay() override;

private:

	float Health = 0.0f;

	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	// !!! OnTakeAnyDamage engine function declare
	//UFUNCTION() // Use GetWorld
	void HealUpdate();
	//UFUNCTION() // Use broadcast
	void SetHealth(float NewHealth);
};