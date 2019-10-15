#include "Cursor.h"

using namespace state;

int Cursor::getCursorX(){
    return cursorX;
}

int Cursor::getCursorY(){
    return cursorY;
}

int Cursor::setCursorX(int newX){
     cursorX = newX;
}

int Cursor::setCursorY(int newY){
     cursorY = newY;
}
