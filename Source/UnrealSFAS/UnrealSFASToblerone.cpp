// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSFASToblerone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUnrealSFASToblerone::AUnrealSFASToblerone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*Sets up mesh component with collisions and notifies when hit*/
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = MeshComponent;
	/*Init values for Quat values */
	PitchValue = 0.0f;
	YawValue = 0.0f;
	RollValue = 0.0f;
}

// Called when the game starts or when spawned
void AUnrealSFASToblerone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnrealSFASToblerone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat QuatRotation = FQuat(FRotator(PitchValue, YawValue, RollValue));

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

