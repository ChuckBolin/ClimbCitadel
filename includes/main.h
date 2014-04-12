/*************************************************************************************************
 * MAIN.H                                                      Copyright (c) 2003 Eamonn Doherty *
 *                                                                                               *
 * Textured quad demo in Direct3D 9                                                              *
 *************************************************************************************************/

#include <Windows.h>
#include "Graphics.h"
#include "Texture.h"

#pragma warning( disable : 4244 ) // conversion from 'int' to 'short', possible loss of data - small number of places 
#pragma warning( disable : 4244 ) // conversion from 'int' to 'short', possible loss of data - small number of places
#pragma warning( disable : 4996 ) // 'function' declared deprecated - gets rid of all those 2005 warnings....


//********************************
//* Global Variables             *
//********************************

HWND hWnd;      //Handle to the window the app is running in
BOOL bRunning;  //Always set when app is running. App will exit if flag becomes false

//********************************
//* Function Prototypes          *
//********************************

//Make a window
int MakeWindow(HINSTANCE hInstance);

//Window procedure
LRESULT CALLBACK WindowProcedure(HWND hWindow,UINT uMessage,WPARAM wParam,LPARAM lParam);