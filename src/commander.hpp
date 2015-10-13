#pragma once

#include "ofMain.h"
#include "videoController.hpp"

class commander{
public:
    commander();
    void setup();
    void update();
    void draw();
    
    videoController vController;
    
};