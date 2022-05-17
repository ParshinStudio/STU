// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"



class USkeletalMeshComponent;



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

	virtual void BeginPlay() override;

	virtual void MakeShot();

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void LogAmmo();

private:
	FAmmoData CurrentAmmo;

};