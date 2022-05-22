// ShootThemUpGame


#include "AI/Services/STUFireService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire"; // Service name
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = BlackBoard && BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (Controller)
	{
		const auto WeaponComponent = STUUTils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
		HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
