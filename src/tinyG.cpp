

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
};


void tinyG::statusUpdate(){
    
    if (ofGetElapsedTimeMillis()% statusInterval == 0 ) {
        ofx::IO::ByteBuffer textBuffer("{sr:n}");
        device.writeBytes(textBuffer);
        device.writeByte('\n');
    }
    try
    {
        std::ostringstream convert;
        uint8_t buffer[10000];
        while (device.available() > 99 )
        {
            std::size_t sz = device.readBytes(buffer, 10000);
            for (int i = 0; i < sz; ++i)
            {
                convert << buffer[i];
            }
            string parser = convert.str(); //convert Byts to String
            result = parser;//Convert To JSON
            //Get Values
            xPos = result["r"]["sr"]["posX"].asFloat();
            yPos = result["r"]["sr"]["posY"].asFloat();
            zPos = result["r"]["sr"]["posZ"].asFloat();
            aPos = result["r"]["sr"]["posA"].asFloat();
            stat = result["r"]["sr"]["stat"].asInt();
        }
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::update") << exc.what();
    }
}




void tinyG::update(){
    try
    {
        uint8_t buffer[10000];
        std::ostringstream convert;
        
        bytesAsString = "";
        
        //99-> min string length
        while (device.available() > 99)
        {
            std::size_t sz = device.readBytes(buffer, 10000);
            
            for (int i = 0; i < sz; ++i)
            {
                convert << buffer[i];
            }
            
            bytesAsString = convert.str();
            result = bytesAsString;
            cout << bytesAsString << endl;
            stat = result["sr"]["stat"].asInt();
            xPos = result["sr"]["posx"].asFloat();
            yPos = result["sr"]["posy"].asFloat();
            zPos = result["sr"]["posz"].asFloat();
            aPos = result["sr"]["posa"].asFloat();
            cout << stat << endl;
//            cout << bytesAsString.at(bytesAsString.find("stat")+6) << endl;
            
            
//            cout << result["sr"]["stat"].asInt();
//            cout <<  bytesAsString.find("stat") << endl;
        }
        
        
        
        
//        if(stat != lastStat){
//            cout << "stat: " << stat << endl;
//            lastStat = stat;
//            if (stat == 3) {
//                busy = false;
//            }else {
//                busy = true;
//            }
//            
//        }
        
        if (stat == 3) {
            busy = false;
        }else {
            busy = true;
        }
        
        if(text != ""){
            
            ofx::IO::ByteBuffer textBuffer(text);
            device.writeBytes(textBuffer);
            device.writeByte('\n');
            cout << text << endl;
        }
        text = "";
    }
    
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::update") << exc.what();
    }
    
//    if(!busy){
//        string nCode = "g1x" + ofToString(ofRandom(0, 900))+"y"+ofToString(ofRandom(0, 900)) + "f6000";
//        cout << "-----> " << counter <<" New Pos: " << nCode << endl;
//        busy = true;
//        sendGcode(nCode);
//        counter++;
//    }
};

void tinyG::draw(){
    ofSetColor(0,0,0);
    int y = 700;
//    for (std::vector<string>::iterator iter = byteStream.begin(); iter != byteStream.end(); ++iter){
//        ofDrawBitmapString(bytesAsString , 100,y);
//        y -= 35;
//    }
}

void tinyG::sendGcode(string t){
    text = t;
};





