// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include <random>
#include <time.h>

struct XY
{
	int X, Y;
	XY(int x, int y)
	{
		X = x; Y = y;
	}
};

/*CONST VALUES*/
const int DEFAULT_MAZE_SIZE = 22;
const int MAX_ITERATIONS = 10000;
const XY MAZE_START_XY_NUMBERS = {20,1};
const int SPAWNER_COUNT_MAX = 4;

class UNREALSFAS_API UnrealSFASRecursiveDivision
{
public:
	UnrealSFASRecursiveDivision();
	~UnrealSFASRecursiveDivision();
	std::vector<XY> AddedAreas; // Vector of X and Y that are already been placed/added to the maze
	uint32 MazeArray[DEFAULT_MAZE_SIZE][DEFAULT_MAZE_SIZE] = {}; // Array to hold all the data for the maze
	bool IsFinished(); // Used to tell the main while loop in generatemaze to stop
	void AssignPropSlots(); // Assigns prop slots in the maze
	void SetFinished(bool B);
	bool CheckAllDirections(int v1, int v2, int Index, int Dist); //Checks all directions in a NESW pattern, including NE, SE, SW, NW
	bool XYisValid(int& v1, int& v2); // Checks to see if the ints passed are valid
	bool GenerateRandomXY(int& v1, int& v2); // Generates 2 random ints for the x and y
	void GenerateMazeLineVer(int GapX, int GapY, XY& TopL, XY& TopR); // Used to fill a line in the maze and find the next editable zone
	void GenerateMazeLineHoz(int GapX, int GapY, XY& BotR, XY& TopR); // Used to fill a line in the maze and find the next editable zone
	void GenerateMaze(int seed); 
	bool FindSpawnLocations(int CurrentX, int CurrentY);
	UFUNCTION()
	void MazeMain(uint32 (&mazeArray)[DEFAULT_MAZE_SIZE][DEFAULT_MAZE_SIZE], int RandomNumber); // The function where every function is called
private:
	bool FunctionEnd;
};
