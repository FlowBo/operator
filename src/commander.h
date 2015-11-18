#pragma once

#include "ofMain.h"
#include "videoController.h"
#include "tinyG.hpp"
#include <fstream>


enum currentCycle{
    HOME,
    SCREW_HOME,
    ENGAGING_SCREW,
    DISENGAGING_SCREW,
    SCAN_COLORS,
    GOTO_COLOR,
    TRAVERSE,
    REST,
    NEW_MIRROR,
    GOTO_ZERO_A
};

class commander{
public:
    commander();
    void setup(videoController *vController);
    void update();
    void draw();
    
    ifstream fin;
    ofstream fout;
    videoController *vController;
    tinyG tinyg;

    void sendGCode(string gCode);
    
    bool useTinyG = false;
    
    //Modul Positions
    std::vector <string> modulPositions;
    void updatePosition(int ID, string pos);
    
    //Cycles
    currentCycle cycle;
    void operating();
    void engageScrewCycle();
    void disengageScrewCycle();
    void screwHomeCycle();
    void gotoZeroA();
    void goToCloasestColor();
    void updateCloasestColor();
    
    bool boot = true;
    
    void getNextColor();
    void getNextMirror();
    
    void getMirrorPos();
    void scanColors();
    int currentMirror;
    float cloasestColorAPos;
    
    bool dryRun = true;
    
};

