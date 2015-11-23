//
//  commander.cpp
//  operator
//
//  Created by Florian Born on 12/10/15.
//
//

#include "commander.h"

commander::commander(){
}

void commander::setup(videoController *vController){
    
    finY.open( ofToDataPath("modulPositionsY.txt").c_str() );
    while(finY!=NULL)
    {
        string str;
        getline(finY, str);
        float strToFloat = ofToFloat(str);
        modulPositionsY.push_back(strToFloat);
    }
    finY.close();
    modulPositionsY.erase(modulPositionsY.end()-1);
    
    
    
    
    cout << "positionY size: " << modulPositionsY.size() << endl;
    
    
    finX.open( ofToDataPath("modulPositionsX.txt").c_str() );
    while(finX!=NULL)
    {
        string str;
        getline(finX, str);
        float strToFloat = ofToFloat(str);
        modulPositionsX.push_back(strToFloat);
    }
    finX.close();
    modulPositionsX.erase(modulPositionsX.end()-1);
    
    cout << "positionX size: " << modulPositionsX.size() << endl;
    
    
    
    
    
    
    //    for(std::vector<string>::iterator iter = modulPositions.begin();iter != modulPositions.end();++iter){
    //        cout << (*iter) << endl;
    //    }
    if(useTinyG){
        tinyg.setup();
        cycle = HOME;
        currentMirror = 23;
    }
}

void commander::update(){
    if(useTinyG){
        operating();
        printStatus();
        tinyg.update();
    }
    
}

void commander::printStatus(){
    if(cycle != lastCycle)
    {
        lastCycle = cycle;
        cout << cycle <<endl;
    }
    //    cout << tinyg.isBusy() <<endl;
}

void commander::operating(){
    if(!tinyg.isBusy()){
        switch (cycle) {
            case HOME:
                if(!tinyg.isBusy()){
                    cycle = REST;
                }
                break;
            case REST:
                if(!tinyg.isBusy()){
                    getNextMirror();
                    cycle = TRAVERSE;
                }
                break;
            case TRAVERSE:
                if(!tinyg.isBusy()){
                    sendGCode("g1f6000");//Set feedrate to 6000
                    string g = modulPositions.at(currentMirror);
                    sendGCode(g);
                    cycle = ENGAGING_SCREW;
                }
                break;
            case ENGAGING_SCREW:
                if(!tinyg.isBusy()){
                    engageScrewCycle();
                    cycle = SCREW_HOME;
                }
                break;
            case SCREW_HOME:
                if(!tinyg.isBusy()){
                    screwHomeCycle();
                    cycle = SCAN_COLORS;
                }
                break;
            case SCAN_COLORS:
                if(!tinyg.isBusy()){
                    scanColors();
                    cycle = GOTO_COLOR;
                }
                break;
            case GOTO_COLOR:
                if(tinyg.isBusy()){
                    updateCloasestColor();
                }else{
                    cout << "Found Color" << endl;
                    goToCloasestColor();
                    cycle = DISENGAGING_SCREW;
                }
                break;
            case DISENGAGING_SCREW:
                if(!tinyg.isBusy()){
                    disengageScrewCycle();
                    cycle = GOTO_ZERO_A;
                }
                break;
            case GOTO_ZERO_A:
                if(!tinyg.isBusy()){
                    gotoZeroA();
                    cycle = REST;
                }
                break;
            default:
                break;
        }
        
    }
}


void commander::getNextMirror(){
    currentMirror++;
    if (currentMirror >= modulPositions.size()) {
        currentMirror = 0;
    }
}
void commander::updateCloasestColor(){
    
}
void commander::goToCloasestColor(){
    string s = "g1a"+ ofToString(cloasestColorAPos) +"f10000";
    if (dryRun) {
        s = "g1a45.2f10000";
    }
    sendGCode(s);
}
void commander::engageScrewCycle(){
    if(!dryRun){
        sendGCode("g1z14a1300f200");//Need Correct Engaging Value!!!!
    }
    else if (dryRun){
        sendGCode("g1z8a720f200");//Need Correct Engagin
    }
}
void commander::disengageScrewCycle(){
    sendGCode("g1z0f3000");
}
void commander::gotoZeroA(){
    sendGCode("g0a0");
}
void commander::screwHomeCycle(){
    if(!dryRun){
        sendGCode("g28.2a0");
    }
}
void commander::scanColors(){
    //    sendGCode("g1a5400f6000");
    if(!dryRun){
        sendGCode("g1a4320f10000");
    }else{
        sendGCode("g1a1000f10000");
    }
}


void commander::draw(){
    if(useTinyG){
        tinyg.draw();
    }
}

void commander::sendGCode(string gCode){
    if (useTinyG) {
        tinyg.sendGcode(gCode);
    }else{
        cout << "TinyG is not connected or deactivated" << endl;
    }
}


void commander::updatePosition(string ID, string posX, string posY ){
    
    int IdI = ofToInt(ID);
    float posXf = ofToFloat(posX);
    float posYf = ofToFloat(posY);
    if(IdI >= modulPositions.size() || IdI < 0){
        cout << "ID is to Big" << endl;
    }else{
        modulPositionsX.at(IdI) = posXf;
        modulPositionsY.at(IdI) = posYf;
    }
    //Save to file
    foutX.open(ofToDataPath("modulPositionsX.txt").c_str());
    if (foutX.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsX.begin();iter != modulPositionsX.end();++iter){
            foutX << (*iter) << endl;
        }
        foutX.close();
    }
    foutY.open(ofToDataPath("modulPositionsY.txt").c_str());
    if (foutY.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsY.begin();iter != modulPositionsY.end();++iter){
            foutY << (*iter) << endl;
        }
        foutY.close();
    }
}


void commander::calcMirrorPosPerModul()
{
    vector<float> tempPosX;
    vector<float> tempPosY;
    mirrorPositionX.clear();
    mirrorPositionY.clear();
    for(int i = 0 ; i < modulPositionsX.size(); i++)
    {
        float originX = modulPositionsX.at(i);
        float originY = modulPositionsY.at(i);
        for (int y = 0; y < 6; y++) {
            for(int x = 0; x < 6; x++){
                tempPosX.push_back(originX + (x * screwGap));
                tempPosY.push_back(originY + (y * screwGap));
            }
        }
        if((i+1)%5 == 0){
            int row = 0;
            for (int y = 0; y < 6; y++)//6 rows
            {
                int skip = 0;
                for(int m = 0; m < 5; m++)// 5 Modul
                {
                    for (int x = 0; x < 6; x++) // get 6 Mirrors
                    {
                        mirrorPositionX.push_back(tempPosX.at(row+x+skip));

                        mirrorPositionY.push_back(tempPosY.at(row+x+skip));
                    }
                    skip += 36;//36 mirrors per modul
                }
                row += 6;
            }
            tempPosX.clear();
            tempPosY.clear();
        }
    }
    int c = 0;
    for(vector<float>::iterator iter = mirrorPositionX.begin(); iter != mirrorPositionX.end();++iter)
    {
        cout << "[" << mirrorPositionX.at(c) << "," << mirrorPositionY.at(c) << "]" ;
        if ((c+1) % 30 == 0) {
            cout << endl;
        }
        c++;
    }
    
}

void commander::updateOffset(string offsetX, string offsetY){
    float offsetXf = ofToFloat(offsetX);
    float offsetYf = ofToFloat(offsetY);
    
    float offSetXDiff = offsetXf - (modulPositionsX.at(0));
    float offSetYDiff = offsetYf - (modulPositionsY.at(0));
    
    for(std::vector<float>::iterator iter = modulPositionsX.begin();iter != modulPositionsX.end();++iter){
        (*iter) += offSetXDiff;
    }
    for(std::vector<float>::iterator iter = modulPositionsY.begin();iter != modulPositionsY.end();++iter){
        (*iter) += offSetYDiff;
    }
    
    foutX.open(ofToDataPath("modulPositionsX.txt").c_str());
    if (foutX.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsX.end();iter != modulPositionsX.begin();--iter){
            foutX << (*iter) << endl;
        }
        foutX.close();
    }
    
    foutY.open(ofToDataPath("modulPositionsY.txt").c_str());
    if (foutY.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsY.begin();iter != modulPositionsY.end();++iter){
            foutY << (*iter) << endl;
        }
        foutY.close();
    }
    calcMirrorPosPerModul();
    
}






