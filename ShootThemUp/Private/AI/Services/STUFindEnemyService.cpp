// ShootThemUpGame


#include "AI/Services/STUFindEnemyService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy"; // Service name
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto BlackBoard = OwnerComp.GetBlackboardComponent(); 
	if (BlackBoard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = STUUTils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
		// Get perception from component
		if(PerceptionComponent)
		{
			BlackBoard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
			// Set closest actor as value in EnemyActor in blackboard service
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
