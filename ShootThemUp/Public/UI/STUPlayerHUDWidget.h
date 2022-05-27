// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/*
class USTUWeaponComponent;
class USTUHealthComponent;
*/
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "VFX")
	void OnTakeDamage();
	// Allow call function from bp graph

protected:
	virtual void NativeOnInitialized() override;

private:
	void OnHealthChanged(float Health, float HealthDelta);
	void OnNewPawn(APawn* NewPawn);
};

/*
private:
	USTUWeaponComponent* GetWeaponComponent() const;
	USTUHealthComponent* GetHealthComponent() const;
*/
