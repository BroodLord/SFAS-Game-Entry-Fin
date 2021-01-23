// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/*Used to store player data*/
struct PlayerData
{
	int PlayerKills;
	float Time;
	float Score;
};

UCLASS()
class UNREALSFAS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	/*Perk Array*/
	TArray<int> PerkArray = {0,0,0};
	/*Counters for the upgrades and upgrade text, They are here because they need to persist between levels*/
	int DamageUpgradeCounter = 0;
	int AmmoUpgradeCounter = 0;
	int SpeedUpgradeCounter = 0;
	/******************************************************************************************************/
	int PlayerGold = 0; // Player Gold
	// Used to collect the player data
	PlayerData Data;
	// Used to save the player data for the score menu
	PlayerData PassedData;
	/*Getter and Setter for the seed*/
	void SetSeed(std::string NewSeed);
	std::string GetSeed();
	/*********************************/
	/*Getter and add a kill*/
	int GetKills();
	void AddKill();
	/************************/
	/*Gets the score and Calculate Score*/
	float GetScore();
	void CalculateScore();
	/***********************************/
	/*Get and Set Timer*/
	float GetTime();
	void SetTime(float FinishedTime);
	/*******************/
	// Used to reset the perk
	void ResetPerks();
private:
	// String to store the seed, passed to the maze generator later
	std::string Seed;
	/*Time Markers*/
	const int TIME_MARKER_ONE = 180;
	const int TIME_MARKER_TWO = 360;
	const int TIME_MARKER_THREE = 540;
	/********************/
	/*Modifers*/
	const int SCORE_MODIFER = 3;
	const int GOLD_MODIFER = 5;
	/**********/
};
