// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent; // Forward niagara declaration
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
	ASTURifleWeapon(); // For niagara

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override; // For niagara
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBetweenShots = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageAmount = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent; // Niagara component

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

private:
	FTimerHandle ShotTimerHandle;
	// Timer for shooting loop
	void MakeDamage(const FHitResult& HitResult);

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
	void InitMuzzleFX();
	void SetMuzzleFXVisibility(bool Visible);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;
};
