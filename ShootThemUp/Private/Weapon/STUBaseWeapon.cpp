// ShootThemUpGame

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
// Allow to work with SkeletalMesh
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
// Allow GetWorld()
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
// Allow to get character and controller

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	// Create component and set as root
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullents cant be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips cant be less or equal zero"))
	// Check that quantity in blueprint not less than 0
	CurrentAmmo = DefaultAmmo;
	// Set from blueprint to all weapons
}

void ASTUBaseWeapon::StartFire(){} // overrided functions

void ASTUBaseWeapon::StopFire(){} // overrided functions

void ASTUBaseWeapon::MakeShot(){} // overrided functions

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;
	return Player->GetController<APlayerController>();
	// Get owner of ACharacter and get his controller
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
	// Try to get view point 
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	// Get Forward Vector from rotation and set spread of shooting
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	// Set difference between points
	return true;
	// Set TraceStart and TraceEnd new params and return true
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
	//Return HitResult
}

void ASTUBaseWeapon::DecreseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}
	CurrentAmmo.Bullets--;
	LogAmmo();
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
		// Broadcast to all signed classes
	}
}
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}
bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
} 
void ASTUBaseWeapon::ChangeClip()
{
	CurrentAmmo.Bullets = DefaultAmmo.Bullets; 
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("Clips = 0"));
			return;
		}
		CurrentAmmo.Clips--;
	} // logic function to -- clips when possible
	UE_LOG(LogBaseWeapon, Display, TEXT("Reloaded"));
}
bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}
void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " /";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite: " : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
	// Logging function only
}


/* 
NOT USED CODE, NOT REFACTORED, FOR REFERENCE USE ONLY
void ASTUBaseWeapon::MakeShot()
{
	if (!GetWorld()) return;

	const auto Player = Cast<ACharacter>(GetOwner());
	//Cast to char to get char
	if (!Player) return;
	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;
	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
		
	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = ViewLocation; //SocketTransform.GetLocation();
	// Get Location of MuzzleSocketName in weapon mesh
	const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector();
	// Get Forward Vector
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	// Set difference between points

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult HitResult;
	// Declare hitresult engine object
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
	}
} 
*/