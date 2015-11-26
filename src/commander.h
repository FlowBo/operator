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
    
    float screwGap = 33.56;
    
    videoController *vController;
    tinyG tinyg;


    //Setup
    bool useTinyG = false;
    void start();
    
    //Modul Positions
    std::vector <string> modulPositions;
    std::vector <float> modulPositionsX;
    std::vector <float> modulPositionsY;
    void newModulPos(int idi);
    void updateModulPosition(int ID, float posX, float posY);
    void updateOffset(string offsetX ,string offsetY);

    //Mirror Pos
    std::vector <float> mirrorPositionX;
    std::vector <float> mirrorPositionY;
    void calcMirrorPosPerModul();
    
    //Motion Commands
    void sendGCode(string gCode);
    void gotoModul(int ID);
    
    //Status Report
    float getPosition(char axis);

private:
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
    bool startMachine = false;
    
    

    
};

