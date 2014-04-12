#include "..\includes\cGameStateObject.h"

cGameStateObject::cGameStateObject(){
}
cGameStateObject::~cGameStateObject(){
}
void cGameStateObject::initialize(){
}
void cGameStateObject::activate(){
}


cGameStateObject* cGameStateObject::update(double timeDifference){
  return 0;

}

void cGameStateObject::render(){

}
//void cGameStateObject::render(HWND &hWnd,   HDC &hdc){}

void cGameStateObject::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){


}

void cGameStateObject::deactivate(){
}
void cGameStateObject::resume(){
}
void cGameStateObject::pause(){
}
void cGameStateObject::save(){
}

void cGameStateObject::addTransitionEvent(int event, cGameStateObject* p_Next){

}
