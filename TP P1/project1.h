
#ifndef TP_P1_project1_h
#define TP_P1_project1_h


#include <vector>
#include <deque>
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

// Adds all map tiles as deque elements until ring is found
// Returns true when ring is found
bool addDequeElements(std::deque<pathElement> &dequeElements, std::vector<char> &input,
                      pathElement &start, int roomNum, int rowxcol, bool useStack);

// Checks desired tile in vector
// Returns 1 if tile is walkable space or a portal
// Returns 3 if tile is the ring
// Returns 0 if tile is anything else
int checkTile(std::vector<char> &input, int room, int row, int col, int rowxcol);

// Pushes one pathElement onto deque based on the routing
// scheme selected. If stack, adds to the top of deque.
// If queue, adds to the back of deque.
void addElement(std::deque<pathElement> &dequeElements, pathElement &add, bool useStack);

// Changes the char stored in vector to mark it as a
// tile that has already been added
void markAdded(std::vector<char> &input, pathElement &added, int rowxcol);

#endif
