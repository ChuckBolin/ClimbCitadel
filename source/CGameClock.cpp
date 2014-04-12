//CGameClock.cpp
#include "..\includes\CGameClock.h"

CGameClock::CGameClock(){
  m_year[0] = "1880";
  m_year[1] = "1881";
  m_year[2] = "1882";
  m_year[3] = "1883";
  m_year[4] = "1884";
  m_year[5] = "1885";
  m_month[0] = "January";
  m_month[1] = "February";
  m_month[2] = "March";
  m_month[3] = "April";
  m_month[4] = "May";
  m_month[5] = "June";
  m_month[6] = "July";
  m_month[7] = "August";
  m_month[8] = "September";
  m_month[9] = "October";
  m_month[10] = "November";
  m_month[11] = "December";
  m_counter = 0;
  m_monthIndex = 0;
  m_yearIndex = 0;
  m_timer.initialize();
  m_bRunning = false;
}

void CGameClock::Pause(){
  m_bPaused = true;
}

void CGameClock::Resume(){
  m_bPaused = false;
}

void CGameClock::Reset(){
  m_bPaused = true;
  m_counter = 0;
  m_monthIndex = 0;
  m_yearIndex = 0;
  m_bRunning = false;
}

void CGameClock::Start(){
  m_bPaused = false;
  m_bOver = false;
  m_timeScale = C_TIMER_NORMAL;
  m_bRunning = true;
}

void CGameClock::SetMonthYear(int month, int year){
  if(month < 0 || month > 11)
    return;
  if(year < 0 || year > 5)
    return;

  m_monthIndex = month;
  m_yearIndex = year;
}

//********************* update(double timeDifference) *****************8
void CGameClock::update(){
  if(m_bPaused == true)
    return;

  if(m_counter > 3599){
    m_bOver = true;   
    m_bRunning = false;
    return;
  }
 
  if(m_timer.getTimer(1.0) == true){
    m_counter += m_timeScale;
    m_yearIndex = m_counter / 720; //0 to 5
    m_monthIndex = (m_counter - (m_yearIndex * 720)) / 60;
  }
}

std::string CGameClock::GetMonth(){
  if(m_monthIndex < 0 || m_monthIndex > 11)
    return "";

  return m_month[m_monthIndex];  
}

std::string CGameClock::GetYear(){
  if(m_yearIndex < 0 || m_yearIndex > 5)
    return "";

  return m_year[m_yearIndex];
}

bool CGameClock::IsOver(){
  return m_bOver;
}

void CGameClock::SetTimeScale(int ts){
  if(ts < 0) 
    return;

  m_timeScale = ts;
}

int CGameClock::GetCounter(){
  return m_counter;
}