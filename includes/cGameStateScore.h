//cGameStateScore.h
#ifndef GAME_STATE_SCORE_H
#define GAME_STATE_SCORE_H

#include "cgamestateobject.h"
#include "CHighScore.h"
#include "Graphics.h"
#include "CAudioManager.h"
//#include "CAudioConstants.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct HIGH_SCORE{
  std::string name;
  int score;
};

class cGameStateScore : public cGameStateObject{
public:
  cGameStateScore(void);
  ~cGameStateScore(void);
  void initialize();
  void activate();
  cGameStateObject* update(double timeDifference);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;


  std::vector<HIGH_SCORE> m_HighScore;
  std::string convertInteger(int num);
};

#endif
