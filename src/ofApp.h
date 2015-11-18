#pragma once

#include "ofMain.h"
#include "ofxAruco.h"
#include "commander.h"
#include "videoController.h"


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
    commander op900;
    
    videoController vController;
    
    int useExternalCamera = 2;
    
    ofxAruco aruco;
    
    string PanelID;
    string newPosition;
    
    ofImage image;
    std::string changeThisString;
    
    std::vector<ofColor>imageColorsSorted;
    
};
