// ShootThemUpGame

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
// Show owner
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
// Work With anim notify classes

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All)

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Char can hold %i weapon items"), WeaponNum);
	// Check blueprint value is correct

	InitAnimations();

	CurrentWeaponIndex = 0;
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
		// Remove gun from socket and destroy
	}
	Weapons.Empty();
	// Clear Weapons array
	Super::EndPlay(EndPlayReason);
	// Call in engine parent function
}


void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	// Get owner of character
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
		// Spawn weapons
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		// Sign for broadcast and call OnEmptyClip when broadcast
		Weapon->SetOwner(Character);
		// Set Owner to have it in BaseWeapon
		Weapons.Add(Weapon);
		// Add objects to Array
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
		// Attach weapons to armory socket
	}
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(WeaponComponentLog, Warning, TEXT("InvalidWeaponIndex"));
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon) // Nullptr when beginplay
	{
		CurrentWeapon->StopFire();
		// Stop timer when equip another weapon
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
		// Replace in hand weapon to armory socket
	}
	CurrentWeapon = Weapons[WeaponIndex];
	// Replace CurrentWeapon object
	
	// CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	// Find by index and Set anim montage for reloading (Dangerous way)

	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
	{return Data.WeaponClass == CurrentWeapon->GetClass(); });
	// Find anim montage for reloading
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	// Set anim montage for reloading

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	// Function to eqip new weapon
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	// Create attachment rules
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
	// Attach component to skeletal mesh with rules to socket
}

void USTUWeaponComponent::StartFire() 
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
	// Calling Fire in Base Weapon
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	// Calling Fire in Base Weapon
}

void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
	// Equip weapon with new index on hands
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}
void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	// Get notify from montage <template function>
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	} // sign on notify inclass function
	else
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is not set"));
		checkNoEntry();
		// Check blueprint type chosen
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is not set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	} // sign on notify inclass function
}
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	EquipAnimInProgress = false;
	// Function work with char only
}
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;
	ReloadAnimInProgress = false;
	// Function work with char only
}

bool USTUWeaponComponent::CanFire() const 
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const 
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}
void  USTUWeaponComponent::Reload()
{
	ChangeClip();
}
void USTUWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}
void USTUWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
	// Stop timer when reload or clip empty and activate inclass function
}