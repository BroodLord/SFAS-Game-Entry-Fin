// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSFASCharacter.h"
#include "UnrealSFASToblerone.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyAIController.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealSFASCharacter

AUnrealSFASCharacter::AUnrealSFASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/*Gets the SkeletalMesh*/
	USkeletalMeshComponent* MeshComponent = this->GetMesh();
	MeshComponent->SetNotifyRigidBodyCollision(true);

	/*Sets up the player gun*/
	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(MeshComponent);
	Gun->SetupAttachment(GetMesh(), ("RightHand"));

	/*Sets up the gun barrel*/
	GunBarrel = CreateDefaultSubobject<USceneComponent>(TEXT("GunBarrel"));
	GunBarrel->SetupAttachment(Gun);
	GunBarrel->SetupAttachment(GetMesh(), ("Muzzle"));

	/*Sets up the game music*/
	GameMusic = CreateDefaultSubobject<USoundBase>(TEXT("GameMusic"));

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealSFASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealSFASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealSFASCharacter::MoveRight);


	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnrealSFASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUnrealSFASCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &AUnrealSFASCharacter::DelayReload);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealSFASCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealSFASCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealSFASCharacter::OnResetVR);
}

void AUnrealSFASCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
 	if (OtherActor->GetClass()->IsChildOf(AUnrealSFASToblerone::StaticClass()))
	{
		/*If we collide with the end point actor then we want to get the game instance and let it calculate the score and then load the score menu*/
		UE_LOG(LogTemp, Warning, TEXT("COLLISION!"));
		float RealtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
		GInstance = Cast<UMyGameInstance>(TempInstance);
		GInstance->SetTime(RealtimeSeconds);
		GInstance->CalculateScore();
		UGameplayStatics::OpenLevel(GetWorld(), "ScoreMenu");
	}
}

FVector AUnrealSFASCharacter::GetMousePositionToWorld(UWorld* world)
{

	if (world)
	{
		/*This will convert the mouse from 2d space on the screen to 3d space in the world*/
		PC = world->GetFirstPlayerController();
		FVector2D MousePos = FVector2D(0, 0);
		FVector Worldpos = FVector(MousePos.X, MousePos.Y, 0);
		FVector Direction = FVector(0, 0, 0);
		PC->GetMousePosition(MousePos.X, MousePos.Y);
		PC->DeprojectMousePositionToWorld(Worldpos, Direction);
		return Direction;
	}
	return FVector();
}

// Called when the game starts or when spawned
void AUnrealSFASCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
	{
		/*If we are on the maze level then load the upgrades the player may have and play the game music*/
		SetUpgrades();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GameMusic, GetActorLocation());
	}
	OnActorHit.AddDynamic(this, &AUnrealSFASCharacter::OnHit);
}

void AUnrealSFASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*We only want to do this on MazeBase because the other levels are main menus*/
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
	{
		/*We get the world and make the mouse visable, and we set the player rot to the mouse rotation*/
		UWorld* world;
		world = GetWorld();
		PC = world->GetFirstPlayerController();
		PC->bShowMouseCursor = true;
		FVector Direction = GetMousePositionToWorld(GetWorld());
		FRotator PlayerPos = this->GetActorRotation();
		FRotator PlayerRot = FRotator(PlayerPos.Pitch, Direction.Rotation().Yaw, PlayerPos.Roll);
		this->SetActorRotation(PlayerRot);
	}
}

void AUnrealSFASCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealSFASCharacter::StartFire()
{
	/*Fires a shot and then sets a timer so the player can be automatic with the gun, instead of just single fire*/
	FireShot();

	GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &AUnrealSFASCharacter::FireShot, TimeBetweenShots, true);
}

void AUnrealSFASCharacter::StopFire()
{
	/*Clears the timer*/
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
}

void AUnrealSFASCharacter::FireShot()
{
	if (Ammo > 0 && !GetWorldTimerManager().IsTimerActive(TimerHandle_HandleReload))
	{
		/*Fire a hitresult to see if we hit anything*/
		FHitResult Hit;
		const float WeaponRange = 2000.f;
		const FVector StartTrace = this->GetActorLocation();
		const FVector EndTrace = (this->GetActorForwardVector() * WeaponRange) + StartTrace;

		FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
		/*Play a sound when firing the gun*/
		UGameplayStatics::PlaySound2D(GetWorld(), GunSound);
		/*This if statement checks to see if we hit anything*/
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			if (ImpactParticle)
			{
				/*This will spawn an emitter at the hit location and play the emitter*/
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
				AActor* HitActor = Hit.Actor.Get();
				if (HitActor->ActorHasTag(TEXT("EnemyAI")))
				{
					/*If what we hit has the "EnemyAI" tag then we want to deal damage to the AI*/
					APawn* HitPawn = Cast<APawn>(HitActor);
					//AController* temp = Cast<AController>(HitPawn->Controller);
					AEnemyAIController* TempAIController = Cast<AEnemyAIController>(HitPawn->Controller);
					/*If this comes back true then the AI is dead and we want to add a kill to the players data*/
					if (TempAIController->TakeDamageAmount(Damage))
					{
						UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
						GInstance = Cast<UMyGameInstance>(TempInstance);
						GInstance->AddKill();
					}
				}
			}
		}
		/*Play a muzzle shot when we fire a bullet*/
		if (MuzzleParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, GunBarrel->GetComponentTransform());
		}
		--Ammo;
	}
}

void AUnrealSFASCharacter::ReloadWeapon()
{
	// Resets the ammo
	Ammo = MaxAmmo;
}

// PR2 Form - 19-20 with Evidence;

void AUnrealSFASCharacter::DelayReload()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_HandleReload))
	{
		/*Plays a reload sounds (lasts for 2 seconds) and set a timer for 1.5 seconds*/
		UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
		GetWorldTimerManager().SetTimer(TimerHandle_HandleReload, this, &AUnrealSFASCharacter::ReloadWeapon, RELOAD_SPEED, false);
	}
}

void AUnrealSFASCharacter::SetUpgrades()
{
	UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
	GInstance = Cast<UMyGameInstance>(TempInstance);
	/*This will loop through the PerkArray and Assign the correct perks*/
	/*Perks are stored as ints if its 0 there is no perk and ifs a 1 then perk is at tier 1 and so on*/
	for (auto i = 0; i < GInstance->PerkArray.Max(); i++)
	{
		if (i == 0 && GInstance->PerkArray[i] != 0)
		{
			/*Increase the player damage*/
			for (int j = 0; j < GInstance->PerkArray[i]; j++)
			{
				Damage += 20;
			}
		}
		else if (i == 1 && GInstance->PerkArray[i] != 0)
		{
			/*Increases the Max ammo for the player*/
			for (int j = 0; j < GInstance->PerkArray[i]; j++)
			{
				MaxAmmo += 10;
				Ammo = MaxAmmo;
			}
		}
		else if (i == 2 && GInstance->PerkArray[i] != 0)
		{
			UCharacterMovementComponent* Movement = this->GetCharacterMovement();
			/*Increases the max walk speed*/
			for (int j = 0; j < GInstance->PerkArray[i]; j++)
			{
				
				Movement->MaxWalkSpeed += 50;
			}
		}
	}
}

void AUnrealSFASCharacter::TakeDamageAmount(int Amount)
{
		/*Decrease the players health by a given amount and plays a sounds effect to signal this*/
		Health -= Amount;
		UGameplayStatics::PlaySound2D(GetWorld(), HurtSound);
		if (Health <= 0)
		{
			/*If the players health drops below 0 then reset the perks and return to the mainmenu*/
			UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
			GInstance = Cast<UMyGameInstance>(TempInstance);
			GInstance->ResetPerks();
			UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");

		}
}

void AUnrealSFASCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealSFASCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealSFASCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealSFASCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealSFASCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUnrealSFASCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
