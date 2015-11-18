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
    fin.open( ofToDataPath("modulPositions.txt").c_str() );
    while(fin!=NULL)
    {
        string str;
        getline(fin, str);
        modulPositions.push_back(str);
    }
    fin.close();
    modulPositions.erase(modulPositions.end());
    cout << "Modular position size: " << modulPositions.size() << endl;
    for(std::vector<string>::iterator iter = modulPositions.begin();iter != modulPositions.end();++iter){
        cout << (*iter) << endl;
    }
    if(useTinyG){
        tinyg.setup();
        cycle = HOME;
    }
}

void commander::update(){
    if(useTinyG){
        tinyg.update();
    }
    
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
    
}
void commander::updateCloasestColor(){

}
void commander::goToCloasestColor(){
    string s = "g1a"+ ofToString(cloasestColorAPos) +"f10000";
    sendGCode(s);
}
void commander::engageScrewCycle(){
    sendGCode("g1z3.5a360f100");//Need Correct Engaging Value!!!!
}
void commander::disengageScrewCycle(){
    sendGCode("g1z0f3000");
}
void commander::gotoZeroA(){
    sendGCode("g1a0f16000");
}
void commander::screwHomeCycle(){
    sendGCode("g28.2a0");
}
void commander::scanColors(){
//    sendGCode("g1a5400f6000");
      sendGCode("g1a540f6000");//!!!!Need Correct Rotation Value!!!!
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


void commander::updatePosition(int ID, string pos){
    
    if(ID >= modulPositions.size() || ID < 0){
        cout << "ID is to Big" << endl;
    }else{
        modulPositions.at(ID) = pos;
    }
    //Save to file
    fout.open(ofToDataPath("modulPositions.txt").c_str());
    if (fout.is_open())
    {
        for(std::vector<string>::iterator iter = modulPositions.begin();iter != modulPositions.end();++iter){
            fout << (*iter) << endl;
        }
        fout.close();
    }
    
}