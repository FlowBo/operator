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
        ofShowCursor();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    guiSetup();
    op900.setup(&vController);
    image.loadImage("red_square.png");
    for(int x = image.width; x >= 0;x--){
        for (int y = image.height; y >= 0; y--) {
            imageColorsSorted.push_back(image.getColor(x, y));
        }
    }

    
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
    std::stringstream ss;
    ss << ofGetFrameRate << endl;
    ss << "Increment: " << increment;
    ofDrawBitmapString(ss.str(), 10,ofGetWindowHeight() - 30);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '+'){
        
        if (shift) {
            increment += 0.1;
        }else{
            increment += 0.02;
        }
        
        if (increment > 10) {
            increment = 10;
        }
    }
    if(key == '-'){
        if (shift) {
            increment -= 0.1;
        }else{
            increment -= 0.02;
        }
        if (increment < 0.02) {
            increment = 0.02;
        }
    }
    if(key == OF_KEY_LEFT){
        float xPos = op900.getPosition('x');
        float nx = xPos - increment;
        std::stringstream g;
        g << "g0x" << nx;
        op900.sendGCode(g.str());
        cout << "newXPos:" << g.str() << endl;
    }
    if(key == OF_KEY_RIGHT){
        float xPos = op900.getPosition('x');
        float nx = xPos + increment;
        std::stringstream g;
        g << "g0x" << nx;
        op900.sendGCode(g.str());
        cout << "newXPos:" << g.str() << endl;
    }
    if(key == OF_KEY_UP){
        float yPos = op900.getPosition('y');
        float ny = yPos + increment;
        std::stringstream g;
        g << "g0y" << ny;
        op900.sendGCode(g.str());
        cout << "newYPos:" << g.str() << endl;
    }
    if(key == OF_KEY_DOWN){
        float yPos = op900.getPosition('y');
        float ny = yPos - increment;
        std::stringstream g;
        g << "g0y" << ny;
        op900.sendGCode(g.str());
        cout << "newYPos:" << g.str() << endl;
        
    }
    if(key == ','){
        float zPos = op900.getPosition('z');
        float nz = zPos + increment;
        std::stringstream g;
        g << "g0z" << nz;
        op900.sendGCode(g.str());
        cout << "newzPos:" << g.str() << endl;
        
    }
    if(key == '.'){
        float zPos = op900.getPosition('z');
        float nz = zPos - increment;
        std::stringstream g;
        g << "g0z" << nz;
        op900.sendGCode(g.str());
        cout << "newzPos:" << g.str() << endl;
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
    if(key == OF_KEY_SHIFT){
        if (!shift) {
            shift = true;
        }else{
            shift = false;
        }
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
        op900.setG55();
    }
       if(key == ' '){
        if (feedhold) {
            op900.sendGCode("~");
            feedhold = false;
            ofLog(OF_LOG_NOTICE) << "Resume" << endl;
        }else{
            op900.sendGCode("!");
            feedhold = true;
            ofLog(OF_LOG_NOTICE) << "Feedhold" << endl;
        }
    }
    if (key == 'm') {
        string id;
        ofSystemTextBoxDialog("Module ID for new Pos", id);
        int idi = ofToInt(id);
        cout << idi << endl;
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


