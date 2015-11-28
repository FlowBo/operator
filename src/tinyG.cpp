

#include "tinyG.hpp"


tinyG::tinyG(){};

void tinyG::setup(){
    stat = 5;
    lastStat = 100;
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        
        //        bool success = device.setup(devicesInfo[0], 115200);
        bool success =device.setup(devicesInfo[0], 115200,
                                   ofx::IO::SerialDevice::DATA_BITS_EIGHT,
                                   ofx::IO::SerialDevice::PAR_NONE,
                                   ofx::IO::SerialDevice::STOP_ONE,
                                   ofx::IO::SerialDevice::FLOW_CTRL_SOFTWARE,
                                   ofx::IO::SerialDevice::DEFAULT_TIMEOUT);
        
        if(success)
        {
            ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
            device.setRequestToSend(true);
            device.setDataTerminalReady(true);
            
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
    cout << "FlowControl: " << device.getFlowControl() << endl;
    
    sendGcode("$ej 1");
    sendGcode("g28.2x0y0z0");
    sendGcode("g55");
    
};

void tinyG::printStatus(){
    cout << "xPos: " << xPos << endl;
    cout << "yPos: " << yPos << endl;
    cout << "zPos: " << zPos << endl;
    cout << "aPos: " << aPos << endl;
    cout << "stat: " << stat << endl;
}

void tinyG::statusUpdate(){
    if (ofGetElapsedTimeMillis()% statusInterval < 10 ) {
        sendGcode("{sr:n}");
    }
}

void tinyG::update(){
    if(text != ""){
        ofx::IO::ByteBuffer textBuffer(text);
        device.writeBytes(textBuffer);
        device.writeByte('\n');
        cout << text << endl;
        text = "";
    }
    if (lastMessage != statusReport.message) {
        lastMessage = statusReport.message;
        ofLog(OF_LOG_VERBOSE) <<  "Last Message: " << lastMessage;
        result.parse(statusReport.message);
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


//void tinyG::update(){
//    statusUpdate();
//    try
//    {
//
//        uint8_t buffer[10000];
//        std::ostringstream convert;
//
//        bytesAsString = "";
//
//        //99-> min string length
//        while (device.available() > 99)
//        {
//            std::size_t sz = device.readBytes(buffer, 10000);
//
//            for (int i = 0; i < sz; ++i)
//            {
//                convert << buffer[i];
//            }
//
//            bytesAsString = convert.str();
//            cout << "bytesAsString: " <<bytesAsString << endl;
//            result = bytesAsString;
//            cout << "Result: " <<result << endl;
//            stat = result["r"]["sr"]["stat"].asInt();
//            xPos = result["r"]["sr"]["posx"].asFloat();
//            yPos = result["r"]["sr"]["posy"].asFloat();
//            zPos = result["r"]["sr"]["posz"].asFloat();
//            aPos = result["r"]["sr"]["posa"].asFloat();
//        }
//
//        if (stat == 3) {
//            busy = false;
//        }else {
//            busy = true;
//        }
//
//        if(text != ""){
//
//            ofx::IO::ByteBuffer textBuffer(text);
//            device.writeBytes(textBuffer);
//            device.writeByte('\n');
//            cout << text << endl;
//        }
//        text = "";
//        device.flushInput();
//        device.flushOutput();
//
//    }
//
//    catch (const std::exception& exc)
//    {
//        ofLogError("ofApp::update") << exc.what();
//    }
//   };

void tinyG::draw(){
    ofSetColor(0,0,0);
    int y = 700;
    
}

void tinyG::sendGcode(string t){
    text = t;
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




