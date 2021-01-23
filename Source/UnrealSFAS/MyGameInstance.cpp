// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "MenuHUD.h"

// This will Set the Seed
void UMyGameInstance::SetSeed(std::string NewSeed)
{
	Seed = NewSeed;
}

// This will Get the seed and return it
std::string UMyGameInstance::GetSeed()
{
	std::string TempString;
	TempString = Seed;
	return TempString;
}
/*This will reset the perks*/
void UMyGameInstance::ResetPerks()
{
	/*Perks are reset to 0,0,0 because perks are sloted with ints, 0 = no perk, any other number is the level of perk it is*/
	PerkArray = { 0,0,0 };
	DamageUpgradeCounter = 0;
	AmmoUpgradeCounter = 0;
	SpeedUpgradeCounter = 0;
}
// Gets the amount of kills
int UMyGameInstance::GetKills()
{
	return Data.PlayerKills;
}
// Add a kill to the player stats
void UMyGameInstance::AddKill()
{
	Data.PlayerKills += 1;
}
// Get the timer
float UMyGameInstance::GetTime()
{
	return Data.Time * 100;
}
// Set the timer, only called once the level is finished
void UMyGameInstance::SetTime(float FinishedTime)
{
	Data.Time = FinishedTime;
}
// Used to calculate the score
void UMyGameInstance::CalculateScore()
{
	// If the player finished the level before the first time marker
	if (Data.Time <= TIME_MARKER_ONE)
	{
		// Calulates the score = kills * 3;
		int IntScore = Data.PlayerKills * SCORE_MODIFER;
		Data.Score = (float)IntScore * 3;
	}
	// If the player finished the level before the second time marker
	else if (Data.Time <= TIME_MARKER_TWO)
	{
		// Calulates the score = kills * 2;
		int IntScore = Data.PlayerKills * SCORE_MODIFER;
		Data.Score = (float)Data.PlayerKills * 2;
	}
	// If the player finished the level before the thrid time marker
	else if (Data.Time <= TIME_MARKER_THREE)
	{
		// Calulates the score = kills;
		int IntScore = Data.PlayerKills * SCORE_MODIFER;
		Data.Score = (float)Data.PlayerKills;
	}
	/*Give the player gold equal to the kills times 5*/
	PlayerGold += (int)Data.Score * GOLD_MODIFER;
	/*Pass the data to the saved data*/
	PassedData = Data;
	/*Reset for the next level*/
	Data.PlayerKills = 0;
	Data.Time = 0.f;
}

/*Gets the score*/
float UMyGameInstance::GetScore()
{
	return Data.Score;
}
