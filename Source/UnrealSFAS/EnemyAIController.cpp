// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "UnrealSFASCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	/*Set the Zombie Stats*/
	Health = MAX_HEALTH;
	Damage = MAX_DAMAGE;
}
// Use to deal damage to the AI
bool AEnemyAIController::TakeDamageAmount(int damage)
{
	/*Take an amount of health away from the AI*/
	Health -= damage;
	/*If health is less or equal to 0 then disable the collisions so zombies can run through and player the death noise*/
	if (Health <= 0) 
	{ 
		APawn* AIPawn = GetPawn();
		AIPawn->SetActorEnableCollision(false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ZombieNoise, AIPawn->GetActorLocation());
		return true; 
	}
	/*************************************************/
	else { return false; }
}

void AEnemyAIController::BeginPlay() 
{
	Super::BeginPlay();
	// Get Ref to the player and cast it to an actor
	APawn* myCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Player = Cast<AActor>(myCharacter);
	// Run the Behavior tree
	if (EnemyBehaviorTree != nullptr)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set the values in the blackboard
	GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), Player);
	GetBlackboardComponent()->SetValueAsInt(TEXT("Health"), Health);
}
// Function used to attack the player
void AEnemyAIController::AttackPlayer()
{
	// Cast player to the player class and take damage
	AUnrealSFASCharacter* PlayerBP =  Cast<AUnrealSFASCharacter>(Player);
	PlayerBP->TakeDamageAmount(Damage);
}
// Destory the AI
void AEnemyAIController::DestoryAI()
{
	GetPawn()->Destroy();
}
