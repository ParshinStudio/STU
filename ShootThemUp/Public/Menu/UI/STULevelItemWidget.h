// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLevelSelectedSignature OnLevelSelected;

	void SetLevelData(const FLevelData& Data);
	// Set when widget created from menu widget
	FLevelData GetLevelData() const { return LevelData; }
	// Return data about widget

	void SetSelected(bool IsSelected); 
	// Return selected or not 

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelSelectButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* FrameImage;

	virtual void NativeOnInitialized() override;

private:
	FLevelData LevelData;
	// Storage of level data in widget

	UFUNCTION()
	void OnLevelItemClicked();
	// Call when button clicked

	UFUNCTION()
	void OnLevelItemHovered();
	UFUNCTION()
	void OnLevelItemUnhovered();
	// Call when hover mouse
	
};
