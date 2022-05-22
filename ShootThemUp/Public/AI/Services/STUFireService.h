// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFireService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUFireService : public UBTService
{
	GENERATED_BODY()

	USTUFireService();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;
	// Add selector to service 

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
