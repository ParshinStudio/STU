// ShootThemUpGame


#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
// Add Camera Component
#include "Components/InputComponent.h"
// Add Input Component
#include "GameFramework/SpringArmComponent.h"
// Add SpringArmComponent Component
#include "Components/STUWeaponComponent.h"
// Add WeaponComponent
#include "Components/SphereComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
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
	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	// Create Camera sphere collision
}

void ASTUPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);
	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
	const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(HideMesh);
	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);
	for (auto MeshChild : MeshChildren)
	{
		const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
		if (MeshChildGeometry)
		{
			MeshChildGeometry->SetOwnerNoSee(HideMesh);
		}
	}

}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(WeaponComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::TurnAround);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
	// WeaponComponent, &USTUWeaponComponent::Fire is direct function calling
	// Jump is engine function
	// Bind Axis/Action in InputComponent to functions and pawn
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	AddMovementInput(GetActorForwardVector(), Amount);
	// Get Vector and multiply this vector by Amount
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
	// Get Vector and multiply this vector by Amount
}

void ASTUPlayerCharacter::LookUp(float Amount)// Can be binded directly because its receive one parameter
{
	AddControllerPitchInput(Amount);
	// Vertical Camera Rotation (In blueprint enable "Use Pawn Control Rotation" if use without SpringArm)
}

void ASTUPlayerCharacter::TurnAround(float Amount) // Can be binded directly because its receive one parameter
{
	AddControllerYawInput(Amount);
	// Horizontal Camera Rotation 
}

void ASTUPlayerCharacter::OnStartRunning() // Control WantsToRun state
{
	WantsToRun = true;
}

void ASTUPlayerCharacter::OnStopRunning() // Control WantsToRun state
{
	WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
	// Return true if bools are true
}

void ASTUPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}