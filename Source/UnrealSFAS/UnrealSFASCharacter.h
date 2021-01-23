// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "UnrealSFASCharacter.generated.h"

const float RELOAD_SPEED = 1.5F;

UCLASS(config=Game)
class AUnrealSFASCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void Tick(float DeltaTime) override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AUnrealSFASCharacter();


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/*Player Health*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats)
	int Health = 100;

	/*Player Current Damage*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats)
	int Damage = 10;

	/*Player Current Ammo*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats)
	int Ammo = 30;

	/*Player Max ammo*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats)
	int MaxAmmo = 30;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/*Called when the play takes damage from a source*/
	void TakeDamageAmount(int Amount);

protected:

	/*Time take between shots, */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timers)
		float TimeBetweenShots;

	/*Gun Muzzle flash */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UParticleSystem* MuzzleParticle;

	/*Gun shot sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USoundBase* GunSound;

	/*Player Hurt sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
		class USoundBase* HurtSound;

	/*Gun reload sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USoundBase* ReloadSound;

	/*Gun impact particle effect */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UParticleSystem* ImpactParticle;

	/*The static Gun attached to the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USceneComponent* Gun;

	/*The spawn location for muzzle flash and hit result*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class USceneComponent* GunBarrel;

	FTimerHandle TimerHandle_HandleFire;

	FTimerHandle TimerHandle_HandleReload;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	FVector GetMousePositionToWorld(UWorld* world);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	//void MoveSlowly();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
			const FHitResult& Hit);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
private:

	/*Fires the weapon on the player*/
	void StartFire();
	/*Used to clear the fire weapon timer*/
	void StopFire();
	/*Does all the working for what the player hits with his gun*/
	void FireShot();
	/*Reloads the gun back to the max*/
	void ReloadWeapon();
	/*Plays audio and then calls the reloadweapon function*/
	void DelayReload();
	/*Upgrades the players stats at the start of the game if active*/
	void SetUpgrades();

	/*Ref to the game instance for storing data and reterving data*/
	class UMyGameInstance* GInstance;
	/*The game music for the level*/
	UPROPERTY(EditAnywhere)
		USoundBase* GameMusic;
	/*Ref to the player controller*/
	UPROPERTY(EditAnywhere)
	APlayerController* PC;

};

