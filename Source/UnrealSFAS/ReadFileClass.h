// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALSFAS_API ReadFileClass
{
public:
	// Will read the maze in
	void ReadFile(std::string Name);
	// Vector of vectors to store the maze
	std::vector<std::vector<int>> MazeMap;
	const int ASCII_CONVERT_NUM = 48;
};

