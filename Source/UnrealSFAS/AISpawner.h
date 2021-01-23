// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "AISpawner.generated.h"

UCLASS()
class UNREALSFAS_API AAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

	// The AI Spawn Point
	UPROPERTY(EditAnywhere, Category = AI)
		class USceneComponent* AISpawnPoint;

	// The Mesh of the spawner
	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* Mesh;

	// Reference to the AI we want to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> AIClass;

	// Used to spawn the AI
	void SpawningFunction();

	// Timer for the spawner
	FTimerHandle MemberTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
