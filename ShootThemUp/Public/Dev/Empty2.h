// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Empty2.generated.h"

UCLASS()
class SHOOTTHEMUP_API AEmpty2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmpty2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
