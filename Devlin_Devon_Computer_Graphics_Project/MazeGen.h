#pragma once
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

bool randomMove(bool first_move);
bool validInteger(char* cstr);
void generateMaze();
void initializeMaze();
void saveMaze();
void randomPoint(bool part);
int MazeGen(int width, int height);