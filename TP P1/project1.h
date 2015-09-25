
#ifndef TP_P1_project1_h
#define TP_P1_project1_h


#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>


// Reads map input and stores information into vector
// Returns false if input is invalid. 
bool readInMap(std::vector<char> &update, int roomNum, int rowxcol, const char type);

// Locates starting position of route based on vector
void locateStart(std::vector<char> &map, int roomNum, int rowxcol);

#endif
