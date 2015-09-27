
#ifndef TP_P1_project1_h
#define TP_P1_project1_h


#include <vector>
#include <deque>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>

// Class to organize tiles in map
// Class includes the tile column number, row number,
// portal destination if tile is a portal, and source
// tile that lead to current tile.
// Includes a function setValues that initializes
// all member variables.
struct mapElement {
    int roomNum;
    int rowNum;
    int colNum;
    
    // Room destination if tile is a portal
    // 100 if tile is not a portal
    int portal;
    
    // Constructors
    mapElement(): roomNum(0), rowNum(0), colNum(0), portal(0) {}
    mapElement(int x,int y,int z,int p) {
        roomNum = x;
        rowNum = y;
        colNum = z;
        portal = p;
    }
};

struct pathElement {
    char direction;
    mapElement info;

    // Constructor
    pathElement(char d) {
        direction = d;
    }
};

// Reads map input and stores information into vector
// Finds the starting tile and initializes mapElement for it
// Returns false if input is invalid. 
bool readInMap(std::vector<char> &update, int roomNum, int rowxcol,
               const char type, mapElement &start);

// Adds all map tiles as deque elements until ring is found
// Returns true when ring is found
bool addDequeElements(std::deque<mapElement> &dequeElements, std::vector<char> input,
                      std::vector<char> &path, mapElement &start, int roomNum,
                      int rowxcol, bool useStack);

// Checks desired tile in vector
// Returns 1 if tile is walkable space
// Returns 2 if tile is a portal and initializes struct portal
// Returns 3 if tile is the ring
// Returns 0 if tile is anything else
int checkTile(std::vector<char> &input, int room, int row, int col, int rowxcol);

// Pushes one mapElement onto deque based on the routing
// scheme selected. If stack, adds to the top of deque.
// If queue, adds to the back of deque.
void addElement(std::deque<mapElement> &dequeElements, mapElement &add, bool useStack);

// Changes the char stored in vector to mark it as a
// tile that has already been added
void markAdded(std::vector<char> &input, mapElement &added, int rowxcol);

pathElement tracePrevious(std::vector<char> &path, std::stack<pathElement> &trace,
                          mapElement current, int roomNum, int rowxcol);

#endif
