
// Author:  Yupeng Wang
// EECS 281
// Project 1
///////////////////////////////////////////////////////////

#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <sstream>
#include <iostream>
#include <getopt.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include "project1.h"

using namespace std;

static struct option longopts[] = {
    {"stack", no_argument, nullptr, 's'},
    {"queue", no_argument, nullptr, 'q'},
    {"output", required_argument, nullptr, 'o'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
};

bool readInMap(vector<char> &update, int roomNum, int rowxcol, const char type) {
    char label;
    int i = 0;
    
    if (type == 'M') {
        while (cin.get(label)) {
            if (label == '/' && cin.peek() == '/') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                if (label == '.' || label == '#' || label == 'S' ||
                    label == 'R' || isdigit(label)) {
                    update[i] = label;
                    ++i;
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
        int row, col, room;
        while (!cin.eof()) {
            cin >> inter;
            if (inter == '/' && cin.peek() == '/') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                if (inter != '(') return false;
                
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
                    cin >> inter;
                    if (inter != ')') return false;
                }
            }
        }
        return true;
    }
    return false;
}

void locateStart(std::vector<char> &map, int roomNum, int rowxcol) {
    
}

int main(int argc, char *argv[]) {
    // turn off synchronization
    ios_base::sync_with_stdio(false);

    bool useStack = true;
    bool useMapOutput = true;
    char type;
    
    int roomNum = 0;
    int rowxcol = 0;
    int idx = 0;
    int routeCounter = 0;
    
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
    if (!readInMap(input, roomNum, rowxcol, type)) {
        // If invalid input, output error message
        cerr << "Invalid input. Got yourself some illegal"
        << "map characters\n";
        exit(1);
    }
    
    if (useStack) {
        
        
    }
    return 0;
}