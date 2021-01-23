// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadFileClass.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

void ReadFileClass::ReadFile(std::string Name)
{
    // Gets the full paths
    FString FullPath = FPaths::ProjectDir();
    // This line will get the Path, add the fill we want to check and add the level name to get the path we want to check
    std::string TempPath = std::string(TCHAR_TO_UTF8(*FullPath)) + "\\Levels\\" + Name;
    std::ifstream InFile;
    int SizeX = 0;
    int SizeY = 0;
    char TempChar; // used to store the current number in the map
    InFile.open(TempPath);
    if (InFile.is_open())
    {
        // Gets the X Size diemension
        InFile >> SizeX;
        // Resize as this will be the amount of rows we will have
        MazeMap.resize(SizeX);
        // Gets the Y Size diemension
        InFile >> SizeY;
        /*Loop through and resize so the rows max the diemension*/
        for (int i = 0; i < MazeMap.size(); i++)
        {
            MazeMap[i].resize(SizeY);
        }
        // Start a loop so we can read the map in
        for (int i = 0; i < SizeY; i++)
        {
            // Create a temp row so we can add it as a whole line later
            std::vector<int> TempRow;
            // loop through each element in the row
            for (int j = 0; j < SizeX; j++)
            {
                // Read in the current element
                InFile >> TempChar;
                // Convert from Char to int 
                int Cast = (int)TempChar - ASCII_CONVERT_NUM;
                // Push it to the vector
                TempRow.push_back(Cast);
            }
            // Put that vector onto the map
            MazeMap[i] = TempRow;
        }
        // Close the file
        InFile.close();
    }
}
