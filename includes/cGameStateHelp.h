//cGameStateHelp.h
#ifndef GAME_STATE_HELP_H
#define GAME_STATE_HELP_H

#include "cgamestateobject.h"
#include <iostream>
#include "Graphics.h"
#include "CAudioManager.h"
//#include "CAudioConstants.h"

class cGameStateHelp : public cGameStateObject{
public:
  cGameStateHelp(void);
  ~cGameStateHelp(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference);
  void render();
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  HDC m_hdcBB;  //Device Context for Backbuffer
  HBITMAP m_hBitmap;
  HBITMAP m_hOldSurface;
  HBITMAP m_hSurface;
  RECT m_RC;
  int m_width;
  int m_height;
  HBRUSH m_hBrush;
  HBRUSH m_hBlackBrush;
  HBRUSH m_hOldBrush;
  HBRUSH m_hBlueBrush;
  HBRUSH m_hRedBrush;
  HBRUSH m_hYellowBrush;
  LOGBRUSH m_lb;

};


#endif
