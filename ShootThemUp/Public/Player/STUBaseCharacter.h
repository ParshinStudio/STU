// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"


class USTUHealthComponent;
// Declare HealthComponent
// class UTextRenderComponent;
// Declare TextRenderComponent
class USTUWeaponComponent;

class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* CharHealthComponent;
	// Declare HealthComponent
	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	// UTextRenderComponent* HealthTextComponent;
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

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color"; // Get in actor material color

	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float Health, float HealthDelta);
	// Health Change Function

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;
	// Declare statement for fast running
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetMovementDirection() const;
	// Get Movement vector
	
	virtual void OnDeath();

	void SetPlayerColor(const FLinearColor& Color);

private:

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
	// Ground landed func declared (UFUNCTION() because calling LandedDelegate in engine)
	

};
