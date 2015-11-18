
#include "videoController.h"
#include "ofxCv.h"
#include "ofBitmapFont.h"

videoController::videoController(){};

void videoController::setup(int useExternalCamera){
    
    //Video
    cout << "avalible video devices: " <<video.listDevices().size() << endl;
    if (useExternalCamera < 2 && video.listDevices().size() > useExternalCamera) {
        video.setDeviceID(useExternalCamera);
    }else{
        video.setDeviceID(0);
        cout << "external camera not avalible" << endl;
    }
    
    camWidth 		= 1280;	// try to grab at this size.
    camHeight 		= 720;
    video.setDesiredFrameRate(60);
    video.setVerbose(true);
    video.initGrabber( camWidth ,camHeight);
    ofSetVerticalSync(true);
    
    //Marker
    string boardName = "boardConfiguration.yml";
    aruco.setup("intrinsics.int", video.getWidth(), video.getHeight(), boardName);
    aruco.getBoardImage(board.getPixelsRef());
    board.update();
    
    //Colors
    for (float xP = 0; xP < 30; xP += 1) {
        for (float yP = 0; yP < 30; yP += 1) {
            mirrorPos.push_back(ofVec2f(ofMap(xP, 0, 30, 0,1),ofMap(yP, 0, 30, 0, 1)));
            trueColors.push_back(ofColor(0,0,0));
        }
    }
    
    
}

void videoController::update(){
    bool bNewFrame = false;
    video.update();
    bNewFrame = video.isFrameNew();
    if (bNewFrame){
        aruco.detectBoards(video.getPixelsRef());
    };
    if(aruco.getNumMarkers() == 4){
        std::vector<aruco::Marker> m = aruco.getMarkers();
        convertedMarker.clear();
        for (std::vector<aruco::Marker>::iterator it = m.begin(); it != m.end();++it){
            convertedMarker.push_back(ofVec2f((*it).getCenter().x,(*it).getCenter().y ));
        }
        updateMarker(convertedMarker);
    }
    calculateColors();

    pixels = video.getPixels();

}
void videoController::updateMarker(std::vector<ofVec2f> m){
    
    //TODO
    //Replace calculation with marker ID
    float dist = 99999999.0f;
    std::vector<ofVec2f>::iterator toDelete;
    for (std::vector<ofVec2f>::iterator it = m.begin(); it != m.end();++it){
        float pointDist = (*it).distance(ofVec2f(0,0));
        if(pointDist < dist )
        {
            dist = pointDist;
            toDelete = it;
            TL = (*it);
        }
    }
    m.erase(toDelete);
    dist = 0;
    for (std::vector<ofVec2f>::iterator it = m.begin(); it != m.end();++it){
        
        float pointDist = (*it).distance(ofVec2f(0,0));
        if(pointDist > dist )
        {
            dist = pointDist;
            toDelete = it;
            BR = (*it);
        }
    }
    m.erase(toDelete);
    if(m.at(0).x  < m.at(1).x ){
        BL = m.at(0);
        TR = m.at(1);
    }else{
        BL = m.at(1);
        TR = m.at(0);
    }
}


void videoController::calculateColors(){
    mirrorVecInterpolated.clear();
    for(std::vector<ofVec2f>::iterator it = mirrorPos.begin(); it != mirrorPos.end(); ++it){
        mirrorVecInterpolated.push_back(
                                        interpolateVec(ofVec2f( (*it).x, (*it).y) ));
    }
    
    int c = 0;
    for(std::vector<ofVec2f>::iterator it = mirrorVecInterpolated.begin(); it != mirrorVecInterpolated.end(); ++it) {
        int x = (*it).x;
        int y = (*it).y;
        //FIX STEP
        int step = 10;
        ofColor newColor = findSurroundingColors(video.getPixels(),x,y,step);
        trueColors.at(c) = newColor;
        c++;
    }
}
ofColor videoController::findSurroundingColors(unsigned char * image, int x, int y, float dia){
    int r = 0;
    int g = 0;
    int b = 0;
    int counter = 0;
    for (int i = 0; i < dia; i++) {
        for (int j = 0; j < dia; j++) {
            int nX = abs(x+i);
            int nY = abs(y+i);
            int arrayPos = abs((nX+nY*video.getWidth()) * 3);
            ofColor c = ofColor((int)image[arrayPos],(int)image[arrayPos+1],(int)image[arrayPos+2]);
            r += c.r;
            g += c.g;
            b += c.b;
            counter++;
        }
    }
    r /= counter;
    g /= counter;
    b /= counter;
    return ofColor(r,g,b);
}
ofVec2f videoController::interpolateVec(ofVec2f v){
    
    // remapping
    float topX = ofMap(v.x, 0, 1, TL.x, TR.x);
    float bottomX = ofMap(v.x, 0, 1, BL.x, BR.x);
    float leftY = ofMap(v.y, 0,1, TL.y, BL.y);
    float rightY = ofMap(v.y, 0, 1, TR.y, BR.y);
    // interpolation
    float newX = ofLerp(topX, bottomX,v.y);
    float newY = ofLerp(leftY, rightY, v.x);
    //   cout << "(" << newX << ") (" << newY << ")" << endl;
    return ofVec2f(newX,newY);
    
    
}
void videoController::draw(){
    ofSetColor(255, 255, 255);
    video.draw(0,0);
    displayMarker();
    drawColors();
}
void videoController::drawColors(){
    int c = 0;
    for(std::vector<ofVec2f>::iterator it = mirrorVecInterpolated.begin(); it != mirrorVecInterpolated.end(); ++it) {
        int x = (*it).x;
        int y = (*it).y;
        ofColor color = trueColors.at(c);
        ofSetColor(color.r, color.g, color.b);
        //TODO -> FIX SIZE!
        ofRect(x, y, 10, 10);
        c++;
    }
}
void videoController::displayMarker(){
    for(int i=0;i<aruco.getNumMarkers();i++){
        aruco.begin(i);
        drawMarker();
        aruco.end();
    }
    ofDrawBitmapString("topLeft" + ofToString(TL) , TL.x , TL.y);
    ofDrawBitmapString("topRight"+ ofToString(TR), TR.x , TR.y);
    ofDrawBitmapString("bottomLeft"+ ofToString(BL), BL.x , BL.y);
    ofDrawBitmapString("bottomRight"+ ofToString(BR), BR.x , BR.y);
}

void videoController::drawMarker(){
    ofDrawAxis(0.15f);
    ofPushMatrix();
    // move up from the center by size*.5
    // to draw a box centered at that point
    ofTranslate(0,0.15f*0.5,0);
    ofFill();
    ofSetColor(ofColor::white,50);
    ofBox(0.15f);
    ofNoFill();
    ofSetColor(ofColor::white);
    ofBox(0.15f);
    ofPopMatrix();
}
int videoController::getCamWidth(){
    return camWidth;
}

