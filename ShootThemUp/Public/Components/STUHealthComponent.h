// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUHealthComponent();


	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const { return Health / MaxHealth; }
	// For WBP

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;
	// !!! Declare of delegate
	
	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

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

	void PlayCameraShake();

	void Killed(AController* KillerController);
};
