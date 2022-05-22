// ShootThemUpGame


#include "AI/Tasks/STUNextLocationTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
// For task working

USTUNextLocationTask::USTUNextLocationTask()
{
	NodeName = "Next Location"; // Name in graph
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BlackBoard) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	auto Location = Pawn->GetActorLocation(); // Set center for search radius
	if (!SelfCenter) // If self center true, reset center as player actor
	{
		auto CenterActor = Cast<AActor>(BlackBoard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if (!CenterActor) return EBTNodeResult::Failed;
		Location = CenterActor->GetActorLocation();
	}

	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation); // Creating random location
	if (!Found) return EBTNodeResult::Failed;

	BlackBoard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location); // Set new location to vector
	return EBTNodeResult::Succeeded;
}
