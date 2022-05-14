// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
// Declare Camera
class USpringArmComponent;
// Declare Camera SpringArm
class USTUHealthComponent;
// Declare HealthComponent
class UTextRenderComponent;
// Declare TextRenderComponent
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	// Declare Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	// Declare Camera SpringArm

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* CharHealthComponent;
	// Declare HealthComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;
	// Declare TextRenderComponent

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;
	// Declare WeaponComponent

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;
	// Declare montage slot in editor

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);
	// Landed params to interpolate

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool IsRunning() const;
	// Declare statement for fast running
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetMovementDirection() const;
	// Get Movement vector

private:
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	// Declare movement functions
	void LookUp(float Amount);
	void TurnAround(float Amount);
	// Declare camera movement functions
	bool WantsToRun = false;
	bool IsMovingForward = false;
	void OnStartRunning();
	void OnStopRunning();
	// Declare fast running

	void OnDeath();
	// Death Function
	void OnHealthChanged(float Health);
	// Health Change Function

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
	// Ground landed func declared (UFUNCTION() because calling LandedDelegate in engine)
	

};
