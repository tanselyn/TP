
// Author:  Yupeng Wang
// EECS 281
// Project 1
///////////////////////////////////////////////////////////
//potato
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

int main(int argc, char *argv[]) {
    // turn off synchronization
    ios_base::sync_with_stdio(false);
    
    int idx = 0;
    int routeCounter = 0;
    char compare = 'L';
    bool useStack = true;
    bool useMap = true;
    
    int roomNum = 0;
    int rowxcol = 0;
    
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
                useMap = strcmp(&compare, optarg);
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
    
    cin >> compare >> rowxcol >> roomNum;
    vector<char> input(roomNum*rowxcol*rowxcol, '.');
    
    
    
    if (useStack) {
        
        
        return 0;
    }