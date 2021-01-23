// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>
#include <iostream>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealSFASMaze.generated.h"

class AUnrealSFASToblerone;
class UnrealSFASRecursiveDivision;

const int MAX_LEVELID_LEN = 4;
const int STATUE_ADJUSTMENT = 150;
const int TORCH_ADJUSTMENT = 200;
const int FLOOR_ADJUSTMENT = 250;
const int TURN_ONE_TICK = 90;
const int TURN_TWO_TICK = 180;
const int SPAWN_CHANCE = 40;
const int Z_ADJUSTMENT = 50;

UCLASS()
class UNREALSFAS_API AUnrealSFASMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnrealSFASMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*The Mesh for the walls*/
	UPROPERTY(EditDefaultsOnly, Category = MazeWall)
		UStaticMesh* WallMesh;
	/*The Mesh for the Floor*/
	UPROPERTY(EditDefaultsOnly, Category = MazeFloor)
		UStaticMesh* FloorMesh;
	/*This is used as a door prop that will spawn near doors if able*/
	UPROPERTY(EditDefaultsOnly, Category = MazeProp)
		TSubclassOf<AActor> Statue;
	/*This is used as a list of props that can spawn on the floor if able*/
	UPROPERTY(EditAnywhere, Category = MazeProp)
		TArray<TSubclassOf<AActor>> PropArray;
	/*This is used as a torch that can spawn on the walls if able*/
	UPROPERTY(EditAnywhere, Category = MazeProp)
		TSubclassOf<AActor> Torch;
	/*This grabs a ref to the end point object in the world*/
	UPROPERTY(EditAnywhere, Category = EndActor)
		AActor* EndPointActor;
	/*This is used to spawn AI spawners into the level at a given location*/
	UPROPERTY(EditAnywhere, Category = AI)
		TSubclassOf<class AActor> AISpawner;
	/*Used to call the maze generation*/
	UnrealSFASRecursiveDivision* MazeAlgorithem;
	/*Used to get the Hud class*/
	UPROPERTY(EditAnywhere, Category = MenuHUD)
		TSubclassOf<class AMenuHUD> AMenuHUDClass;
};
