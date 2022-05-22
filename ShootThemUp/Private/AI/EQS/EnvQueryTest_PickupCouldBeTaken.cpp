// ShootThemUpGame


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "Pickups/STUBasePickup.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"


UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
	const auto DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	bool WantsBeTakable = BoolValue.GetValue();
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if (!PickupActor) continue; //

		const auto CouldBeTaken = PickupActor->CouldBeTaken();
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
		
	}
}

/* Without bool in EQS, only in actor
void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		(AActor*) ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if (!PickupActor) continue; 

		const auto CouldBeTaken = PickupActor->CouldBeTaken();
		if(CouldBeTaken)
		{
			It.SetScore(TestPurpose, FilterType, true, true);
		}
		else
		{
			It.ForceItemState(EEnvItemStatus::Failed);
		}
	}
}
*/