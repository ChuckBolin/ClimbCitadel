//CGlobal.cpp
#include "../includes/CGlobal.h"

CGlobal::CGlobal(){
  g_bLeftMouseDown = false;
  g_bRightMouseDown = false;
  g_SecondCount = 0;
  g_FPS_Counter = 0;
  g_strFPS  = "";	
  g_WindowedMode = true;      //program defaults to windowed mode
  g_screenWidth = -1;
  g_screenHeight = -1;
  g_bGameSnow = false;
  g_bGameAudio = false;
  g_bDisplayFramerate = false;
  g_GameLevel = 1;
}