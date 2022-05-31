// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent; // Forward niagara declaration
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
	ASTURifleWeapon(); // For niagara

	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Zoom(bool Enabled) override;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageAmountHead = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent; // Niagara component

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FOVZoom = 50.0f;

private:
	FTimerHandle ShotTimerHandle;
	// Timer for shooting loop
	void MakeDamage(const FHitResult& HitResult);

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
	UPROPERTY()
	UAudioComponent* FireAudioComponent;
	void InitFX();
	void SetFXActive(bool IsActive);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;

	float DefaultCameraFOV = 90.0f;
};
