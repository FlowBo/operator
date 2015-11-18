#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
#include "ofxJSON.h"

class tinyG{
public:
    tinyG();
    void setup();
    void update();
    void draw();
    void sendGcode(string t );
    //Getter Function
    bool isBusy(){return busy;};
    
    float getXPos(){return xPos;};
    float getYPos(){return yPos;};
    float getZPos(){return zPos;};
    float getAPos(){return aPos;};
    
private:
    ofx::IO::SerialDevice device;
    char charKey;
    string text;

    int counter = 0;
    bool busy = false;

    
    //TinyG status
    float xPos;
    float yPos;
    float zPos;
    float aPos;
    int stat;
    
    int lastStat;
    
    ofxJSONElement result;
    string bytesAsString;
    std::vector<string> byteStream;
    
    
    

    
};