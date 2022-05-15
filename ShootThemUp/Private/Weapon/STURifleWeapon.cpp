// ShootThemUpGame


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"


void ASTURifleWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
	if (!GetWorld()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;
	// Return TraceStart, TraceEnd

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	// MakeHit Return HitResult

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		// Base Weapon Function
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Blue, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		// Debug lines 
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Blue, false, 3.0f, 0, 3.0f);
		// Debug lines 
	}
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	// Get Forward Vector from rotation and set spread of shooting
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	// Set difference between points
	return true;
	// Set TraceStart and TraceEnd new params and return true
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	// Check actor collision with trace and make damage to DamagedActor if line touched DamagedActor
}