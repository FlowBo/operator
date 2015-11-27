#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    if(useExternalCamera == 0)
    {
        vController.setup(useExternalCamera);
    }else if (useExternalCamera == 1)
    {
        vController.setup(useExternalCamera);
    }
    op900.setup(&vController);
    image.loadImage("red_square.png");
    for(int x = image.width; x >= 0;x--){
        for (int y = image.height; y >= 0; y--) {
            imageColorsSorted.push_back(image.getColor(x, y));
        }
    }
    ofShowCursor();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    guiSetup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    op900.update();
    if(useExternalCamera < 2){
       vController.update();
    }
    
}   
//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(200, 200, 200);
    ofSetColor(0, 0, 0);
    if(useExternalCamera < 2){
        vController.draw();
    }
    op900.draw();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetWindowHeight() - 50);
    ofDrawBitmapString("Incement: " + ofToString(increment), 10,ofGetWindowHeight() - 30);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '+'){
        increment += 0.02;
        if (increment > 10) {
            increment = 10;
        }
    }
    if(key == '-'){
        increment -= 0.02;
        if (increment < 0.02) {
            increment = 0.02;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'n'){
        string pId, newPosX, newPosY;
        pId = ofSystemTextBoxDialog("Panel ID", pId);
        newPosX = ofSystemTextBoxDialog("X Position", newPosX);
        newPosY = ofSystemTextBoxDialog("Y Position", newPosY);
        ofLog(OF_LOG_NOTICE) << "ID: " << pId << " Pos: " << newPosX << ", " << newPosY << endl;
        op900.updateModulPosition(ofToInt(pId), ofToFloat(newPosX), ofToFloat(newPosY));
        
    }
    if(key == 'g'){
        string gCode;
        gCode = ofSystemTextBoxDialog("Send Gcode to TinyG", gCode);
        op900.sendGCode(gCode);
    }
    if(key == 'c'){
        //Start Maschine
        op900.start();
    }
    if(key == 's'){
        //Get Status
        op900.sendGCode("{sr:n}");
    }
    if (key == 'o') {
        //Set new offset for
//        string offsetX;
//        string offsetY;
//        
//        offsetX = ofSystemTextBoxDialog("Enter X-Offset (Position)", offsetX);
//        offsetY = ofSystemTextBoxDialog("Enter Y-Offset (Position)", offsetY);
//        op900.updateOffset(offsetX,offsetY);
//        
        op900.setG55();
    }
    if(key == OF_KEY_LEFT){
        float xPos = op900.getPosition('x');
        string g = ofToString(xPos - increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newXPos:" << g << endl;
    }
    if(key == OF_KEY_RIGHT){
        float xPos = op900.getPosition('x');
        string g = ofToString(xPos + increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newXPos:" << g << endl;
    }
    if(key == OF_KEY_UP){
        float yPos = op900.getPosition('y');
        string g = ofToString(yPos + increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newYPos:" << g << endl;
    }
    if(key == OF_KEY_DOWN){
        float yPos = op900.getPosition('y');
        string g = ofToString(yPos - increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newYPos:" << g << endl;

    }
    if(key == ','){
        float zPos = op900.getPosition('z');
        string g = ofToString(zPos + increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newZPos:" << g << endl;
    }
    if(key == '.'){
        float zPos = op900.getPosition('z');
        string g = ofToString(zPos - increment);
        op900.sendGCode(g);
        ofLog(OF_LOG_NOTICE) << "newZPos:" << g << endl;
    }
    if(key == ' '){
        if (feedhold) {
            op900.sendGCode("~");
            ofLog(OF_LOG_NOTICE) << "Resume" << endl;
        }else{
            op900.sendGCode("!");
            ofLog(OF_LOG_NOTICE) << "Feedhold" << endl;
        }
    }
    if (key == 'm') {
        string id;
        ofSystemTextBoxDialog("Module ID for new Pos", id);
        int idi = ofToInt(id);
        op900.newModulPos(idi);
    }
    if(key == 'r'){
        gotoModulButtonPressed();
    }
    if(key == 'a'){
        op900.setScrewGap();
    }
}


void ofApp::guiSetup(){
    gotoModulButton.addListener(this,&ofApp::gotoModulButtonPressed);
    gui.setup();
    gui.add(gotoModulButton.setup("GotoModul"));
}
void ofApp::gotoModulButtonPressed(){
    string id;
    ofSystemTextBoxDialog("go to Module ID", id);
    int idi = ofToInt(id);
    op900.gotoModul(idi);
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


