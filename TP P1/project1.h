
#ifndef TP_P1_project1_h
#define TP_P1_project1_h


#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>

// Struct to organize tiles in the map
// Struct includes tile type, column number, row number,
// and room number of the tile.
struct pathElement {
    int roomNum;
    int rowNum;
    int colNum;
    pathElement *source;
};

// Reads map input and stores information into vector
// Finds the starting tile and initializes pathElement for it
// Returns false if input is invalid. 
bool readInMap(std::vector<char> &update, int roomNum, int rowxcol,
               const char type);

// Adds map tiles as stack elements until ring is found
// Returns true when ring is found
bool addStackElements(std::stack<pathElement> &stackElements, std::vector<char> &input,
                      pathElement &start, int roomNum, int rowxcol);

// Adds map tiles as queue elements until ring is found
// Returns true when ring is found
bool addQueueElements(std::queue<pathElement> &queueElements, std::vector<char> &input,
                      pathElement &start, int roomNum, int rowxcol);

#endif
