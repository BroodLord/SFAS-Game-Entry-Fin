// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealSFASToblerone.generated.h"

UCLASS()
class UNREALSFAS_API AUnrealSFASToblerone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnrealSFASToblerone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Main Mesh of the BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* MeshComponent;
private:
	/*Pitch, Yaw and Roll values are used to make the object constantly spin*/
	UPROPERTY(EditAnywhere, Category = QuatValues)
		float PitchValue;
	UPROPERTY(EditAnywhere, Category = QuatValues)
		float YawValue;
	UPROPERTY(EditAnywhere, Category = QuatValues)
		float RollValue;
};
