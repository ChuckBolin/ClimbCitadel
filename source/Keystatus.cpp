//Keystatus.cpp
#include "..\includes\Keystatus.h"

bool keyDown(int key)
{
  if(GetAsyncKeyState(key) == 0) //key is not pressed, return false
    return false;
  else                           //key IS pressed, return true 
    return true;
}
