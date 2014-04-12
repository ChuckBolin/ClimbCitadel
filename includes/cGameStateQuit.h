//cGameStateQuit.h
#ifndef GAME_STATE_QUIT_H
#define GAME_STATE_QUIT_H

#include "cgamestateobject.h"
#include <iostream>
#include "Graphics.h"
#include "CAudioManager.h"
#include "Log.h"
#include "keystatus.h"
#include "CGlobal.h"
#include "CSnow.h"

struct SCROLL_MSG{
  std::string msg;
  int red;
  int green;
  int blue;
};

class cGameStateQuit : public cGameStateObject{
public:
  cGameStateQuit(void);
  ~cGameStateQuit(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference);
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
    std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
    int m_event;
    std::vector<SCROLL_MSG> m_msg;
    int m_scrollValue;
    CTimer m_timer;
    CSnow m_snow;
};


#endif
