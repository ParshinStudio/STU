// ShootThemUpGame

#include "STUGameModeBase.h"
#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "UI/STUGameHud.h"
#include "AIController.h"
#include "STUPlayerState.h"
#include "Components/STURespawnComponent.h"
#include "STUUtils.h"
#include "EngineUtils.h" // For gameover
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "GameFramework/Pawn.h"


DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10; // min time before round end to respawn

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
	// Replace player state and create for every controller
}

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();
	CurrentRound = 1;
	StartRound();
	SetMatchState(ESTUMatchState::InProgress);
}

void ASTUGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController); // Respawn Players from controller
	}
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
	// Set controlled pawn to controller in bp editor 
}

void ASTUGameModeBase::StartRound()
{
	RoundCoundown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}
void ASTUGameModeBase::GameTimerUpdate()
{
	UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCoundown, CurrentRound, GameData.RoundsNum);

	//const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
	//RoundCoundown -= TimerRate;

	if (--RoundCoundown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}
void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset(); // Destroy actor and clean controller (destroy pawn)
	}
	RestartPlayer(Controller); // Respawn new player in controller and place in world
	SetPlayerColor(Controller); // When respawned set skin color in base char throug inclass function
}

void ASTUGameModeBase::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get(); // Get controller from iterator
		if (!Controller) continue;

		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue; // Get State by controller

		PlayerState->SetTeamID(TeamID);  // Seter function in state
		PlayerState->SetTeamColor(DetermineColorByTeam(TeamID)); // Seter function in state
		SetPlayerColor(Controller); 

		TeamID = TeamID == 1 ? 2 : 1; // Change each other
	}
}
FLinearColor ASTUGameModeBase::DetermineColorByTeam(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}
	UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
	return GameData.DefaultTeamColor; // If no id, default paint
	// If id received check color from array of id 
}
void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;
	const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if (!Character) return;

	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor()); 
	// Call function in char
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;
	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}
	StartRespawn(VictimController);
}

void ASTUGameModeBase::LogPlayerInfo()
{
	if (!GetWorld()) return;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get(); // Get controller from iterator
		if (!Controller) continue;

		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue; // Get State by controller

		PlayerState->LogInfo();
	}
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnAvailable = RoundCoundown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!RespawnAvailable) return;
	const auto RespawnComponent = STUUTils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}
void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ASTUGameModeBase::GameOver()
{
	UE_LOG(LogSTUGameModeBase, Display, TEXT("GAME OVER"));
	LogPlayerInfo();

	for (auto Pawn : TActorRange<APawn>(GetWorld())) // Get all pawns in world
	{
		auto PawnWeapon = STUUTils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
		if (!PawnWeapon) continue;
		PawnWeapon->StopFire();
		auto PawnHealth = STUUTils::GetSTUPlayerComponent<USTUHealthComponent>(Pawn);
		if (!PawnHealth) continue;
		PawnHealth->TryToAddHealth(100);
		
		Pawn->TurnOff();
		Pawn->DisableInput(nullptr);
	}
	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
	if (MatchState == State) return;
	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetMatchState(ESTUMatchState::Pause);
	}
	return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}
	return PauseCleared;
}