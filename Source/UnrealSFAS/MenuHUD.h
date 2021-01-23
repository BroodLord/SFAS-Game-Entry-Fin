// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSFAS_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:
	/*Shared Pointers for each Slate Widget*/
	TSharedPtr<class SMainMenuWidget> MenuWidget;
	TSharedPtr<class SScoreMenuWidget> ScoreWidget;
	TSharedPtr<class SPerkMenuWidget> PerkWidget;
	TSharedPtr<class SGamePlayWidget> GamePlayWidget;
	TSharedPtr<class SHowToPlayMenuWidget> HTPWidget;
	/**************************************************/
	/*Shared Pointer to contain the current widget*/
	TSharedPtr<class SWidget> MenuWidgetContainer;
	/*Set the Texts for Kills, Time and Score*/
	FString SetKillText();
	FString SetTimeText();
	FString SetScoreText();
	/************************************/
	/*Sets the game instance for future reference*/
	void SetInstance();
	/*Pointer to the Game Instance*/
	class UMyGameInstance* GInstance;
	/*Sets the seed*/
	void SetSeed(std::string Text);
	/*Tier Prices*/
	int Tier1Price = 500;
	int Tier2Price = 800;
	int Tier3Price = 1200;
	/****************************/
	/*Max amount of Characters*/
	const int ALLOW_CHARACTERS_NUM = 26;
	// Used for checking if the seed is valid
	std::string AllowCharacters = "abcdefghijklmnopqrstuvwxyz";
	// Used to store what notification we want
	int NotificationIndex;
	// Gets the player Gold
	int GetPlayerGold();
	float Time;
	// Sets the time
	void SetTimeVar();
	// Function to check if the seed is valid
	bool CheckSeedIsValid(std::string Seed);
	// Sets the gold
	void SetGold(int Gold);
	// Shows the main Menu
	void ShowMainMenu();
	// Shows the How to play Menu
	void ShowHowToPlayMenu();
	// Add an upgrade
	void AddUpgrade(int Index);
	// Show the perk menu
	void ShowPerkMenu();
	//Show the Game Play Menu
	void ShowGamePlayMenu();
	// Gets the player health
	int GetPlayerHealth();
	// Gets the player ammo
	int GetPlayerAmmo();
	// Removes the current Widget
	void RemoveMenu();
	// Show the score menu
	void ShowScoreMenu();
	// Load a level with a given name
	void LoadLevel(FName Name);
};
