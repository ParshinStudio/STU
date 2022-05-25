// ShootThemUpGame


#include "UI/STUPauseWidget.h"
#include "Components/Button.h"
#include "Gameframework/GameModeBase.h"

bool USTUPauseWidget::Initialize() 
{
	const auto InitStatus = Super::Initialize();
	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}
	return InitStatus;
}
void USTUPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();

}