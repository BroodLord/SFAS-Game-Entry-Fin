// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController();

public:
	// Used to deal damage to an AI
	virtual bool TakeDamageAmount(int damage);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Call in a behavior tree to attack the player
	UFUNCTION(BlueprintCallable)
	virtual void AttackPlayer();
	// Call in a behavior tree when the AI Dies
	UFUNCTION(BlueprintCallable)
	void DestoryAI();
	// Reference to the player
	AActor* Player;
	
private:
	const int MAX_HEALTH = 60;
	const int MAX_DAMAGE = 20;

	// AI Health
	UPROPERTY(VisibleAnywhere, Category = AIProperties)
		int Health;
	// AI Damage
	UPROPERTY(VisibleAnywhere, Category = AIProperties)
		int Damage;
	// Zombie Death Noise
	UPROPERTY(EditAnywhere, Category = GamePlay)
		class USoundBase* ZombieNoise;
	// Refernce to the behavior tree
	UPROPERTY(EditAnywhere, Category = BehaviorTree)
		UBehaviorTree* EnemyBehaviorTree;
};
