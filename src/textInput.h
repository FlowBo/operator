
#ifndef DEF_TEXTINPUT
#define DEF_TEXTINPUT

#include <string>
#include "ofMain.h"

class textInput{
    public:
    //TOUTE LES CLASSE ET VAR PUBLIC
    //bool estVivant();
    void    drawText();
    void	typeKey(int key);
    textInput(int _x, int _y, std::string* _adresseToString, std::string nameOfValue);//positionX, positionY, pointer of value to Change, name of textBox
    ~textInput();

    private:
    //TOUTE LES CLASSE ET VAR PRIVATE
    int positionXBloc;
    int positionYBloc;


    //recte
    ofRectangle myRect;
    ofRectangle textbounding;
	ofRectangle titlebBunding;
    int margin;
    float heightSizeLine;
    float widthSizeLetter;
    std::string	text;
    ofTrueTypeFont myfont;
    int		position;//positionLastCharacter
    int		cursorx, cursory;//ligne clignotente
    int widthMax;
    int maxCharacter;
//    POINT p;
	bool focused;
	std::string *stringToChange;
	std::string title;
};

#endif

