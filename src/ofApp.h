#pragma once

#include "ofMain.h"
#include "ofxAruco.h"
#include "commander.h"
#include "videoController.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool shift = false;
    void guiSetup();
    
    float increment = 0.1;
    float add = 0.1;
    
    float feedhold = false;
    
    videoController vController;
    
    int useExternalCamera = 2;
    
    ofxAruco aruco;
    
    string PanelID;
    string newPosition;
    
    ofImage image;
    std::vector<ofColor>imageColorsSorted;
    
    
    //Gui
    ofxPanel gui;
    ofxButton gotoModulButton;
    void gotoModulButtonPressed();
    commander op900;
};
