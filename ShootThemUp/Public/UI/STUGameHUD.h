// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	virtual void BeginPlay() override;
	
public:
	virtual void DrawHUD() override;
private:
	void DrawCrossHair();
};
