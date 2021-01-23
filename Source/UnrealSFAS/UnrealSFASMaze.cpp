// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSFASMaze.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "UnrealSFASToblerone.h"
#include "MenuHUD.h"
#include "NavigationSystem.h"
#include "ReadFileClass.h"
#include "UnrealSFASRecursiveDivision.h"

// Sets default values
AUnrealSFASMaze::AUnrealSFASMaze()
{
	PrimaryActorTick.bCanEverTick = false;
	EndPointActor = CreateDefaultSubobject<AActor>(TEXT("EndPointActor"));
	MazeAlgorithem = new UnrealSFASRecursiveDivision();
}

// Called when the game starts or when spawned
// Called when the game starts or when spawned
void AUnrealSFASMaze::BeginPlay()
{
	Super::BeginPlay();
	/* Gets the current GameInstance */
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (WallMesh)
	{
		/*Default settings for the maze*/
		const int32 mazeSizeX = 22;
		const int32 mazeSizeY = 22;
		const float blockSize = 400.0f;
		const float blockWidth = 4.0f;
		const float blockHeight = 4.0f;
		const float blockZPos = 50.0f;
		/*******************************/

		// Create a empty maze with the given sizes
		uint32 FinishedMaze[mazeSizeY][mazeSizeX];

		/*Create a pointer to the file reader and see if the seed text is a level name*/
		ReadFileClass* Read = new ReadFileClass();
		Read->ReadFile(GameInstance->GetSeed());
		/******************************************************************************/
		bool HasPlayerSpawn = false;
		bool HasExitSpawn = false;
		bool HasAISpawner = false;
		/*Loop through the map*/
		for (int i = 0; i < Read->MazeMap.size(); i++)
		{
			for (int j = 0; j < Read->MazeMap[i].size(); j++)
			{
				/*Check to see if the maze contains the following, if so it is valid*/
				FinishedMaze[i][j] = Read->MazeMap[i][j];
				if (FinishedMaze[i][j] == 2)
				{
					HasPlayerSpawn = true;
				}
				else if (FinishedMaze[i][j] == 3)
				{
					HasExitSpawn = true;
				}
				else if (FinishedMaze[i][j] == 4)
				{
					HasAISpawner = true;
				}
				/****************************************/
				// If we have them all then break as we don't need to be in here anymore
				if (HasPlayerSpawn && HasExitSpawn && HasAISpawner) { break; }
			}
		}
		/*If the level above wasn't valid then try and generate a maze from it instead*/
		if (!HasPlayerSpawn || !HasExitSpawn || !HasAISpawner)
		{
			int SeedNumber = 0;
			/*Get the seed if the seed size is greater then 4 then just generate a maze with the ID of 4 (aaaa) */
			/*Loop through the string and break it down into letters and - 96 to get the Ascii value*/
			/*If its the first number then set that as the number, else times the current seed with the new number*/
			std::string SeedString = GameInstance->GetSeed();
			if (SeedString.size() < MAX_LEVELID_LEN + 1)
			{
				for (auto i = 0; i < MAX_LEVELID_LEN; i++)
				{
					char Char = (char)SeedString[i];
					int CharNum = (int)Char - 96;
					if (i == 0) { SeedNumber = CharNum; }
					else { SeedNumber = SeedNumber * CharNum; }
				}
			}
			/*******************************************************************************/
			else { SeedNumber = MAX_LEVELID_LEN; }
			MazeAlgorithem->MazeMain(FinishedMaze, SeedNumber);
		}

		float XPos = 0.0f;
		float YPos = 0.0f;
		FQuat WorldRotation(FVector(0.0f, 0.0f, 1.0f), 0.0f);
		FVector WorldScale(blockWidth, blockWidth, blockHeight);
		USceneComponent* RootComp = GetRootComponent();

		int TorchCounter = 5;
		for (int32 i = 0; i < mazeSizeY; i++)
		{
			YPos = static_cast<float>(i - (mazeSizeY / 2)) * blockSize;

			for (int32 j = 0; j < mazeSizeX; j++)
			{
				XPos = static_cast<float>(j - (mazeSizeX / 2)) * blockSize;

				/*If its 1 then its a wall and we want to spawn it in and attach it to the root as well as set its transform*/
				if (FinishedMaze[i][j] == 1)
				{
					UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this);
					FVector WorldPosition(XPos, YPos, blockZPos);
					FTransform WorldXForm(WorldRotation, WorldPosition, WorldScale);

					meshComponent->SetStaticMesh(WallMesh);
					meshComponent->SetWorldTransform(WorldXForm);
					meshComponent->AttachToComponent(RootComp, FAttachmentTransformRules::KeepWorldTransform);
					meshComponent->RegisterComponent();
				}
				/******************************************************************************************************/
				// If its not a wall then its any of the other slots
				else
				{
					/*Floor setup*/
					/*Because the player can make a maze of any size the floor needs to be dynamic so this will do the same as the block and will spawn them in*/
					/*The only change we make to the spawning is to lower the floor down so that player stands on it*/
					UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(this);
					FVector Position(XPos, YPos, blockZPos - FLOOR_ADJUSTMENT);
					FTransform WorldXForm(WorldRotation, Position, WorldScale);

					meshComponent->SetStaticMesh(FloorMesh);
					meshComponent->SetWorldTransform(WorldXForm);
					meshComponent->AttachToComponent(RootComp, FAttachmentTransformRules::KeepWorldTransform);
					meshComponent->RegisterComponent();
					/***********************************************************************************************/
					/*Player setup*/
					/*This gets the player and moves them to to location of the player start*/
					if (FinishedMaze[i][j] == 2)
					{
						if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
						{
							APawn* PlayerCharacter;
							PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
							FVector WorldPosition(XPos, YPos, blockZPos);
							PlayerCharacter->SetActorLocation(WorldPosition);
						}
					}
					/***********************************************************************************************/
					/*End point setup*/
					/*This will get the poisition and set the endpoints pos*/
					else if (FinishedMaze[i][j] == 3)
					{
						if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
						{
							FVector WorldPosition(XPos, YPos, blockZPos);
							EndPointActor->SetActorLocation(WorldPosition);
						}

					}
					/***********************************************************************************************/
					/*AI Spawner setup*/
					/*This gets the position and and spawns a AI spawner actor there*/
					
					else if (FinishedMaze[i][j] == 4)
					{
						if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
						{
							FVector WorldPosition(XPos, YPos, blockZPos);
							AActor* Spawner = GetWorld()->SpawnActor<AActor>(AISpawner, WorldPosition, { 0,0,0 });
						}
					}
					/***********************************************************************************************/
					/*Torch setup*/
					else if (FinishedMaze[i][j] == 6)
					{
						/*This Gets the position and Moves them on the Y  & X so they connect with the walls,
						  this also figures out the rotation and either moves them One Tick (90 Degress) or Two Tick (180 Degress)
						  After that is figured out it spawns them in that position*/
						if (TorchCounter == 5)
						{
							FVector WorldPosition(XPos, YPos, blockZPos);
							FRotator Rot = { 0,0,0 };
							if (FinishedMaze[i + 1][j] == 1)
							{
								Rot = { 0,TURN_TWO_TICK,0 };
								WorldPosition.Y += TORCH_ADJUSTMENT;
							}
							else if (FinishedMaze[i - 1][j] == 1)
							{
								WorldPosition.Y -= TORCH_ADJUSTMENT;
							}
							else if (FinishedMaze[i][j + 1] == 1)
							{
								Rot = { 0,TURN_ONE_TICK,0 };
								WorldPosition.X += TORCH_ADJUSTMENT;
							}
							else if (FinishedMaze[i][j - 1] == 1)
							{
								Rot = { 0,-TURN_ONE_TICK,0 };
								WorldPosition.X -= TORCH_ADJUSTMENT;
							}
							AActor* ThisActor = GetWorld()->SpawnActor<AActor>(Torch, WorldPosition, Rot);
							TorchCounter = 0;
						}
						else
						{
							TorchCounter++;
						}
						/***********************************************************************************************/
					}
					/*Statue setup*/
					/*This Gets the position and Moves them on the Y & X so they connect with the walls,
					  this also figures out the rotation and either moves them One Tick (90 Degress) or Two Tick (180 Degress)
					  After that is figured out it spawns them in that position*/
					else if (FinishedMaze[i][j] == 7)
					{
						FVector WorldPosition(XPos, YPos, blockZPos);
						FRotator Rot = { 0,0,0 };
						if (FinishedMaze[i + 1][j] == 1)
						{
							Rot = { 0,TURN_TWO_TICK,0 };
							WorldPosition.Y += STATUE_ADJUSTMENT;
						}
						else if (FinishedMaze[i - 1][j] == 1)
						{
							WorldPosition.Y -= STATUE_ADJUSTMENT;
						}
						else if (FinishedMaze[i][j + 1] == 1)
						{
							Rot = { 0,TURN_ONE_TICK,0 };
							WorldPosition.X += STATUE_ADJUSTMENT;
						}
						else if (FinishedMaze[i][j - 1] == 1)
						{
							Rot = { 0,-TURN_ONE_TICK,0 };
							WorldPosition.X -= STATUE_ADJUSTMENT;
						}
						AActor* ThisActor = GetWorld()->SpawnActor<AActor>(Statue, WorldPosition, Rot);
					}
					/***********************************************************************************************/
					/*Central Props setup*/
					/*This will generate a random number on to spawn a prop or not if it does spawn one then we create another random number and use that as an index
					  We then adjust the Y and Z and spawn the actor in*/
					else if (FinishedMaze[i][j] == 8)
					{
						int SpawnChance = rand() % 100;
						if (SpawnChance < 40)
						{
							int ItemRand = rand() % PropArray.Num();
							FVector WorldPosition(XPos, YPos, blockZPos);
							FRotator Rot = { 0,180,0 };
							WorldPosition.Y += STATUE_ADJUSTMENT;
							WorldPosition.Z -= Z_ADJUSTMENT;
							AActor* ThisActor = GetWorld()->SpawnActor<AActor>(PropArray[ItemRand], WorldPosition, Rot);
						}
					}
					/***********************************************************************************************/

				}
			}
		}

	}
}

