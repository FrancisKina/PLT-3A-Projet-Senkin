#include "Cursor.h"

using namespace state;

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
