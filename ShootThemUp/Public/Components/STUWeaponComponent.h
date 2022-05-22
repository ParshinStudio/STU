// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
//Declare class to create subclass



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUWeaponComponent();
	
	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const; // Get from base weapon
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const; // Get from base weapon

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

protected:

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;
	// Way to declare object should be spawned  in world
	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;
	// Array of weapons

	int32 CurrentWeaponIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;
	//Array Of Weapons stucts with montage which should be spawned from editor bp

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";
	// Sockets in Char

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;
	// Socket for monage

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire() const;
	bool CanEquip() const;

	void EquipWeapon(int32 WeaponIndex);

private:
	
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;
	// Montage which will be used for current weapon in slot

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
	
	bool CanReload() const;

	void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();

	/* 
	DECLARED IN OTHER HEADER STUUtils
	template<typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation) // UAnimSequenceBase is holder of notifies 
	{
		if (!Animation) return nullptr;
		const auto NotifyEvents = Animation->Notifies;
		// Array of Notifies of montage
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			// Create pointer for notify
			if (AnimNotify) // If notify exist
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}
	*/

};