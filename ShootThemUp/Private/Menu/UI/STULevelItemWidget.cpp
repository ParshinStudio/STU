// ShootThemUpGame


#include "Menu/UI/STULevelItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"


void USTULevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnhovered);
	} // Sign on button delegate
}

void USTULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
	
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;
	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}
	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	} // Functions to set data
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
	}
}

void USTULevelItemWidget::OnLevelItemHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	} // Functions to set data
}
void USTULevelItemWidget::OnLevelItemUnhovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	} // Functions to set data
}