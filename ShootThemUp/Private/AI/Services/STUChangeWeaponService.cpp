// ShootThemUpGame


#include "AI/Services/STUChangeWeaponService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "ChangeWeapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		const auto WeaponComponent = STUUTils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
		if (WeaponComponent && FMath::RandRange(0.0f, 1.0f) <= Probability && Probability > 0)
		{
			WeaponComponent->NextWeapon();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}