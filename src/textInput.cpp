#include "textInput.h"
#include "ofMain.h"
#include "ofApp.h"


//declaration de toute les fonction ici
//------------------------------------------------------------
textInput::textInput(int _x, int _y, std::string* _adresseToSring, std::string nameOfValue){//constructeru (setup)
	stringToChange = 0;//pointer Of value to change
	stringToChange = _adresseToSring;

    myfont.loadFont("Vollkorn-Regular.ttf", 12); //font

    positionXBloc = _x;//position
    positionYBloc = _y;

    widthMax=150;

    margin = 20;

	title = nameOfValue;    
	titlebBunding = myfont.getStringBoundingBox(title, positionXBloc-100, positionYBloc);


    //Text
	text="E";//check fontsize
    textbounding = myfont.getStringBoundingBox(text, positionXBloc + (titlebBunding.width + 30), positionYBloc);
    heightSizeLine = textbounding.height;
    widthSizeLetter = textbounding.width;

    //Rect
    myRect.x = textbounding.x-(margin/2);
    myRect.y = textbounding.y-(margin/2);
    myRect.height = textbounding.height+margin;
    myRect.width = widthMax;//max sizeBox so max character

	position=0;
	text="";
	focused = true;
}
//------------------------------------------------------------
textInput::~textInput(){//destructeur

}
//------------------------------------------------------------
void textInput::drawText(){

    //clignotement de la ligne noire
	if(focused){
	float timeFrac = 255.0f * sin(3.0f * ofGetElapsedTimef());
		ofSetColor(timeFrac,timeFrac,timeFrac);
		ofSetLineWidth(3.0f);
		ofLine(textbounding.width+ 3 + positionXBloc + (titlebBunding.width + 30), textbounding.y+textbounding.height - heightSizeLine, textbounding.width +3+positionXBloc + (titlebBunding.width + 30), textbounding.y+textbounding.height);
	};

	ofSetColor(2,2,2);
	
	if(focused){
		    ofSetColor(2,2,150);
	}

    ofNoFill();
    textbounding = myfont.getStringBoundingBox(text, positionXBloc, positionYBloc);
	ofSetLineWidth(1);
    ofRectRounded( myRect, 5 );

    ofFill();
    ofSetColor(2,2,2);
    myfont.drawString(text, positionXBloc + (titlebBunding.width + 30),positionYBloc);
    myfont.drawString(title, positionXBloc,positionYBloc);

	if(ofGetMouseX()>myRect.x && ofGetMouseX()<myRect.width+myRect.x && ofGetMouseY()>myRect.y && ofGetMouseY()<myRect.height+myRect.y && ofGetMousePressed()){
		focused = true;
	}
	else if(ofGetMousePressed()){
		focused = false;
	};
}
//------------------------------------------------------------
void textInput::typeKey(int key) {
	if(focused){
		//add charachterf
		//cout<<textbounding.width << "  "<< widthMax<<"  "<<myRect.width<<endl;
		if((textbounding.width + widthSizeLetter+ (margin/2)) < widthMax){
			if (key >=32 && key <=126) {
				text.insert(text.begin()+position, key);
				position++;
			}
		}

		if (key==OF_KEY_RETURN) {//send data
			*stringToChange = text;
			text = "";
			position = 0;
            
		}

		if (key==OF_KEY_BACKSPACE) {
			if (position>0) {
				text.erase(text.begin()+position-1);
				--position;
			}
		}

		if (key==OF_KEY_DEL) {
			if (text.size() > position) {
				text.erase(text.begin()+position);
			}
		}

		if (key==OF_KEY_LEFT)
			if (position>0)
				--position;

		if (key==OF_KEY_RIGHT)
			if (position<text.size()+1)
				++position;

		//for multiline:
		cursorx = cursory = 0;

		//Saut a la ligne
			//	for (int i=0; i<position; ++i) {
			//		if (*(text.begin()+i) == '\n') {
			//			++cursory;
			//			cursorx = 0;
			//		} else {
			//			cursorx++;
			//		}
			//	}
		//ifIsInputfiel
		for (int i=0; i<position; ++i) {
				cursorx++;
		}
	}
}
