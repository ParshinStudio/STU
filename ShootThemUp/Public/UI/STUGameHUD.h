// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUUtils.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverHUDWidgetClass;

	virtual void BeginPlay() override;
	
public:
	virtual void DrawHUD() override;
private:
	UPROPERTY()
	TMap<ESTUMatchState, UUserWidget*> GameWidgets;
	UPROPERTY()
	UUserWidget* CurrentWidget = nullptr;


	void DrawCrossHair();
	void OnMatchStateChanged(ESTUMatchState State);
};
