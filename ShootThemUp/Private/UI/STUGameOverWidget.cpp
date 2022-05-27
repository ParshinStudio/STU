// ShootThemUpGame


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void USTUGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetWorld())
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
			// Sign on delegate
		}
	}
	if (ResetLevelButton)
	{
		ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
	}
}

void  USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
	if (State == ESTUMatchState::GameOver)
	{
		UpdatePlayerStat();
	}
}
void USTUGameOverWidget::UpdatePlayerStat()
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren(); // Clear before adding rows

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;
		
		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;
		
		const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;
		
		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(STUUTils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(STUUTils::TextFromInt(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(STUUTils::TextFromInt(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
		PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

		PlayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

void USTUGameOverWidget::OnResetLevel()
{
	const FName CurrentLevelName = "TestLevel";
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}
