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
    cout << imageColorsSorted.size() << endl;
    changeThisString = "yeah";
    ofShowCursor();
    ofSetFrameRate(60);
    
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
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'n'){
        string pId, newPosX, newPosY;
        pId = ofSystemTextBoxDialog("Panel ID", pId);
        newPosX = ofSystemTextBoxDialog("X Position", newPosX);
        newPosY = ofSystemTextBoxDialog("Y Position", newPosY);
        cout << "ID: " << pId << " Pos: " << newPosX << ", " << newPosY << endl;
        op900.updatePosition(pId, newPosX, newPosY);
    }
    if(key == 'g'){
        string gCode;
        gCode = ofSystemTextBoxDialog("Send Gcode to TinyG", gCode);
        op900.sendGCode(gCode);
    }
    if(key == ' '){
        op900.useTinyG = true;
    }

    if (key == 'o') {
        string offsetX;
        string offsetY;
        
        offsetX = ofSystemTextBoxDialog("Enter X-Offset (Position)", offsetX);
        offsetY = ofSystemTextBoxDialog("Enter Y-Offset (Position)", offsetY);
        op900.updateOffset(offsetX,offsetY);
        
    }
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
