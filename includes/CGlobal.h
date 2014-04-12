//CGlobal.h
#ifndef CGLOBAL_H
#define CGLOBAL_H
#include <windows.h>
#include <string>

class CGlobal{
public:
  CGlobal();

  HWND g_hWnd;            //Handle to the window the app is running in
  BOOL g_bRunning;        //Always set when app is running. App will exit if flag becomes false
  POINTS g_mouse;         //Mouse g_mouse
  bool g_bLeftMouseDown;
  bool g_bRightMouseDown;
  int g_SecondCount;
  int g_FPS_Counter;
  std::string	 g_strFPS;		// SMJ - 11/13/2007 I hate globals!
  bool g_WindowedMode;
  int g_screenWidth;
  int g_screenHeight;
  bool g_bGameSnow;
  bool g_bGameAudio;
  bool g_bDisplayFramerate;
  int g_GameLevel;
};

#endif