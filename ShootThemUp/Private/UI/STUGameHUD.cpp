// ShootThemUpGame


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
// For beginplay override
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverHUDWidgetClass));

	for (auto GameWidgetPair : GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if (!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
		// Add all widgets from map TMap<ESTUMatchState, UUserWidget*> GameWidgets; to viewport and hide 
	}
	if (GetWorld())
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
		} // Sign on delegate
	}
}

void ASTUGameHUD::DrawCrossHair()
{
	const TInterval<float>Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}

	UE_LOG(LogSTUGameHUD, Display, TEXT("Match State Changed: %s"), *UEnum::GetValueAsString(State));
}