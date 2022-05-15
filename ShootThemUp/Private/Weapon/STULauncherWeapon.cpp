// ShootThemUpGame


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
// Allow calling Projectile functions from class

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	if (!GetWorld()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;
	// Return TraceStart, TraceEnd

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	// MakeHit Return HitResult

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	// Spawn Transwform for object in MuzzleSocket
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
	// Create object but delay to get params before FinishSpawning
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		// SetShotDirection in Projectile after actor created
		Projectile->SetOwner(GetOwner());
		// Set char as owner
		Projectile->FinishSpawning(SpawnTransform);
		// FinishSpawning when params set
	}
}
