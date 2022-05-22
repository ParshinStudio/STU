// ShootThemUpGame


#include "AI/STUAIController.h"

#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceprionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent); // Create and set PerceptionComponent
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

void ASTUAIController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn); // Call in engine function

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // Get ASTUAICharacter
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // Run BehaviorTree selected in char bp
	}
} 

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
	// Return FocusOnKeyName object
}