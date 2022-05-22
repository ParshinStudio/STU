// ShootThemUpGame


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
// Add Camera Component
#include "Components/InputComponent.h"
// Add Input Component
#include "GameFramework/SpringArmComponent.h"
// Add SpringArmComponent Component
#include "Components/STUCharacterMovementComponent.h"
// Add overrided Movement Component header
#include "Components/STUHealthComponent.h"
// Add HealthComponent
#include "Components/STUWeaponComponent.h"
// Add WeaponComponent
#include "Components/TextRenderComponent.h"
// Add TextRenderComponent
#include "GameFramework/Controller.h"
// Add Spectator pawn control
#include "Components/CapsuleComponent.h"
// Allow to work with Capsule

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit):Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
// Replace CharacterMovementComponent in character
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	// Create SpringArmComponent and add to component
	SpringArmComponent->bUsePawnControlRotation = true;
	// All Pawn to control SpringArmComponent Rotation
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
	// Set Camera position
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	// Create Camera and add to component, attach to SpringArmComponent

	CharHealthComponent = CreateDefaultSubobject<USTUHealthComponent>(TEXT("CharacterHealthComponent"));
	// Create HealthComponent
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);
	// Create TextRenderComponent

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>(TEXT("WeaponComponent"));
	// Create WeaponComponent
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CharHealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(GetMesh());
	// Check than components exist

	OnHealthChanged(CharHealthComponent->GetHealth(), 0.0f);
	// Set Health param in text after health component created
	CharHealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	// !!! Sign from HealthComponent OnDeath delegate and send character and his function in params
	CharHealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	// !!! Sign from HealthComponent HealthChanged delegate and send character and his function in params

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
	// !!! When Landed, Delegate func calling (engine func)
}


void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(WeaponComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::TurnAround);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump); 
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
	// WeaponComponent, &USTUWeaponComponent::Fire is direct function calling
	// Jump is engine function
	// Bind Axis/Action in InputComponent to functions and pawn

}


void ASTUBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	AddMovementInput(GetActorForwardVector(), Amount);
	// Get Vector and multiply this vector by Amount
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
	// Get Vector and multiply this vector by Amount
}

void ASTUBaseCharacter::LookUp(float Amount)// Can be binded directly because its receive one parameter
{
	AddControllerPitchInput(Amount);
	// Vertical Camera Rotation (In blueprint enable "Use Pawn Control Rotation" if use without SpringArm)
}

void ASTUBaseCharacter::TurnAround(float Amount) // Can be binded directly because its receive one parameter
{
	AddControllerYawInput(Amount);
	// Horizontal Camera Rotation 
}

void ASTUBaseCharacter::OnStartRunning() // Control WantsToRun state
{
	WantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning() // Control WantsToRun state
{
	WantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
	// Return true if bools are true
}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	// Check that velocity vector in not zero
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	// Get normal of velocity vector
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	// Get Acos of scalar product (multyply) of VelocityNormal and ForwardVector 
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	// Get orthogonal vector 
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	// Get Degrees from AngleBetween Acos value
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
	// Check that cross product is not zero and Get * of AngleBetween in degress(Degrees) and Sign of CrossProduct in Z flat
}

void ASTUBaseCharacter::OnDeath() 
{
	UE_LOG(BaseCharacterLog, Warning, TEXT("PLAYER %s is dead"), *GetName()); // *Getname string->char array

	// PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Ignore collision after dead
	WeaponComponent->StopFire();
	//Stop timers when dead

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true); 
	// Enable mesh relaxation
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	// SetText of HealthTextComponent as Health variable in string
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	// Get speed of falling 
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;
	// Check that speed more than minimum value
	const  auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	// Return interpolation between first and second value
	UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
	// !!! Call damage func in engine

}

