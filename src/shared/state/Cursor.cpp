#include "Cursor.h"

using namespace state;

Cursor::Cursor(){
	cursorX = 0;
	cursorY = 0;
}

int Cursor::getCursorX(){
    return cursorX;
}

int Cursor::getCursorY(){
    return cursorY;
}

void Cursor::setCursorX(int newX){
     cursorX = newX;
}

void Cursor::setCursorY(int newY){
     cursorY = newY;
}
