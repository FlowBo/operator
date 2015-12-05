

#include "tinyG.hpp"


tinyG::tinyG(){};

void tinyG::setup(){
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        bool success = device.setup(devicesInfo[0], 115200);
        
        if(success)
        {
            device.registerAllEvents(this);//  registerAllEvents(this);
         
            ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
        }
        else
        {
            ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }

//    sendGcodeDirect("$ej 1");
//    ofSleepMillis(300);
    stat = 5;
    lastStat = 100;
    lastMessage = "HALLO";
    sendGcode("g28.2x0y0z0");
    
    
};



void tinyG::update(){
    if (lastMessage != statusReport.message) {
        lastMessage = statusReport.message;
        ofLog(OF_LOG_VERBOSE) <<  "Last Message: " << lastMessage;
        result.parse(statusReport.message);
        //        cout << result << endl << endl;
        if (!result["sr"]["stat"].isNull()) {
            stat = result["sr"]["stat"].asInt();
        }
        if (!result["sr"]["posx"].isNull()) {
            xPos = result["sr"]["posx"].asFloat();
        }
        if (!result["sr"]["posy"].isNull()) {
            yPos = result["sr"]["posy"].asFloat();
        }
        if (!result["sr"]["posz"].isNull()) {
            zPos = result["sr"]["posz"].asFloat();
        }
        if (!result["sr"]["posa"].isNull()) {
            aPos = result["sr"]["posa"].asFloat();
        }
        ofLog(OF_LOG_VERBOSE) <<  "xPos: " << xPos;
        ofLog(OF_LOG_VERBOSE)<<   "yPos: " << yPos;
        ofLog(OF_LOG_VERBOSE)<<   "zPos: " << zPos;
        ofLog(OF_LOG_VERBOSE)  << "aPos: " << aPos;
        ofLog(OF_LOG_VERBOSE) <<  "stat: "<<  stat;
        cout << "------------" << endl;
    }
}



void tinyG::sendGcode(string t){
    ofx::IO::ByteBuffer textBuffer(t);
    device.writeBytes(textBuffer);
    device.writeByte('\n');
    cout << t << endl;
};

void tinyG::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args)
{
    SerialMessage message(args.getBuffer().toString(), "", 10000);
    statusReport = message;

}

void tinyG::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    SerialMessage message(args.getBuffer().toString(),
                          args.getException().displayText(),
                          1000);
}




