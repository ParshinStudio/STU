// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"



class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;


UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	ASTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;
	void ChangeClip();
	bool CanReload() const;

	virtual void StartFire();
	virtual void StopFire();
	// Will be used misc blueprints, thats why its virtual

	FWeaponUIData GetUIData() const { return UIData; }
	// Func to return UIData to other class
	FAmmoData GetAmmoData() const { return CurrentAmmo; }
	// Func to return CurrentAmmo to other class

	bool TryToAddAmmo(int32 ClipsAmount); // for ammo pickup called from weapon component
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const; // for ammo add and decorator

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	// Declare component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";
	// Declare Variable for socket
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FAmmoData DefaultAmmo {15, 10, false};
	// Default weapon structure

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWeaponUIData UIData;
	// Declare UIData for weapons

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem* MuzzleFX;
	// Socket Niagara system in weapon
	
	virtual void BeginPlay() override;

	virtual void MakeShot();

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreseAmmo();
	bool IsClipEmpty() const;
	void LogAmmo();

	UNiagaraComponent* SpawnMuzzleFX();

private:
	FAmmoData CurrentAmmo;

};