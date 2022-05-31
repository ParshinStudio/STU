// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class ASTUProjectile;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

protected:
	virtual void MakeShot() override;
};
