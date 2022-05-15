// ShootThemUpGame

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
//Allow to work with UShpereComponent
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	// Projectile will be blocked by all colloson channels and ignore physics(work with QueryOnly)
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.4f;
	// Create and set default ProjectileMovementComponent params
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	check(CollisionComponent);
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	// Set InitialSpeed to Velocity
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	// Ignore Owner
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	// Sign to delegate, declared in h file, call OnProjectileHit() when CollisionComponent Hit other actor
	SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit
(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;
	MovementComponent->StopMovementImmediately();
	// Stop actor
	UGameplayStatics::ApplyRadialDamage
	(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), { GetOwner()}, this, nullptr, DoFullDamage);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

	Destroy();
	// In engine function
}

AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
