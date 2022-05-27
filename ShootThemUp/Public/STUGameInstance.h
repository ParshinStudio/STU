// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FLevelData GetStartupLevel() const { return StartupLevel; }
	void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }
	// Set and get start level data

	TArray<FLevelData> GetLevelsData() const { return LevelsData; }
	// Return array with setted levels

	FName GetMenuLevelName() const { return MenuLevelName; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FLevelData> LevelsData;
	//In editor array to fill 
	
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;

private:

	FLevelData StartupLevel;
	// Current chosen level
};
