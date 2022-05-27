// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "UI/STUBaseWidget.h"
#include "STUMenuWidget.generated.h"


class UButton;
class UHorizontalBox;
class USTULevelItemWidget;
class USTUGameInstance;


UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;
	// Declaration of box to add widgets

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;
	// Widget to create (choose in editor)

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;

private:
	UFUNCTION()
	void OnStartGame();
	UFUNCTION()
	void OnQuitGame();

	TArray<USTULevelItemWidget*> LevelItemWidgets;
	// Array of created widgets

	void InitLevelItems();
	void OnLevelSelected(const FLevelData & Data);
	// Call when button clicked
	USTUGameInstance* GetSTUGameInstance() const;
	// Return game instance
};