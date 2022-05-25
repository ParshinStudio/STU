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

	FOnMatchStateChangedSignature OnMatchStateChanged; // Delegate

	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// Override of default pawn for controller
	
	void Killed(AController* KillerController, AController* VictimController);

	FGameData GetGameData() const { return GameData; }
	int32 GetCurrentRoundNum() const { return CurrentRound; }
	int32 GetRoundSecondsRemaining() const { return RoundCoundown; }

	void RespawnRequest(AController* Controller);

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate ) override;
	virtual bool ClearPause() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController>AIControllerClass; // Choose Controller in bp

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn>AIPawnClass; // Choose pawn for AI in bp
	
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData; // CoreTypes struct

private:

	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;

	int32 CurrentRound = 1;
	int32 RoundCoundown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeam(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);

	void LogPlayerInfo();

	void StartRespawn(AController* Controller);

	void GameOver();

	void SetMatchState(ESTUMatchState State);
};
