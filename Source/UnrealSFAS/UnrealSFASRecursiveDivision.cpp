// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealSFASRecursiveDivision.h"

static int RecurrisionCount;
static int SRAND_NUM = 0;

UnrealSFASRecursiveDivision::UnrealSFASRecursiveDivision()
{
	FunctionEnd = false;
	RecurrisionCount = 0;
	for (auto  i = 0; i < DEFAULT_MAZE_SIZE; i++)
	{
		for (auto j = 0; j < DEFAULT_MAZE_SIZE; j++)
		{
			// The Max Size - 1 && 0 will always be a wall as the maze is covered in walls on each side
			if (i == 0 || i == DEFAULT_MAZE_SIZE - 1 || j == 0 || j == DEFAULT_MAZE_SIZE - 1){ MazeArray[j][i] = 1; }
			// Set the reset to 0, gives us an empty maze
			else{ MazeArray[j][i] = 0; }
		}
	}
}

UnrealSFASRecursiveDivision::~UnrealSFASRecursiveDivision()
{
}
//Checks all directions in a NESW pattern, including NE, SE, SW, NW
bool UnrealSFASRecursiveDivision::CheckAllDirections(int v1, int v2, int Index, int Dist)
{
	if (MazeArray[v1 + Dist][v2] == Index) { return false; }
	else if (MazeArray[v1][v2 + Dist] == Index) { return false; }
	else if (MazeArray[v1 + Dist][v2 + Dist] == Index) { return false; }
	else if (MazeArray[v1 - Dist][v2 + Dist] == Index) { return false; }
	else if (MazeArray[v1 - Dist][v2] == Index) { return false; }
	else if (MazeArray[v1][v2 - Dist] == Index) { return false; }
	else if (MazeArray[v1 - Dist][v2 - Dist] == Index) { return false; }
	else if (MazeArray[v1 + Dist][v2 - Dist] == Index) { return false; }
	return true;
}

/* We want to check the XY because we don't want the walls to spawn to close together creating a "double wall" */
bool UnrealSFASRecursiveDivision::XYisValid( int& v1, int& v2)
{
	// Checks to see if are on a wall
	if (MazeArray[v1][v2] == 1) { return false; }

	//Check to see if we have already been here before
	std::vector<XY>::iterator it;
	for (it = AddedAreas.begin(); it != AddedAreas.end(); ++it)
	{
		if (v1 == it->X || v2 == it->Y)
		{
			return false;
		}
	}
	// Check that all Directions from this point in a 2 block distance aren't walls
		if (!CheckAllDirections(v1, v2, 1, 1))
		{
			return false;
		}
	return true;
}

// This will generate a random XY so we can draw a new line of walls there, but we need to make sure its valid
bool UnrealSFASRecursiveDivision::GenerateRandomXY( int& v1, int& v2)
{
	v1 = rand() % 20 + 1;
	v2 = rand() % 20 + 1;
	// If valid then return and we can just put a line of walls there
	if (XYisValid(v1, v2))
	{
		// reset
		RecurrisionCount = 0;
		return true;
	}
	else
	{
		// Because we don't want to waste full attempts on a maze by quiting and generating a new number we enter a recurrision loop
		++RecurrisionCount;
		// After testing 400 was a number that generated most new possible XYs
		if (RecurrisionCount < 400)
		{
			GenerateRandomXY(v1, v2);
		}
		else
		{
			return false;
		}
		return false;
	}
}

void UnrealSFASRecursiveDivision::GenerateMazeLineVer( int GapX, int GapY, XY& TopL, XY& TopR)
{
	bool LineCreation = false; // Condition for the while
	bool FailedLineXCreation = false;
	bool FailedLineYCreation = false;
	int Counter = 1; // Counter starts at 1 so we don't keep resetting walls
	while (!LineCreation)
	{
		if (!FailedLineXCreation)
		{
			// Loop through all the numbers that are above the current on the X and set to 1 if its not a wall
			if (GapX + Counter < DEFAULT_MAZE_SIZE)
			{
				if (MazeArray[GapX + Counter][GapY] != 1)
				{
					MazeArray[GapX + Counter][GapY] = 1;
				}
				else
				{
					// We have found where our walls ends, this will be a pre placed wall by the algorithem
					// Take the X back 1 has the current X is a wall
					TopR.X = (GapX + Counter) - 1;
					TopR.Y = GapY;
					FailedLineXCreation = true;
				}
			}
			else
			{
				// We have found where our walls ends, this will be the outter walls
				// Take the X back 1 has the current X is a wall
				TopR.X = (GapX + Counter) - 1;
				TopR.Y = GapY;
				FailedLineXCreation = true;
			}
		}
		/*This isn't any different then above, just moves in the oppersite way*/
		/* Take Function goes "------->" while this one goes "<---------"*/
		if (!FailedLineYCreation)
		{
			if (GapX - Counter > 0)
			{
				if (MazeArray[GapX - Counter][GapY] != 1)
				{
					MazeArray[GapX - Counter][GapY] = 1;
				}
				else
				{
					TopL.X = (GapX - Counter) + 1;
					TopL.Y = GapY;
					FailedLineYCreation = true;
				}
			}
			else
			{
				TopL.X = (GapX - Counter) + 1;
				TopL.Y = GapY;
				FailedLineYCreation = true;
			}
		}
		if (FailedLineYCreation && FailedLineYCreation)
		{
			LineCreation = true;
		}
		Counter++;
	}
}

/* The only changes in this function is the direction, above was in the X and this is in the Y so see above for comments */
void UnrealSFASRecursiveDivision::GenerateMazeLineHoz( int GapX, int GapY, XY& BotR, XY& TopR)
{
	bool LineCreation = false;
	int Counter = 1;
	bool FailedX = false;
	bool FailedY = false;
	while (!LineCreation)
	{
		if (!FailedX)
		{
			if (MazeArray[GapX][GapY + Counter] != 1)
			{
				MazeArray[GapX][GapY + Counter] = 1;
			}
			else
			{
				//DistUp = Counter;
				BotR.Y = (GapY + Counter) - 1;
				BotR.X = GapX;
				FailedX = true;
			}
		}
		if (!FailedY)
		{

			if (MazeArray[GapX][GapY - Counter] != 1)
			{
				MazeArray[GapX][GapY - Counter] = 1;
			}
			else
			{
				//DistDown = Counter;
				TopR.Y = (GapY - Counter) + 1;
				TopR.X = GapX;
				FailedY = true;
			}
		}
		if (FailedX && FailedY)
		{
			LineCreation = true;
		}
		Counter++;
	}
}

void UnrealSFASRecursiveDivision::GenerateMaze( int seed)
{
	// Number of times we call this function
	int Iterations = 0;
	int DoorGapX, DoorGapY;
	// The starting XY for each area, for ref look at Const for the XY values
	XY TopR(MAZE_START_XY_NUMBERS.Y, MAZE_START_XY_NUMBERS.Y), BotR(MAZE_START_XY_NUMBERS.X, MAZE_START_XY_NUMBERS.Y);
	XY TopL(MAZE_START_XY_NUMBERS.X, MAZE_START_XY_NUMBERS.Y), BotL(MAZE_START_XY_NUMBERS.X, MAZE_START_XY_NUMBERS.X);
	srand(seed); // Sets the Random generations off with the imported seed
	while (!IsFinished())
	{
		// Generate the Door placement
		if (GenerateRandomXY(DoorGapX, DoorGapY))
		{
			/* Create an XY for the door pos and add it to the list of place we have been before */
			XY Gap(DoorGapX, DoorGapY);
			AddedAreas.push_back(Gap);
			int Direction = rand() % 2; // This will either go Up (0) or Down (1), Boolean would have been perfect but unreal shouted at me so I changed it to ints
			if (Direction == 0)
			{
				// Set the door to a unqiue number in the maze
				MazeArray[DoorGapX][DoorGapY] = 9;
				// Fills the line
				GenerateMazeLineHoz(DoorGapX, DoorGapY, BotR, TopR);
				int SecondDirection = rand() % 2; // Left = 0. Right = 1
				/* In the second direction we find the next valid square to place a wall */
				//11111111111
				//12000021001
				//10000001001
				//10000000001
				//12000021001
				//11111111111
				/*Above is a simple example, the two represent where the new locations Tops and Bots are*/
				if (SecondDirection == 0)
				{
					bool NewAreaFound = false;
					int GapCounter = 0; // This will increment till a wall in found
					bool FoundTL = false;
					bool FoundBL = false;
					/* Because we are starting on the left we already have the Top R and Bot R so - 1 to get the new values */
					TopR.X = TopR.X - 1;
					BotR.X = BotR.X - 1;
					while (!NewAreaFound)
					{
						// If we haven't found the Top Left Location
						if (!FoundTL)
						{
							// Since we started on the left and only need the right we can just - to get the walls
							if (TopR.X - GapCounter > 0)
							{
								// if we find a wall then we need to stop and - 1 to get the value we want
								if (MazeArray[TopR.Y][TopR.X - GapCounter] == 1)
								{
									FoundTL = true;
									TopL.Y = TopR.Y;
									TopL.X = TopR.X - (GapCounter - 1);
								}
							}
							else
							{
								FoundTL = true;
							}
						}
						/* This isn't any different from the above */
						// We do these parts differently because the BotL and TopL could be in different locations.
						if (!FoundBL)
						{
							if (BotR.X - GapCounter > 0)
							{
								if (MazeArray[BotR.Y][BotR.X - GapCounter] == 1)
								{
									FoundBL = true;
									BotL.Y = BotR.Y;
									BotL.X = BotR.X - (GapCounter - 1);
								}
							}
							else
							{
								FoundBL = true;
							}
						}
						/*When both are found we want to leave this function*/
						if (FoundTL && FoundBL)
						{
							NewAreaFound = true;
							Direction = false;
						}
						GapCounter++;
					}
				}
				else
				{
					/*Not too different from the left side*/
					bool NewAreaFound = false;
					int GapCounter = 0;
					bool FoundTR = false;
					bool FoundBR = false;
					/*Because we are going left we now have a problem, it registers the already found values as the Top and Bot right,
					  so we have to flip these values around for readablity*/
					TopL = TopR;
					BotL = BotR;
					TopR.X = 0; TopR.Y = 0;
					BotR.X = 0; BotR.Y = 0;
					TopL.X = TopL.X + 1;
					BotL.X = BotL.X + 1;
					/************************************************/
					// Its the same as above but with a few changes 
					while (!NewAreaFound)
					{
						if (!FoundTR)
						{
							// New want to check against the max number
							if (TopL.X + GapCounter < DEFAULT_MAZE_SIZE)
							{
								// Same as before but in oppersite direction
								if (MazeArray[TopL.Y][TopL.X + GapCounter] == 1)
								{
									FoundTR = true;
									TopR.Y = TopL.Y;
									TopR.X = TopL.X + (GapCounter - 1);
								}
							}
							else
							{
								FoundTR = true;
							}
						}
						if (!FoundBR)
						{
							// New want to check against the max number
							if (BotL.X + GapCounter < DEFAULT_MAZE_SIZE)
							{
								// Same as before but in oppersite direction
								if (MazeArray[BotL.Y][BotL.X + GapCounter] == 1)
								{
									FoundBR = true;
									BotR.Y = BotL.Y;
									BotR.X = BotL.X + (GapCounter - 1);
								}
							}
							else
							{
								FoundBR = true;
							}
						}
						if (FoundTR && FoundBR)
						{
							NewAreaFound = true;
							Direction = true;
						}
						GapCounter++;
					}
				}
			}
			else
			{
				/* This is the same as the direction 0 but going oppersite direction */
				MazeArray[DoorGapX][DoorGapY] = 9;
				GenerateMazeLineVer(DoorGapX, DoorGapY, TopL, TopR);
				int SecondDirection = rand() % 2; // Down = 0. Up = 1
				if (SecondDirection == 0)
				{
					bool NewAreaFound = false;
					int GapCounter = 0;
					bool FoundTL = false;
					bool FoundTR = false;
					// Going down so we already have the top values
					TopR.Y = TopR.Y + 1;
					TopL.Y = TopL.Y + 1;
					/* Same as before just looking for the bot values */
					while (!NewAreaFound)
					{
						if (!FoundTL)
						{
							if (TopR.Y + GapCounter < DEFAULT_MAZE_SIZE)
							{
								if (MazeArray[TopR.X][TopR.Y + GapCounter] == 1)
								{
									FoundTL = true;
									BotR.X = TopR.X;
									BotR.Y = TopR.Y + (GapCounter - 1);
								}
							}
							else
							{
								FoundTL = true;
							}
						}
						if (!FoundTR)
						{
							if (TopR.Y + GapCounter < DEFAULT_MAZE_SIZE)
							{
								if (MazeArray[TopL.X][TopL.Y + GapCounter] == 1)
								{
									FoundTR = true;
									BotL.X = TopL.X;
									BotL.Y = TopL.Y + (GapCounter - 1);
								}
							}
							else
							{
								FoundTR = true;
							}
						}
						if (FoundTR && FoundTL)
						{
							NewAreaFound = true;
						}
						GapCounter++;
					}
				}
				else
				{
					bool NewAreaFound = false;
					int GapCounter = 0;
					bool FoundBL = false;
					bool FoundBR = false;
					// Going up so we need to swap the values
					BotR = TopR;
					BotL = TopL;
					TopR.X = 0; TopR.Y = 0;
					TopL.X = 0; TopL.Y = 0;
					BotR.Y = BotR.Y - 1;
					BotL.Y = BotL.Y - 1;
					/* Same as before just looking for the top values */
					while (!NewAreaFound)
					{
						if (!FoundBR)
						{
							if (BotR.Y - GapCounter > 0)
							{
								if (MazeArray[BotR.X][BotR.Y - GapCounter] == 1)
								{
									FoundBR = true;
									TopR.X = BotR.X;
									TopR.Y = BotR.Y - (GapCounter - 1);
								}
							}
							else
							{
								FoundBR = true;
							}
						}
						if (!FoundBL)
						{
							if (BotL.Y - GapCounter > 0)
							{
								if (MazeArray[BotL.X][BotL.Y - GapCounter] == 1)
								{
									FoundBL = true;
									TopL.X = BotL.X;
									TopL.Y = BotL.Y - (GapCounter - 1);
								}
							}
							else
							{
								FoundBL = true;
							}
						}
						if (FoundBR && FoundBL)
						{
							NewAreaFound = true;
						}
						GapCounter++;
					}
				}
			}
		}
		// If we have hit the max iterations we desire then we can leave this function
		if (Iterations == MAX_ITERATIONS)
		{
			SetFinished(true);
		}
		Iterations++;
	}
	return;
}

void UnrealSFASRecursiveDivision::AssignPropSlots()
{
	for (int CurrentY = 0; CurrentY < DEFAULT_MAZE_SIZE; CurrentY++)
	{
		for (int CurrentX = 0; CurrentX < DEFAULT_MAZE_SIZE; CurrentX++)
		{
			//Assign Door Statue slots
			if (MazeArray[CurrentX][CurrentY] == 9)
			{
				// Checks the NE,SE,SW,NW Areas and then checks NESW so its not blocking any corriders
				if (MazeArray[CurrentX + 1][CurrentY + 1] != 1 && MazeArray[CurrentX + 2][CurrentY - 1] != 1) { MazeArray[CurrentX + 1][CurrentY + 1] = 7; }
				if (MazeArray[CurrentX - 1][CurrentY + 1] != 1 && MazeArray[CurrentX - 2][CurrentY + 1] != 1) { MazeArray[CurrentX - 1][CurrentY + 1] = 7; }
				if (MazeArray[CurrentX + 1][CurrentY - 1] != 1 && MazeArray[CurrentX + 2][CurrentY + 1] != 1) { MazeArray[CurrentX + 1][CurrentY - 1] = 7; }
				if (MazeArray[CurrentX - 1][CurrentY - 1] != 1 && MazeArray[CurrentX - 2][CurrentY - 1] != 1) { MazeArray[CurrentX - 1][CurrentY - 1] = 7; }

			}
			//Assign Wall Mounted Torches Slots
			else if (MazeArray[CurrentX][CurrentY] == 1)
			{
				// Checks NESW and assigns any location that is empty with this slot
				if (MazeArray[CurrentX + 1][CurrentY] == 0 || MazeArray[CurrentX + 1][CurrentY] == 7 && CurrentX + 2 < DEFAULT_MAZE_SIZE) { MazeArray[CurrentX + 1][CurrentY] = 6; }
				if (MazeArray[CurrentX - 1][CurrentY] == 0 || MazeArray[CurrentX - 1][CurrentY] == 7 && CurrentY - 2 > 0) { MazeArray[CurrentX - 1][CurrentY] = 6; }
				if (MazeArray[CurrentX][CurrentY + 1] == 0 || MazeArray[CurrentX][CurrentY + 1] == 7 && CurrentY + 2 < DEFAULT_MAZE_SIZE) { MazeArray[CurrentX][CurrentY + 1] = 6; }
				if (MazeArray[CurrentX][CurrentY - 1] == 0 || MazeArray[CurrentX][CurrentY - 1] == 7 && CurrentY - 2 > 0) { MazeArray[CurrentX][CurrentY - 1] = 6; }
			}
			//Assign Room Prop Slots
			else if (MazeArray[CurrentX][CurrentY] == 0)
			{
				if (MazeArray[CurrentX + 1][CurrentY] != 1 && MazeArray[CurrentX - 1][CurrentY] != 1 &&
					MazeArray[CurrentX][CurrentY + 1] != 1 && MazeArray[CurrentX][CurrentY - 1] != 1)
				{
					if (MazeArray[CurrentX + 1][CurrentY] != 9 && MazeArray[CurrentX - 1][CurrentY] != 9 &&
						MazeArray[CurrentX][CurrentY + 1] != 9 && MazeArray[CurrentX][CurrentY - 1] != 9)
					{
							MazeArray[CurrentX][CurrentY] = 8;
					}
				}
			}
			
		}
	}
}

bool UnrealSFASRecursiveDivision::IsFinished()
{
	return FunctionEnd;
}

/*This is used for finding the locations where the player, endpoint and AI spawners can be placed*/
bool UnrealSFASRecursiveDivision::FindSpawnLocations( int CurrentX, int CurrentY)
{
	// Checks the NESW and increases the counter if it hits the walls
	// Spawner logic: if it has 3 walls and 1 enternce then its a long/short corrider with a dead end and this will fill that void
	int Counter = 0;
	if (MazeArray[CurrentX + 1][CurrentY] == 1) { Counter++; }
	if (MazeArray[CurrentX - 1][CurrentY] == 1) { Counter++; }
	if (MazeArray[CurrentX][CurrentY + 1] == 1) { Counter++; }
	if (MazeArray[CurrentX][CurrentY - 1] == 1) { Counter++; }
	if (Counter == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UnrealSFASRecursiveDivision::MazeMain(uint32 (&mazeArray)[DEFAULT_MAZE_SIZE][DEFAULT_MAZE_SIZE], int RandomNumber)
{
	std::vector<int> AlreadySpawnerIndexs; // Vector to store locations that have already been assigned
	SRAND_NUM = RandomNumber;
	GenerateMaze(SRAND_NUM); // Generate the Maze with the given seed
	int StartPosIndex; // used as a ref for later use
	int EndPosIndex; // used as a ref for later use
	std::vector<XY> PossibleSpawnLocations; // Possible locations where the player, end point and AI can spawn

	/*Finds all Areas which would classed as spawn locations*/
	for (int i = 0; i < DEFAULT_MAZE_SIZE; i++)
	{
		for (int j = 0; j < DEFAULT_MAZE_SIZE; j++)
		{
			if (MazeArray[j][i] == 0 && FindSpawnLocations(j, i))
			{
				XY TempValues(j, i);
				PossibleSpawnLocations.push_back(TempValues);
			}
		}
		std::cout << "" << std::endl;
	}
	// START AND END POINTS ASSIGNMENTS
	srand(SRAND_NUM);
	//Generates a random number and set the player spawn at that index
	StartPosIndex = rand() % PossibleSpawnLocations.size();
	MazeArray[PossibleSpawnLocations[StartPosIndex].X][PossibleSpawnLocations[StartPosIndex].Y] = 2;
	// Same as the above but check to make sure that the End point isn't the start point
	EndPosIndex = rand() % PossibleSpawnLocations.size();
	while (EndPosIndex == StartPosIndex)
	{
		EndPosIndex = rand() % PossibleSpawnLocations.size();
	}
	MazeArray[PossibleSpawnLocations[EndPosIndex].X][PossibleSpawnLocations[EndPosIndex].Y] = 3;

	// WALL MOUNTED PROPS ASSIGNMENT
	AssignPropSlots();

	//SPAWNER ASSIGNMENT
	int Counter = 0;
	int SpawnerLocations;
	while (Counter != SPAWNER_COUNT_MAX)
	{
		// Generate a random number between 0 and the size of the vector
		SpawnerLocations = rand() % PossibleSpawnLocations.size();
		// check to see if that index isn't take by the end or start
		if (SpawnerLocations != EndPosIndex && SpawnerLocations != StartPosIndex)
		{
			//loop through indexs that have already been taken and check to see if the number generated is the same
			for (int i = 0; i < AlreadySpawnerIndexs.size(); i++)
			{
				if (SpawnerLocations == AlreadySpawnerIndexs[i])
				{
					break;
				}
			}
			// Push the new index
			AlreadySpawnerIndexs.push_back(SpawnerLocations);
			// Set that index in the maze map
			MazeArray[PossibleSpawnLocations[SpawnerLocations].X][PossibleSpawnLocations[SpawnerLocations].Y] = 4;
			Counter++;
		}
	}
	// Pass all the array data to the array passed in the function call.
	for (int i = 0; i < DEFAULT_MAZE_SIZE; i++)
	{
		for (int j = 0; j < DEFAULT_MAZE_SIZE; j++)
		{
			mazeArray[j][i] = MazeArray[j][i];
		}
	}
}

void UnrealSFASRecursiveDivision::SetFinished(bool B)
{
	FunctionEnd = B;
}
