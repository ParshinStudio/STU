// ShootThemUpGame


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "STUGameInstance.h"

#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuWidget, All, All)

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}
	InitLevelItems();
}

void USTUMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);

}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation) return;
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
	// Get level name from instance
}

void USTUMenuWidget::OnQuitGame()
{
	if (!GetWorld()) return;
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Level data should not be empty"));
	// Check that game instance array of levels is not empty 

	if (!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();
	// Clear of horizontal box and existance

	for (auto LevelData : STUGameInstance->GetLevelsData()) // Return array with setted levels in editor
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		// LevelItemWidgetClass is Class chosen in menu widget to create
		if (!LevelItemWidget) continue;
		LevelItemWidget->SetLevelData(LevelData);
		// Set data to created widget
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);
		// Sign on one of widgets clicked

		LevelItemsBox->AddChild(LevelItemWidget);
		// Add widget to box
		LevelItemWidgets.Add(LevelItemWidget);
		// Fill array of widgets in menu widget
	}

	if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
		// If no startup level in instance, get data from first level in array
	}
	else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel());
		// If was chosen before, get this level. Also use to set level 
	}
}
void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance();
	if (!STUGameInstance) return;

	STUGameInstance->SetStartupLevel(Data);
	// Set startup level in instance

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
			// Make frame visible in selected level widget
		}
	}
}
USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}