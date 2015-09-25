
// Author:  Yupeng Wang
// EECS 281
// Project 1
///////////////////////////////////////////////////////////

#include <vector>
#include <deque>
#include <sstream>
#include <iostream>
#include <getopt.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>
#include "project1.h"

using namespace std;

static struct option longopts[] = {
    {"stack", no_argument, nullptr, 's'},
    {"queue", no_argument, nullptr, 'q'},
    {"output", required_argument, nullptr, 'o'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
};

void addElement(std::deque<pathElement> &dequeElements, pathElement &add, bool useStack) {
    if (useStack) {
        dequeElements.push_front(add);
    }
    else dequeElements.push_back(add);
}

int checkTile(std::vector<char> &input, int room, int row, int col, int rowxcol) {

    if (input[(room*rowxcol*rowxcol) + (row*rowxcol)
                + col] == '.' || isdigit(input[(room*rowxcol*rowxcol)
                                            + (row*rowxcol) + col])) {
        return 1;
    }
    else if (input[(room*rowxcol*rowxcol) + (row*rowxcol) + col]  == 'R') {
        return 2;
    }
    return 0;
}

void markAdded(std::vector<char> &input, pathElement &added, int rowxcol) {
    input[(added.roomNum*rowxcol*rowxcol) + (added.rowNum*rowxcol)
          + added.colNum] = '~';
}

bool readInMap(vector<char> &update, int roomNum, int rowxcol, const char type,
               pathElement &start) {
    char label;
    int i = 0;
    int col = 0;
    int row = 0;
    int room = 0;
    
    if (type == 'M') {
        while (cin >> label) {
            if (label == '/' && cin.peek() == '/') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                if (label == 'S') {
                    start.roomNum = room;
                    start.rowNum = row;
                    start.colNum = col;
                }
                if (label == '.' || label == '#' || label == 'S' ||
                    label == 'R' || isdigit(label)) {
                    update[i] = label;
                    ++i;
                    if (col < rowxcol - 1) {
                        ++col;
                    }
                    else if (col == rowxcol - 1 && row < rowxcol - 1) {
                        ++row;
                        col = 0;
                    }
                    else if (col == rowxcol - 1 && row == rowxcol - 1) {
                        col = 0;
                        row = 0;
                        ++room;
                    }

                }
                else {
                    return false;
                }
            }
        }
        return true;
    }
    
    else if (type == 'L') {
        char inter;
        while (!cin.eof()) {
            cin >> inter;
            if (inter == '/' && cin.peek() == '/') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                if (inter != '(') return cin.eof();
                
                cin >> row;
                if (row < 0 || row >= rowxcol) return false;
                
                cin >> inter;
                if (inter != ',') return false;
                
                cin >> col;
                if (col < 0 || col >= rowxcol) return false;
                
                cin >> inter;
                if (inter != ',') return false;
                
                cin >> room;
                if (room < 0 || room >= roomNum) return false;
                
                cin >> inter;
                if (inter != ',') return false;
                
                cin >> label;
                if (label == '.' || label == '#' || label == 'S' ||
                    label == 'R' || isdigit(label)) {
                    update[(room*rowxcol*rowxcol) + (row*rowxcol)
                           + col] = label;
                    if (label == 'S') {
                        start.roomNum = room;
                        start.rowNum = row;
                        start.colNum = col;
                    }
                }
                else return false;
                
                cin >> inter;
                if (inter != ')') return false;
            }
        }
        return true;
    }
    return false;
}

bool addDequeElements(deque<pathElement> &dequeElements, vector<char> &input,
                      pathElement &start, int roomNum, int rowxcol, bool useStack) {
    pathElement *next;
    pathElement *previous;
    
    addElement(dequeElements, start, useStack);
    
    while (!dequeElements.empty()) {
        previous = &dequeElements.front();
        dequeElements.pop_front();

        if (previous->rowNum - 1 >= 0) {
            // If tile to the north is walkable space or portal
            if (checkTile(input, previous->roomNum, previous->rowNum - 1,
                          previous->colNum, rowxcol) == 1) {
                next = new pathElement;
                next->colNum = previous->colNum;
                next->rowNum = previous->rowNum - 1;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                markAdded(input, *next, rowxcol);
            }
            // If tile to the north is the ring
            else if (checkTile(input, previous->roomNum, previous->rowNum - 1,
                               previous->colNum, rowxcol) == 2) {
                next = new pathElement;
                next->colNum = previous->colNum;
                next->rowNum = previous->rowNum - 1;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                return true;
            }
        }
        if (previous->colNum + 1 < rowxcol) {
            if (checkTile(input, previous->roomNum, previous->rowNum,
                          previous->colNum + 1, rowxcol) == 1) {
                next = new pathElement;
                next->colNum = previous->colNum + 1;
                next->rowNum = previous->rowNum;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                markAdded(input, *next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum + 1, rowxcol)== 2) {
                next = new pathElement;
                next->colNum = previous->colNum + 1;
                next->rowNum = previous->rowNum;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                return true;
            }
        }
        if (previous->rowNum + 1 < rowxcol) {
            if (checkTile(input, previous->roomNum, previous->rowNum + 1,
                          previous->colNum, rowxcol) == 1) {
                next = new pathElement;
                next->colNum = previous->colNum;
                next->rowNum = previous->rowNum + 1;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                markAdded(input, *next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum + 1,
                               previous->colNum, rowxcol) == 2) {
                next = new pathElement;
                next->colNum = previous->colNum;
                next->rowNum = previous->rowNum + 1;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                return true;
            }
        }
        if (previous->colNum - 1 >= 0) {
            if (checkTile(input, previous->roomNum, previous->rowNum,
                          previous->colNum - 1, rowxcol) == 1) {
                next = new pathElement;
                next->colNum = previous->colNum - 1;
                next->rowNum = previous->rowNum;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                markAdded(input, *next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum - 1, rowxcol) == 2) {
                next = new pathElement;
                next->colNum = previous->colNum - 1;
                next->rowNum = previous->rowNum;
                next->roomNum = previous->roomNum;
                next->source = previous;
                addElement(dequeElements, *next, useStack);
                return true;
            }
        }
        if (isdigit(input[(previous->roomNum*rowxcol*rowxcol) + (previous->rowNum*rowxcol)
                          + previous->colNum])) {
            int destination = input[(previous->roomNum*rowxcol*rowxcol) +
                                    (previous->rowNum*rowxcol) + previous->colNum] - '0';
            if (destination < roomNum && destination != previous->roomNum) {
                if (checkTile(input, destination, previous->rowNum,
                    previous->colNum, rowxcol) == 1) {
                    next = new pathElement;
                    next->colNum = previous->colNum;
                    next->rowNum = previous->rowNum;
                    next->roomNum = destination;
                    next->source = previous;
                    addElement(dequeElements, *next, useStack);
                    markAdded(input, *next, rowxcol);
                }
                else if (checkTile(input, destination, previous->rowNum,
                                   previous->colNum, rowxcol) == 2)  {
                    next = new pathElement;
                    next->colNum = previous->colNum;
                    next->rowNum = previous->rowNum;
                    next->roomNum = destination;
                    next->source = previous;
                    addElement(dequeElements, *next, useStack);
                    return true;
                }
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    // turn off synchronization
    ios_base::sync_with_stdio(false);
    
    // true if stack routing scheme should be used
    // false if queue routing scheme should be used
    bool useStack = true;
    
    // true if output should be in map format
    // false if output should be in list format
    bool useMapOutput = true;
    char type;
    
    int roomNum = 0;
    int rowxcol = 0;
    int idx = 0;
    int routeCounter = 0;
    
    pathElement start;
    deque<pathElement> dequeElements;
    
    while (getopt_long(argc, argv, "sqo:h", longopts, &idx) != -1) {
        switch (idx) {
            case 0:
                useStack = true;
                ++routeCounter;
                break;
            case 1:
                useStack = false;
                ++routeCounter;
                break;
            case 2:
                useMapOutput = (optarg[0] == 'M');
                break;
            case 3:
                cout << "Need some help? This program finds paths and stuff. "
                << "Specify some command line arguments if you want: "
                << "stack for stack routing scheme, queue for queue "
                << "routing scheme, output for output type. If you "
                << "include output, you probably want to follow with "
                << "M or L for map or list output format. Or else...\n";
                break;
        }
    }
    if (routeCounter == 0 || routeCounter > 1) {
        cerr << "What are you trying to do? Don't be indecisive. Pick one "
        << "routing scheme.\n";
        exit(1);
    }
    
    cin >> type >> rowxcol >> roomNum;
    vector<char> input(roomNum*rowxcol*rowxcol, '.');
    if (!readInMap(input, roomNum, rowxcol, type, start)) {
        // If invalid input, output error message
        cerr << "Invalid input. Got yourself some illegal "
        << "map characters\n";
        exit(1);
    }
    
    if (addDequeElements(dequeElements, input, start, roomNum, rowxcol, useStack)) {
        cout << "We found the ring!!!\n";
    }
    while (!dequeElements.empty()) {
        pathElement *cleanUp = &dequeElements.front();
        delete cleanUp;
        dequeElements.pop_front();
    }
    //delete all dynamic memory
    
    return 0;
}