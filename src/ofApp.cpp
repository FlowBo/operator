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
    for(int x = image.width-1; x >= 0;x--){
        for (int y = image.height-1; y >= 0; y--) {
            imageColorsSorted.push_back(image.getColor(x, y));
            cout << "(" << image.getColor(x, y) << ")";
        }
    }
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
        string pId;
        pId = ofSystemTextBoxDialog("Panel ID", pId);
        int pIdConvert =  ofToInt(pId);
        string newPos;
        newPos = ofSystemTextBoxDialog("New Position", newPos);
        cout << "ID: " << pIdConvert << " Pos: " << newPos << endl;
        op900.updatePosition(pIdConvert, newPos);
    }
    if(key == 'g'){
        string gCode;
        gCode = ofSystemTextBoxDialog("Send Gcode to TinyG", gCode);
        op900.sendGCode(gCode);
    }
    if(key == ' '){
        op900.sendGCode("G80");
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
