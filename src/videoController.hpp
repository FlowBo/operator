#pragma once

#include "ofMain.h"
//#include "ofxOpenCv.h"
//#include "ofxCv.h"
//#include "aruco.h"

class videoController{
public:
    videoController();
    void setup();
    void update();
    void draw();
    
    
    int camWidth;
    int camHeight;
    ofVideoGrabber video;
    unsigned char * pixels;
    
};