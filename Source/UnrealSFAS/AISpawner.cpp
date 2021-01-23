// Fill out your copyright notice in the Description page of Project Settings.

#include "AISpawner.h"
#include "Templates/SubclassOf.h"
#include "EnemyAIController.h"

// Sets default values
AAISpawner::AAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*Set up the mesh*/
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	/*Set up the spawn location*/
	AISpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AISpawnPoint"));
	AISpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	// Set up the spawn timer with a wait time of 2.5 seconds for the first spawn
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AAISpawner::SpawningFunction, 2.5f, false);
}

void AAISpawner::SpawningFunction() 
{
	// Get the actor Location
	FVector SpawnLocation = this->GetActorLocation();
	// Adjust the spawn location
	SpawnLocation.Z += 40.0f;
	SpawnLocation.Y += 20.0f;
	// Get the rotation
	FRotator SpawnRotation = this->GetActorRotation();
	// Spawn the Actor into the world
	AActor* SpawnAI = GetWorld()->SpawnActor<AActor>(AIClass, SpawnLocation, SpawnRotation);
	// Clear the timer
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
	// Call the function again with a 5 second wait
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AAISpawner::SpawningFunction, 5.f, false);
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

