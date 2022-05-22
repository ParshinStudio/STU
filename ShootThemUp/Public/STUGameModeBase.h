// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTUGameModeBase();

	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// Override of default pawn for controller
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController>AIControllerClass; // Choose Controller in bp

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData; // CoreTypes struct

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn>AIPawnClass; // Choose pawn for AI in bp

private:
	int32 CurrentRound = 1;
	int32 RoundCoundown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

};
