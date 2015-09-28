
// Author:  Yupeng Wang
// EECS 281
// Project 1
///////////////////////////////////////////////////////////

#include <vector>
#include <deque>
#include <stack>
#include <sstream>
#include <iostream>
#include <getopt.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <limits>
#include "project1.h"
#include <fstream>

using namespace std;

static struct option longopts[] = {
    {"stack", no_argument, nullptr, 's'},
    {"queue", no_argument, nullptr, 'q'},
    {"output", required_argument, nullptr, 'o'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
};

void addElement(std::deque<mapElement> &dequeElements, const mapElement &add, bool useStack) {
    if (useStack) {
        dequeElements.push_front(add);
    }
    else dequeElements.push_back(add);
}

int checkTile(std::vector<char> &input, int room, int row, int col, int rowxcol) {
    
    if (input[(room*rowxcol*rowxcol) + (row*rowxcol) + col] == '.') {
        return 1;
    }
    else if (isdigit(input[(room*rowxcol*rowxcol) + (row*rowxcol) + col])) {
        return 2;
    }
    else if (input[(room*rowxcol*rowxcol) + (row*rowxcol) + col]  == 'R') {
        return 3;
    }
    return 0;
}

void markAdded(std::vector<char> &input, const mapElement &added, int rowxcol) {
    input[(added.roomNum*rowxcol*rowxcol) + (added.rowNum*rowxcol)
          + added.colNum] = '~';
}

bool readInMap(vector<char> &update, int roomNum, int rowxcol, const char type,
               mapElement &start) {
    char label;
    int i = 0;
    int col = 0;
    int row = 0;
    int room = 0;
    
    if (type == 'M') {
        while (cin >> label) {
            if ((label == '/' && cin.peek() == '/') || cin.eof()) {
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
            if ((inter == '/' && cin.peek() == '/') || cin.eof()) {
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
                        start.colNum = col;;
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

bool addDequeElements(deque<mapElement> &dequeElements, vector<char> input,
                      vector<char> &path, mapElement &start, int roomNum,
                      int rowxcol, bool useStack) {
    mapElement next(0, 0, 0, 0);
    mapElement *previous;
    cout << "ok" << endl;
    addElement(dequeElements, start, useStack);
    
    while (!dequeElements.empty()) {
        previous = &dequeElements.front();
        dequeElements.pop_front();
        
        if (previous->rowNum - 1 >= 0) {
            mapElement next (previous->roomNum, previous->rowNum - 1, previous->colNum, 100);
            addElement(dequeElements, next, useStack);
            path[(next.roomNum*rowxcol*rowxcol) + (next.rowNum*rowxcol) +
                 next.colNum] = 'n';
            
            // If tile to the north is walkable space
            if (checkTile(input, previous->roomNum, previous->rowNum - 1,
                          previous->colNum, rowxcol) == 1) {
                cout << "N" << endl;
                markAdded(input, next, rowxcol);
            }
            // If the tile to the north is a portal
            else if (checkTile(input, previous->roomNum, previous->rowNum - 1,
                               previous->colNum, rowxcol) == 2) {
                next.portal = input[(next.roomNum*rowxcol*rowxcol) +
                                    (next.rowNum*rowxcol) + next.colNum] - '0';
                cout << "N" << endl;
                markAdded(input, next, rowxcol);
            }
            // If tile to the north is the ring
            else if (checkTile(input, previous->roomNum, previous->rowNum - 1,
                               previous->colNum, rowxcol) == 3) {
                cout << "N" << endl;
                return true;
            }
        }
        if (previous->colNum + 1 < rowxcol) {
            mapElement next (previous->roomNum, previous->rowNum, previous->colNum + 1, 100);
            addElement(dequeElements, next, useStack);
            path[(next.roomNum*rowxcol*rowxcol) + (next.rowNum*rowxcol) +
                 next.colNum] = 'e';
            if (checkTile(input, previous->roomNum, previous->rowNum,
                          previous->colNum + 1, rowxcol) == 1) {
                cout << next.rowNum << endl << next.colNum << endl;
                cout << "E " << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum + 1, rowxcol) == 2) {
                next.portal = input[(next.roomNum*rowxcol*rowxcol) +
                                    (next.rowNum*rowxcol) + next.colNum] - '0';
                cout << "E" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum + 1, rowxcol)== 3) {
                cout << "E" << endl;
                return true;
            }
        }
        if (previous->rowNum + 1 < rowxcol) {
            mapElement next (previous->roomNum, previous->rowNum + 1, previous->colNum, 100);
            addElement(dequeElements, next, useStack);
            path[(next.roomNum*rowxcol*rowxcol) + (next.rowNum*rowxcol) +
                 next.colNum] = 's';
            if (checkTile(input, previous->roomNum, previous->rowNum + 1,
                          previous->colNum, rowxcol) == 1) {
                cout << "S" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum + 1,
                               previous->colNum, rowxcol) == 2) {
                next.portal = input[(next.roomNum*rowxcol*rowxcol) +
                                    (next.rowNum*rowxcol) + next.colNum] - '0';
                cout << "S" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum + 1,
                               previous->colNum, rowxcol) == 3) {
                cout << "S" << endl;
                return true;
            }
        }
        if (previous->colNum - 1 >= 0) {
            mapElement next (previous->roomNum, previous->rowNum, previous->colNum - 1, 100);
            addElement(dequeElements, next, useStack);
            path[(next.roomNum*rowxcol*rowxcol) + (next.rowNum*rowxcol) +
                 next.colNum] = 'w';
            if (checkTile(input, previous->roomNum, previous->rowNum,
                          previous->colNum - 1, rowxcol) == 1) {
                cout << "W" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum - 1, rowxcol) == 2) {
                next.portal = input[(next.roomNum*rowxcol*rowxcol) +
                                    (next.rowNum*rowxcol) + next.colNum] - '0';
                cout << "W" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->roomNum, previous->rowNum,
                               previous->colNum - 1, rowxcol) == 3) {
                cout << "W" << endl;
                return true;
            }
        }
        if (previous->portal < roomNum && previous->portal != previous->roomNum) {
            mapElement next (previous->portal, previous->rowNum, previous->colNum, 100);
            addElement(dequeElements, next, useStack);
            path[(next.roomNum*rowxcol*rowxcol) + (next.rowNum*rowxcol) +
                 next.colNum] = previous->roomNum + '0';
            if (checkTile(input, previous->portal, previous->rowNum,
                          previous->colNum, rowxcol) == 1) {
                cout << "P" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->portal, previous->rowNum,
                               previous->colNum, rowxcol) == 2) {
                next.portal = input[(next.roomNum*rowxcol*rowxcol) +
                                    (next.rowNum*rowxcol) + next.colNum] - '0';
                cout << "P" << endl;
                markAdded(input, next, rowxcol);
            }
            else if (checkTile(input, previous->portal, previous->rowNum,
                               previous->colNum, rowxcol) == 3)  {
                cout << "P" << endl;
                return true;
            }
        }
    }
    return false;
}

pathElement tracePrevious(std::vector<char> &path, stack<pathElement> &trace,
                          mapElement current, int roomNum, int rowxcol) {
    char direction = path[(current.roomNum*rowxcol*rowxcol) +
                          (current.rowNum*rowxcol) + current.colNum];
    
    if (direction == 'S') {
        pathElement previous(direction);
        previous.info.roomNum = current.roomNum;
        previous.info.rowNum = current.rowNum;
        previous.info.colNum = current.colNum;
        previous.info.portal = 100;
        return previous;
    }
    else if (direction == 'n') {
        pathElement previous(direction);
        previous.info.roomNum = current.roomNum;
        previous.info.rowNum = current.rowNum + 1;
        previous.info.colNum = current.colNum;
        previous.info.portal = 100;
        current.rowNum = current.rowNum + 1;
        trace.push(tracePrevious(path, trace, current, roomNum, rowxcol));
    }
    else if (direction == 'e') {
        pathElement previous(direction);
        previous.info.roomNum = current.roomNum;
        previous.info.rowNum = current.rowNum;
        previous.info.colNum = current.colNum - 1;
        previous.info.portal = 100;
        current.colNum = current.colNum - 1;
        trace.push(tracePrevious(path, trace, current, roomNum, rowxcol));
    }
    else if (direction == 's') {
        pathElement previous(direction);
        previous.info.roomNum = current.roomNum;
        previous.info.rowNum = current.rowNum - 1;
        previous.info.colNum = current.colNum;
        previous.info.portal = 100;
        current.rowNum = current.rowNum - 1;
        trace.push(tracePrevious(path, trace, current, roomNum, rowxcol));
    }
    else if (direction == 'w') {
        pathElement previous(direction);
        previous.info.roomNum = current.roomNum;
        previous.info.rowNum = current.rowNum;
        previous.info.colNum = current.colNum + 1;
        previous.info.portal = 100;
        current.colNum = current.colNum + 1;
        trace.push(tracePrevious(path, trace, current, roomNum, rowxcol));
    }
    else if (isdigit(direction)) {
        pathElement previous(direction);
        previous.info.roomNum = direction - '0';
        previous.info.rowNum = current.rowNum;
        previous.info.colNum = current.colNum;
        previous.info.portal = direction;
        current.roomNum = direction - '0';
        trace.push(tracePrevious(path, trace, current, roomNum, rowxcol));
    }
    return pathElement('a');
}

int main(int argc, char *argv[]) {
    // turn off synchronization
    ios_base::sync_with_stdio(false);
    
    ostringstream os;
    
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
    
    mapElement start;
    mapElement end;
    deque<mapElement> dequeElements;
    stack<pathElement> trace;
    
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
    vector<char> path(roomNum*rowxcol*rowxcol, '.');
    
    if (!readInMap(input, roomNum, rowxcol, type, start)) {
        // If invalid input, output error message
        cerr << "Invalid input. Got yourself some illegal "
        << "map characters\n";
        exit(1);
    }
    
    if (addDequeElements(dequeElements, input, path, start, roomNum, rowxcol, useStack)) {
        
        os << rowxcol << '\n' << roomNum << '\n';
        if (useStack) end = dequeElements.front();
        else end = dequeElements.back();
        tracePrevious(path, trace, end, roomNum, rowxcol);
        
        if (useMapOutput) {
            while (!trace.empty()) {
                pathElement check = trace.top();
                trace.pop();
                input[(check.info.roomNum*rowxcol*rowxcol) + (check.info.rowNum*rowxcol)
                      + check.info.colNum] = check.direction;
            }
            for (int i = 0; i < path.size(); ++i) {
                if (i % (rowxcol*rowxcol) == 0) {
                    os << '\n' << "//room " << (i + 1) / (rowxcol*rowxcol) << '\n'
                    << input[i];
                }
                else if (i % rowxcol == 0) {
                    os << '\n' << input[i];
                }
                else os << input[i];
            }
        }
        else {
            os << "//path taken" << '\n';
            while (!trace.empty()) {
                pathElement check = trace.top();
                trace.pop();
                os << "(" << check.info.rowNum << "," << check.info.colNum
                << "," << check.info.roomNum << "," << check.direction
                << ")" << '\n';
            }
            
        }
    }
    
    // Ring is not found
    else {
        os << rowxcol << '\n' << roomNum << '\n';
        // output the same thing as the input (without path taken shown)
        if (useMapOutput) {
            for (int i = 0; i < input.size(); ++i) {
                if (i % (rowxcol*rowxcol) == 0) {
                    os << '\n' << "//room " << (i + 1) / (rowxcol*rowxcol) << '\n'
                    << input[i];
                }
                else if (i % rowxcol == 0) {
                    os << '\n' << input[i];
                }
                else os << input[i];
            }
        }
        else{
            os << "//path taken" << '\n';
        }
    }
    
    cout << os.str();
    return 0;
}