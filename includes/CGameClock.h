//CGameClock.h
#ifndef CGAMECLOCK_H
#define CGAMECLOCK_H

#include "CTimer.h"

const int C_TIMER_STOPPED = 0;
const int C_TIMER_NORMAL = 1;
const int C_TIMER_FAST = 4;
const int C_TIMER_FAST5 = 20;

class CGameClock{
public:
  CGameClock();
  void Pause();
  void Resume();
  void Reset();
  void Start();
  void SetMonthYear(int month, int year);
  void update();
  std::string GetMonth();
  std::string GetYear();
  bool IsOver();
  void SetTimeScale(int ts);
  int GetCounter();
  bool IsPaused(){return m_bPaused;}
  bool IsRunning(){return m_bRunning;}

private:
  CTimer m_timer;   //1 second update(double timeDifference)s
  int m_counter;    //0 to 3600
  int m_monthIndex; //0 to 11
  int m_yearIndex;  //0 to 6
  bool m_bPaused;   //true if paused
  bool m_bOver;     //true of time has been reached
  std::string m_month[12]; //stores names of months
  std::string m_year[12];  //stores names of years
  int m_timeScale; 
  bool m_bRunning;
};

#endif



