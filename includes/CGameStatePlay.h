#ifndef CGAMESTATEPLAY_H
#define CGAMESTATEPLAY_H
#include <vector>
#include <sstream>
#include "CGameStateObject.h"
#include <Windows.h>
#include "Graphics.h"
#include "CAudioManager.h"
#include "Keystatus.h"
#include "CPhysics.h"
#include "CTimer.h"
#include "Log.h"
#include "CFileReader.h"
#include "CSprite.h"
#include "CSnow.h"
#include "CGlobal.h"
#include "CTile.h"
#include "CPlayer.h"
#include "CThing.h"
#include "CGame.h"

class CGameStatePlay: public cGameStateObject{
public:
  CGameStatePlay(void);
  ~CGameStatePlay(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference);
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void InitializeNextLevel();
  void addLevel(int level);
  void LoadLevel(int level);
  
private:

  //transitions data for state changes
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;

  CPhysics m_Physics;
  CTimer m_LevelTimer;  //runs when level is complete
  int m_LevelCounter;   //used for 3 second timer between levels
  CTimer m_winTimer;
  CTimer m_secondTimer;
  CTimer m_drawTrackTimer;
  CTimer m_enablePlay;
  bool m_bEnabled;
  
  CTimer m_gameTimer;  //used for each level
  int m_gameTimerCount;

  bool m_bDataOneShot;
  bool m_bKeyTPressed;
  int m_EndLevel;  
  int m_secondCount;

  int m_timeRemaining;
  bool m_gameOver;
  bool m_validFile;

  //used for pause feature
  bool m_bPaused;
  bool m_bPauseReleased;
  CTimer m_pauseTimer;
  int m_pauseCount;
  bool m_bOneshot;
  bool m_bWinner;
  
  //positon of game world
  int m_locationX;//this is game world position mapped to the center of the screen
  int m_locationY;
  int m_startX;
  int m_startY;
  int m_endX;//defines end of current level in X,Y position
  int m_endY;
  std::vector<GAME_OBJECT> m_object;

  //used for testing screenshot capture
  CTimer m_screenCapture;
  bool m_bCaptureScreen;

  CSnow m_snow;
  CPlayer m_Player;
  int m_GameLevel;
  CThing m_Thing;
  CGame m_Game;

  int m_levelX; //stores X,Y of top-left of Level sprite
  int m_levelY;
  int m_healthX;//stores X,Y of top-left of Health sprite
  int m_healthY; 
  int m_scoreX; //stores X,Y of top-left of Score sprite
  int m_scoreY;
  int m_elevation;
  bool m_bFirstPassWin;

};

#endif