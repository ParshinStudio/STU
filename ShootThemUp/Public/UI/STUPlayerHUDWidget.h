// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

/*
class USTUWeaponComponent;
class USTUHealthComponent;
*/

class UProgressBar;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
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
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString FormatBullets(int32 BulletsNum) const;

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor GoodColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor BadColor = FLinearColor::Red;


	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;

private:
	void OnHealthChanged(float Health, float HealthDelta);
	void OnNewPawn(APawn* NewPawn);

	void UpdateHealthBar();
};

/*
private:
	USTUWeaponComponent* GetWeaponComponent() const;
	USTUHealthComponent* GetHealthComponent() const;
*/
