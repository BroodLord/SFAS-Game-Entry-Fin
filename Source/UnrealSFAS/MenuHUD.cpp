// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "SMainMenuWidget.h"
#include "SGamePlayWidget.h"
#include "SScoreMenuWidget.h"
#include "SPerkMenuWidget.h"
#include "SHowToPlayMenuWidget.h"
#include "MyGameInstance.h"
#include "Widgets/SWeakWidget.h"
#include "UnrealSFASCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"

void AMenuHUD::BeginPlay()
{
	/*Depending on what level we are on we want to show a certain menu*/
	Super::BeginPlay();
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MainMenu")
	{
		ShowMainMenu();
	}
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "ScoreMenu")
	{
		ShowScoreMenu();
	}
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MazeBase")
	{
		ShowGamePlayMenu();
	}
	else
	{
		RemoveMenu();
	}
	/*************************************************************************/
}
// Sets the time variable to the game time variable
void AMenuHUD::SetTimeVar()
{
	Time = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}
// This will set the player gold to the passed gold value
void AMenuHUD::SetGold(int Gold)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	GameInstance->PlayerGold = Gold;
}

// Gets the current gold value
int AMenuHUD::GetPlayerGold()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	return GameInstance->PlayerGold;
}
// This will Set the kill text for the score menu
FString AMenuHUD::SetKillText()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString String = FString::FromInt(GameInstance->PassedData.PlayerKills);
	return String;
}
// This will Set the Time text for the score menu
FString AMenuHUD::SetTimeText()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString String = FString::SanitizeFloat(GameInstance->PassedData.Score);
	return String;
}
// This will Set the Score text for the score menu
FString AMenuHUD::SetScoreText()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString String = FString::SanitizeFloat(GameInstance->PassedData.Time);
	return String;
}
// Sets the seed text
void AMenuHUD::SetSeed(std::string Text)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	GameInstance->SetSeed(Text);
}

// Gets the current health
int AMenuHUD::GetPlayerHealth()
{
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AUnrealSFASCharacter* Player = Cast<AUnrealSFASCharacter>(myCharacter);
	return Player->Health;
}
int AMenuHUD::GetPlayerAmmo()
{
	// Gets the current ammo of the player
	ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AUnrealSFASCharacter* Player = Cast<AUnrealSFASCharacter>(myCharacter);
	return Player->Ammo;
}
// Adds an upgrade
void AMenuHUD::AddUpgrade(int Index)
{
	UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
	GInstance->PerkArray[Index]++;
}
// Sets the instance
void AMenuHUD::SetInstance()
{
	UGameInstance* TempInstance = UGameplayStatics::GetGameInstance(GetWorld());
	GInstance = Cast<UMyGameInstance>(TempInstance);
}
// This will show the main menu widget
void AMenuHUD::ShowMainMenu()
{
	// Reset the Notification Index
	NotificationIndex = 0;
	if (GEngine && GEngine->GameViewport) // Check the Engine
	{
		/*Set the menu widget to an Main Menu Widget*/
		MenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		/*Add the widget to the Container*/
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));

		/*Disable the player input and cursor*/
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}
// This is the same as ShowMainMenu() but with no Notification reset and a change in what widget is beign set, see ShowMainMenu() for notes
void AMenuHUD::ShowGamePlayMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		GamePlayWidget = SNew(SGamePlayWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(GamePlayWidget.ToSharedRef()));

		if (PlayerOwner && UGameplayStatics::GetCurrentLevelName(GetWorld()) != "MazeBase")
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}
// This is the same as ShowMainMenu() but with no Notification reset and a change in what widget is beign set, see ShowMainMenu() for notes
void AMenuHUD::ShowScoreMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		ScoreWidget = SNew(SScoreMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(ScoreWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}
// This is the same as ShowMainMenu() but with a change in what widget is beign set, see ShowMainMenu() for notes
void AMenuHUD::ShowPerkMenu()
{
	NotificationIndex = 0;
	if (GEngine && GEngine->GameViewport)
	{
		PerkWidget = SNew(SPerkMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(PerkWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

// This is the same as ShowMainMenu() but with no Notification reset and a change in what widget is beign set, see ShowMainMenu() for notes
void AMenuHUD::ShowHowToPlayMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		HTPWidget = SNew(SHowToPlayMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(HTPWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}
// Checks to see if the seed is valid
bool AMenuHUD::CheckSeedIsValid(std::string Seed)
{
	bool NotValid = false;
	char SeedLetter;
	// Loop through the different chars in the string
	for (int i = 0; i < 4; i++)
	{
		// Get the current char
		SeedLetter = (char)Seed[i];
		// Loops through all the letters of the alphabet
		for (int j = 0; j < 26; j++)
		{
			// If the char is valid then break out
			if (SeedLetter == (char)AllowCharacters[j])
			{
				// if we have found one then set it to false and break
				NotValid = false;
				break;
			}
			else
			{
				// else set to true
				NotValid = true;
			}
		}
		// if its not valid is still true at the end of it then set the notification index and return false
		if (NotValid)
		{
			NotificationIndex = 3;
			return false;
		}
	}
	return true;
}

// This is used to remove the menu
void AMenuHUD::RemoveMenu()
{
	// Check the engine and the viewport
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		// Remove the viewpoer
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			// Set the player inputs back
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}
// Load a level with a given name
void AMenuHUD::LoadLevel(FName Name)
{
	UGameplayStatics::OpenLevel(GetWorld(), Name);
}