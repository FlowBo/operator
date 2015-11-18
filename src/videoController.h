#pragma once

#include "ofMain.h"
#include "ofxAruco.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofBitmapFont.h"

class videoController {
public:
    //Video
    videoController();
    void setup(int useExternalCamera);
    void update();
    void draw();
    
    int camWidth;
    int camHeight;
    ofVideoGrabber video;
    unsigned char * pixels;
    
    int getCamWidth();
    
    // Color
    std::vector<ofColor> trueColors;
    std::vector<ofVec2f> mirrorPos;
    std::vector<ofVec2f> mirrorVecInterpolated;
    void calculateColors();
    ofVec2f interpolateVec(ofVec2f v);
    ofColor findSurroundingColors(unsigned char * image, int x, int y, float dia);
    void drawColors();
    
    //Marker
    
    void displayMarker();
    void drawMarker();
    
    ofImage board;
    ofBaseVideoDraws * cam;
//    std::vector<ofVec2f> mirrorVec;
//    std::vector<ofVec2f> mirrorVecInterpolated;
    std::vector<ofVec2f> convertedMarker;
    void updateMarker(std::vector<ofVec2f> m);
    ofVec2f TL;
    ofVec2f TR;
    ofVec2f BL;
    ofVec2f BR;

    ofxAruco aruco;
    
    
};