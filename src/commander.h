#pragma once

#include "ofMain.h"
#include "videoController.h"
#include "tinyG.hpp"
#include <fstream>


enum cycle{
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
    
    ifstream finX;
    ofstream foutX;
    ifstream finY;
    ofstream foutY;
    
    float screwGap = 25;
    
    videoController *vController;
    tinyG tinyg;

    void sendGCode(string gCode);
    
    bool useTinyG = false;
    
    //Modul Positions
    std::vector <string> modulPositions;
    std::vector <float> modulPositionsX;
    std::vector <float> modulPositionsY;
    
    std::vector <float> mirrorPositionX;
    std::vector <float> mirrorPositionY;
    
    void calcMirrorPosPerModul();

    
    void updatePosition(string ID, string posX, string posY);
    void updateOffset(string offsetX ,string offsetY);

    //Cycles
    enum cycle cycle;
    enum cycle lastCycle;
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
    
    void printStatus();
    
    void getMirrorPos();
    void scanColors();
    int currentMirror;
    float cloasestColorAPos;
    
    bool dryRun = true;
    
    
    

    
};

