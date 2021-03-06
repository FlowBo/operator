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
    
    fin.open( ofToDataPath("modulPositionsY.txt").c_str() );
    while(fin != NULL)
    {
        string str;
        getline(fin, str);
        float strToFloat = ofToFloat(str);
        modulPositionsY.push_back(strToFloat);
    }
    fin.close();
    modulPositionsY.erase(modulPositionsY.end()-1);
    
    
    fin.open( ofToDataPath("modulPositionsX.txt").c_str() );
    while(fin != NULL)
    {
        string str;
        getline(fin, str);
        float strToFloat = ofToFloat(str);
        modulPositionsX.push_back(strToFloat);
    }
    fin.close();
    modulPositionsX.erase(modulPositionsX.end()-1);
    
    fin.open( ofToDataPath("screwGap.txt").c_str() );
    while(fin!=NULL)
    {
        string str;
        getline(fin, str);
        float strToFloat = ofToFloat(str);
        modulPositionsY.push_back(strToFloat);
    }
    fin.close();
    
//    fin.open( ofToDataPath("offset.txt").c_str() );
//    while(fin!=NULL)
//    {
//        string str;
//        getline(fin, str);
//        float strToFloat = ofToFloat(str);
//        offset.push_back(strToFloat);
//    }
//    fin.close();
//    std::ostringstream s;
//    s << "G10L2P2X" << offset.at(0) << "Y" << offset.at(1);
//    sendGCode(s.str());
    
    calcMirrorPosPerModul();
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
//        tinyg.printStatus();
    }
    
}
void commander::start(){
    fin.open( ofToDataPath("offset.txt").c_str() );
    while(fin!=NULL)
    {
        string str;
        getline(fin, str);
        float strToFloat = ofToFloat(str);
        offset.push_back(strToFloat);
    }
    fin.close();
    std::ostringstream s;
    s << "G10L2P2X" << offset.at(0) << "Y" << offset.at(1);
    sendGCode(s.str());
    sendGCode("g55");
    if(!startMachine){
        startMachine = true;
    }else{
        startMachine = true;
    }
}
void commander::printStatus(){
    if(cycle != lastCycle)
    {
        lastCycle = cycle;
        cout << "current cycle: " << cycle <<endl;
    }
    //    cout << tinyg.isBusy() <<endl;
}

void commander::operating(){
    if(!tinyg.isBusy() && startMachine){
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
                    std::ostringstream s;
                    s << "g0x" <<mirrorPositionX.at(currentMirror);
                    s << "y"  << mirrorPositionY.at(currentMirror);

                    sendGCode(s.str());
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
//    currentMirror++;
//    if (currentMirror >= modulPositions.size()) {
//        currentMirror = 0;
//    }
    
    currentMirror = (int)ofRandom(0, 899);
    
}
void commander::updateCloasestColor(){
    
}
void commander::goToCloasestColor(){
    std::ostringstream  s;
    s << "g1a" << cloasestColorAPos  << "f10000";
    if (!dryRun) {
//        s = "g1a45.2f10000";
        sendGCode(s.str());
    }
    
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
    sendGCode("g0z0");
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
    if(!dryRun){
        sendGCode("g1a4320f10000");
    }else{
//        sendGCode("g1a1000f10000");
    }
}


void commander::draw(){
    if(useTinyG){

    }
}

void commander::sendGCode(string gCode){
    if (useTinyG) {
        tinyg.sendGcode(gCode);
    }else{
        ofLog(OF_LOG_WARNING) << "TinyG is not connected or deactivated" << endl;
    }
}


void commander::updateModulPosition(int ID, float posX, float posY ){
    

    if(ID >= modulPositions.size() || ID < 0){
        ofLog(OF_LOG_ERROR) << "ID is out of bound" << endl;
    }else{
        modulPositionsX.at(ID) = posX;
        modulPositionsY.at(ID) = posY;
    }
    //Save to file
    fout.open(ofToDataPath("modulPositionsX.txt").c_str());
    if (fout.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsX.begin();iter != modulPositionsX.end();++iter){
            fout << (*iter) << endl;
        }
        fout.close();
    }
    fout.open(ofToDataPath("modulPositionsY.txt").c_str());
    if (fout.is_open())
    {
        for(std::vector<float>::iterator iter = modulPositionsY.begin();iter != modulPositionsY.end();++iter){
            fout << (*iter) << endl;
        }
        fout.close();
    }
    calcMirrorPosPerModul();
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



float commander::getPosition(char axis){
    
    if(axis == 'x'){
        return tinyg.getXPos();
    }else if (axis == 'y'){
        return tinyg.getYPos();
    }
    else if (axis == 'z'){
        return tinyg.getZPos();
    }
    else if (axis == 'a'){
        return tinyg.getAPos();
    }
    cout << "WRONG CHAR" << endl;
    return;
}

void commander::newModulPos(int idi){
    float x,y;
    x = tinyg.getXPos();
    y = tinyg.getYPos();
    updateModulPosition(idi, x, y);
}

void commander::gotoModul(int ID){
    if(ID >= modulPositions.size() || ID < 0){
        ofLog(OF_LOG_ERROR) << "ID is out of bound" << endl;
    }else{
        sendGCode("g0z0");
        std::ostringstream s;
        s << "g0x" <<  modulPositionsX.at(ID);
        s << "y" <<  modulPositionsY.at(ID);
        sendGCode(s.str());
    }

}

void commander::setScrewGap(){
    ofLog(OF_LOG_WARNING, "make sure you set the gentry pos to to screw (5/0)");
    float x = tinyg.getXPos();
    float gap = x - modulPositionsX.at(0);
    screwGap = x/5;
    fout.open(ofToDataPath("screwGap.txt").c_str());
    if (fout.is_open())
    {
        fout << screwGap << endl;
    }
    fout.close();
    calcMirrorPosPerModul();
}


void commander::setG55(){
    float x = tinyg.getXPos();
    float y = tinyg.getYPos();
    std::ostringstream s;
    s << "G10L2P2X" << x << "Y" << y;
    sendGCode(s.str());
    
    }

