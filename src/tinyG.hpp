#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
#include "ofxJSON.h"


class SerialMessage
{
public:
    SerialMessage(): fade(0)
    {
    }
    
    SerialMessage(const std::string& _message,
                  const std::string& _exception,
                  int _fade):
    message(_message),
    exception(_exception),
    fade(_fade)
    {
    }
    
    std::string message;
    std::string exception;
    int fade;
};

class tinyG{
public:
    tinyG();
    void setup();
    void update();
    void sendGcode(string t );
    
    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);

    
    //Getter Function
    bool isBusy(){return busy;};
    
    float getXPos(){return xPos;};
    float getYPos(){return yPos;};
    float getZPos(){return zPos;};
    float getAPos(){return aPos;};
private:
    ofx::IO::BufferedSerialDevice device;
    char charKey;
    string text;

    int counter = 0;
    bool busy = false;
    
    int statusInterval = 250; // Millis

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
    
    string lastMessage;
    
    SerialMessage statusReport;
};