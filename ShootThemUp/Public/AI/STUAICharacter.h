// ShootThemUpGame

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;

class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

public:
	ASTUAICharacter(const FObjectInitializer& ObjInit);
	// Custom constructor with params
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnDeath() override;
	virtual void BeginPlay() override;
	void OnHealthChanged(float Health, float HealthDelta) override;
	// Health Change Function
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	float HealthVisibilityDistance = 2000.0f;

private:
	void UpdateHealthWidgetVisibility();
};
