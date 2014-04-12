/**********************************************************************************
 main.cpp - Written by Chuck Bolin, October 2007
 Starting point for Climbing the Citadel
 Remember to link:
   d3d9.lib, d3dx9.lib winmm.lib
**********************************************************************************/
#include <Windows.h>
#include "..\includes\Graphics.h"
#include <string>
#include <sstream>
#include "..\resource.h"
#include "..\includes\CTimer.h"
#include "..\includes\Keystatus.h"
#include "..\includes\CAudioManager.h"
#include "..\includes\cGameStateObject.h"
#include "..\includes\CGameStatePlay.h"
#include "..\includes\cGameStateQuit.h"
#include "..\includes\cGameStateIntro.h"
#include "..\includes\cGameStateMain.h"
#include "..\includes\cGameStateScore.h"
#include "..\includes\cGameStateHelp.h"
#include "..\includes\Log.h"
#include "..\includes\CFileReader.h"
#include "..\includes\CSprite.h"
#include "..\includes\CINIReader.h"
#include "..\includes\CGlobal.h"

// Memory leak checker
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//objects
CDXGraphics g_con;            //wrapper for DirectX 
CTimer g_Timer; 
CTimer g_LoopTimer;
CTimer g_FPS_Timer;
CAudioManager g_Audio;        //wrapper for sounds and music
CSprite g_Sprite;             //stores data for all sprites
Log g_Log;                    //logging of data
CINIReader g_Config;          //reads and stores INI file data
CGlobal g_Global;

//variables
HWND g_hWnd;      //Handle to the window the app is running in
BOOL g_bRunning;  //Always set when app is running. App will exit if flag becomes false
POINTS g_mouse; //Mouse g_mouse
bool g_bLeftMouseDown = false;
bool g_bRightMouseDown = false;
int g_SecondCount = 0;
int g_FPS_Counter;
std::string	 g_strFPS  = "";		// SMJ - 11/13/2007 I hate globals!
//const bool	g_WindowedMode				= true;

//manages game states
cGameStateObject* g_pStatePlay = new CGameStatePlay;
cGameStateObject* g_pStateHelp = new cGameStateHelp;
cGameStateObject* g_pStateMain = new cGameStateMain;
cGameStateObject* g_pStateIntro = new cGameStateIntro;
cGameStateObject* g_pStateScore = new cGameStateScore;
cGameStateObject* g_pStateQuit = new cGameStateQuit;
cGameStateObject* g_pCurrent = 0;
cGameStateObject* g_pNext = 0;
  
// Function Prototypes         
int MakeWindow(HINSTANCE hInstance, bool bWindowedMode);
LRESULT CALLBACK WindowProcedure(HWND hWindow,UINT uMessage,WPARAM wParam,LPARAM lParam);
void Shutdown();

//The program begins here.
/************************************************************************************************
  WinMain (...)
************************************************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Always perform a leak check just before app exits.
	int nDbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	nDbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(nDbgFlags);



	g_Log.Write("***********************************************");
  g_Log.Write("Climbing the Citadel...Program Start");
  g_Log.WriteDate();
  g_Log.WriteTime();
  g_Log.Write("***********************************************");

  //***********************************************************************
  //read config.ini file
  // C O N F I G . I N I
  //***********************************************************************
  g_Log.Write("************ Config.ini ************");
  g_Config.LoadFile("config.ini");
  std::string sValue;
  std::string sParameter;
  int nParameterValue;
  if(g_Config.IsValid() == true){
    g_Log.Write("Valid config.ini file");
    g_Log.Write("Numbers of lines in config file",g_Config.GetNumberOfLines());

    for(int j=0; j< g_Config.GetNumberOfLines();j++){
      sParameter = g_Config.GetTerm(g_Config.GetLineFromFile(j),1);
      sValue = g_Config.GetTerm(g_Config.GetLineFromFile(j),2);
      nParameterValue =  atoi(sValue.c_str());

      g_Log.Write(sParameter, nParameterValue);
      
      
      //load g_Global object with config.ini data
      if(sParameter == "WindowedMode" && nParameterValue == 1)
        g_Global.g_WindowedMode = true;
      else if(sParameter == "WindowedMode" && nParameterValue == 0)
        g_Global.g_WindowedMode = false;
      else if(sParameter == "ScreenWidth" && nParameterValue > 0)
        g_Global.g_screenWidth = nParameterValue;
      else if(sParameter == "ScreenHeight" && nParameterValue > 0)
        g_Global.g_screenHeight = nParameterValue;
      else if(sParameter == "GameLevel" && nParameterValue > 0 && nParameterValue < 6)
        g_Global.g_GameLevel = nParameterValue;
      else if(sParameter == "GameSnow" && nParameterValue == 1)
        g_Global.g_bGameSnow = true;
      else if(sParameter == "GameSnow" && nParameterValue == 0)
        g_Global.g_bGameSnow = false;
      else if(sParameter == "GameAudio" && nParameterValue == 1)
        g_Global.g_bGameAudio = true;
      else if(sParameter == "GameAudio" && nParameterValue == 0)
        g_Global.g_bGameAudio = false;
      else if(sParameter == "FrameRate" && nParameterValue == 1)
        g_Global.g_bDisplayFramerate = true;
      else if(sParameter == "FrameRate" && nParameterValue == 0)
        g_Global.g_bDisplayFramerate = false;
    }
  } 
  g_Log.Write("Reading Config.ini file is complete!");
	// Break on specific memory allocation number
	//_CrtSetBreakAlloc(175);

  MSG msg;    
  g_bRunning = true;//Start program running

  //Make a window and initialize DirectX in windowed mode
  MakeWindow(hInstance, g_Global.g_WindowedMode);

  //map game state information
  g_pStateIntro->addTransitionEvent(GO_MAIN, g_pStateMain);
  g_pStateMain->addTransitionEvent(GO_PLAY, g_pStatePlay);
  g_pStateMain->addTransitionEvent(GO_HELP, g_pStateHelp);
  g_pStateMain->addTransitionEvent(GO_QUIT, g_pStateQuit);
  g_pStateScore->addTransitionEvent(GO_MAIN, g_pStateMain);
  g_pStatePlay->addTransitionEvent(GO_MAIN, g_pStateMain);
  g_pStateHelp->addTransitionEvent(GO_MAIN, g_pStateMain);
  g_Log.Write("************ Game states ************");
  g_Log.Write("g_pStateIntro",g_pStateIntro);
  g_Log.Write("g_pStateMain", g_pStateMain);
  g_Log.Write("g_pStatePlay", g_pStatePlay);
  g_Log.Write("g_pStateScore",g_pStateScore);
  g_Log.Write("g_pStateHelp", g_pStateHelp);

  g_Timer.initialize();
  g_LoopTimer.initialize();
  g_FPS_Timer.initialize();
  g_con.InitD3D (g_hWnd, g_Global.g_WindowedMode);

  //***********************************************************************
  //load textures from graphics file
  // G R A P H I C F I L E S . D A T
  //***********************************************************************
  g_Log.Write("************ Graphicfiles.dat ************");
  CFileReader* cfr = new CFileReader;
  cfr->LoadFile("data\\graphicfiles.dat");
  int fileNumber;
  std::string fileName;
  if(cfr->IsValid()== true){
    g_Log.Write("Numbers of lines in file",cfr->GetNumberOfLines());
    for(int j=0; j< cfr->GetNumberOfLines();j++){
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),1);
      fileNumber = atoi(sValue.c_str());
      sValue = cfr->GetTerm(cfr->GetLineFromFile(j),2);
      fileName = "assets\\artwork\\";
      fileName = fileName + sValue;
      //g_Log.Write("Loaded file",fileName);
      g_Log.Write(fileName, fileNumber);
      g_con.LoadTexture(fileName, fileNumber);
    }
  } 
  else{
    g_Log.Write("ERROR****************** Failure to load textures (graphicfiles.dat)");
    delete cfr;
    g_con.CloseD3D();
    g_Log.Write("DirectX closed!");
    g_Audio.KillAll();
    ::Sleep(2000);
    delete g_pStatePlay;
    delete g_pStateQuit;
    delete g_pStateHelp;
    delete g_pStateMain;
    delete g_pStateIntro;
    delete g_pStateScore;
    g_Log.Write("Game state pointers deleted!");
    g_Log.Write("Terminating program!");
    return 0;
  }
  
  //***********************************************************************
  //load sprite data
  // S P R I T E S . D A T
  //***********************************************************************
  g_Log.Write("************ Sprites.dat ************");
  cfr->LoadFile("data\\sprites.dat");
  GRAPHIC_IMAGE gi;
  if(cfr->IsValid()== true){
    //g_Log.Write("Valid data file for sprites.dat");
    g_Log.Write("Numbers of lines in file",cfr->GetNumberOfLines());
    for(int j=0; j< cfr->GetNumberOfLines();j++){

      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),2);
      gi.fileID = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),3);
      gi.sourceX = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),4);
      gi.sourceY = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),5);
      gi.width = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),6);
      gi.height = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),7);
      gi.rotationX = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),8);
      gi.rotationY = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),9);
      gi.scale = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),10);
      gi.alpha = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),11);
      gi.rotationAngle = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),12);
      gi.name = sValue;
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),13);
      gi.layer = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),14);
      gi.frameMax = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),15);
      gi.updateInterval = (double)atof(sValue.c_str());
      gi.frameCount = 0;
      gi.time = 0;
      gi.enable = true;

      //calc slope if it is a snowwedge
      gi.slope = 0;
      if(gi.width == 128){
        gi.slope = (float)(gi.rotationX - gi.rotationY)/128.0f;
      }
      g_Sprite.AddSprite(gi);
    }
  }
  else{
    g_Log.Write("ERROR****************** Failure to sprite data (sprites.dat)");
    delete cfr;
    g_Log.Write("Terminating program!");
    return 0;
  }

  //used for framerate display
  std::ostringstream oss;
  std::string sFramerate;
  std::string sText;
  int nDisplayFPSCount = 0;

  //initialize audio manager
  if(g_Global.g_bGameAudio == true){
    g_Log.Write("Loading audio clip...");
    if(g_Audio.IsValidSound())
      g_Log.Write("Audio system is okay!");
    else
      g_Log.Write("Audio system failure!");

    g_Audio.LoadSample(0, "assets\\sounds\\windy.mp3");
    g_Audio.LoadSample(1, "assets\\sounds\\yodel.wav");
    g_Audio.LoadSample(2, "assets\\sounds\\intro.mp3");
    g_Audio.LoadSample(3, "assets\\sounds\\main_loop.mp3");
    
    for(int i = 0; i < g_Audio.Size(); i++){
      g_Log.Write(g_Audio.GetFilename(i), i); //write filename and file index to log file
    }
  }
  else{
    g_Log.Write("Audio disabled");
  }

  g_Log.Write("Main game loop entered!");
  ::ShowCursor(false); 
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Version Number
  g_strFPS = "Climbing the Citadel, v. 0.061, Final Entry";
  ::SetWindowText(g_hWnd, g_strFPS.c_str());
  double timeDelta;

  //Main application loop
  //*******************************************************
  while (g_bRunning)
  {
    //Handle messages
    if (PeekMessage (&msg,NULL,0,0,PM_REMOVE)){
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }        
    else{}
      timeDelta = g_LoopTimer.getTimeDifference();
      g_pNext = g_pCurrent->update(timeDelta);
	    if(NULL != g_pNext)
	    {
        g_pCurrent->deactivate();
        g_pCurrent = g_pNext;
        g_pCurrent->activate();
      }
     
	// Render our current game object
    g_pCurrent->render();

    nDisplayFPSCount++;

    //call each second
    if(g_FPS_Timer.getTimer(1)){
      g_FPS_Counter++;
      oss.str(""); //clear oss
      oss << (int) nDisplayFPSCount;
      if(g_FPS_Counter > 60){  //call every 60 seconds
        g_Log.Write("FPS",oss.str());
        g_FPS_Counter == 0;
      }

	  // update our FPS string.
	  // This thing can be rendered inside any of the game objects.
      g_strFPS = "FPS " + oss.str();
      g_Global.g_strFPS = "FPS " + oss.str();

      nDisplayFPSCount=0;//reset frame count
    }
  }//while(...
  
  Shutdown();

  return 0;//(int)msg.wParam;  //returns exit code to operating system
} 
  
/*************************************************************************
Window Procedure - Process Windows events

*************************************************************************/
LRESULT CALLBACK WindowProcedure (HWND g_hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    //Check message
    switch(uMessage)
    {

    case WM_CREATE:
      g_pStateIntro->initialize();
      g_pStateMain->initialize();
      g_pStatePlay->initialize();    
      g_pStateQuit->initialize();
      g_pCurrent = g_pStateIntro;
      g_pStateIntro->activate();
      g_Log.Write("Game states initialized!");
      g_Log.Write("Game State = Intro");
      return 0;

	  case WM_CLOSE:
		  g_bRunning = false;
		  break;

    case WM_DESTROY:
  	  PostQuitMessage(0);
      break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
      return 0;
    
    }
   // return 0;
    return DefWindowProc(g_hWnd,uMessage,wParam,lParam);
}

/*************************************************************************
MakeWindow - Creates window
*************************************************************************/
int MakeWindow (HINSTANCE hInstance, bool bWindowedMode)
{
    WNDCLASSEX wc;
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));   //NULL;//(HCURSOR)IDC_CROSS;
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "CTC";
    wc.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)); 
    
    if(!RegisterClassEx(&wc))
        return false;
    

    //screen data
    int cxScreen, cyScreen;
	  cxScreen = ::GetSystemMetrics(SM_CXMAXIMIZED);
	  cyScreen = ::GetSystemMetrics(SM_CYMAXIMIZED);
    if(g_Global.g_screenWidth < 1 || g_Global.g_screenWidth > cxScreen)
      g_Global.g_screenWidth = cxScreen;   
    if(g_Global.g_screenHeight < 1 || g_Global.g_screenHeight > cyScreen)
      g_Global.g_screenHeight = cyScreen;
    

    //Create window
    g_hWnd = CreateWindowEx (NULL, "CTC", " ",
        g_Global.g_WindowedMode ? WS_BORDER | WS_CAPTION | WS_SYSMENU : WS_EX_TOPMOST,
		0,								// x position
		0,								// y position
		g_Global.g_WindowedMode ? g_Global.g_screenWidth : CW_USEDEFAULT,	// width
		g_Global.g_WindowedMode ? g_Global.g_screenHeight : CW_USEDEFAULT,	// height
    NULL, NULL, hInstance, NULL);
    
    
    if (!g_hWnd){//Make sure window was created properly
      g_Log.Write("Window failed to create!");
      return false;
    }
    g_Log.Write("Window creation successful!");

  	// Show the window!
    ShowWindow( g_hWnd, SW_SHOW );
    UpdateWindow( g_hWnd );

	  // Read the actual screen size
	  RECT rc;
    ::GetClientRect( g_hWnd, &rc );
	  int ViewWidth, ViewHeight;
    ViewWidth  = rc.right - rc.left;
    ViewHeight = rc.bottom - rc.top;
    g_con.SetScreenWidthHeight(ViewWidth, ViewHeight);//ViewWidth, ViewHeight);
    g_Log.Write("WIDTH",g_con.GetScreenWidth());
    g_Log.Write("HEIGHT", g_con.GetScreenHeight());

    return true;//Success
}


void Shutdown()
{
  g_Log.Write("************ Shutting Down ************");
	// Properly shut down the current game object first
	g_pCurrent->deactivate();

	delete g_pStatePlay;
  delete g_pStateQuit;
  delete g_pStateHelp;
  delete g_pStateMain;
  delete g_pStateIntro;
  delete g_pStateScore;
  g_Log.Write("Game state pointers deleted!");

	// Release D3D
	g_con.CloseD3D();//Close DirectX
	g_Log.Write("DirectX closed!");
	g_Audio.KillAll();
  ::ShowCursor(false);
	//::Sleep(2000);
	//::ShowCursor(true); //turn mouse cursor back on
	g_Log.Write("Audio sounds are all killed!");
	g_Log.Write("*************************************");
	g_Log.Write("Program ended!");
	g_Log.Write("*************************************");

	// Kill the main window
	DestroyWindow( g_hWnd );
	g_hWnd = NULL;
}
