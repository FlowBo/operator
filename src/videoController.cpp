//
//  videoController.cpp
//  operator
//
//  Created by Florian Born on 12/10/15.
//
//

#include "videoController.hpp"


videoController::videoController(){};

void videoController::setup(){
    camWidth 		= 1280;	// try to grab at this size.
    camHeight 		= 720;
    video.setDesiredFrameRate(60);
    video.setVerbose(true);
    video.initGrabber( camWidth ,camHeight);
    ofSetVerticalSync(true);
}

void videoController::update(){
    bool bNewFrame = false;
    video.update();
    bNewFrame = video.isFrameNew();
    pixels = video.getPixels();

}

void videoController::draw(){
    video.draw(0,0);
}