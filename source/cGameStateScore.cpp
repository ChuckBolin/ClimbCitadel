//cGameStateScore.cpp
#include "..\includes\cgamestatescore.h"
#include "..\includes\keystatus.h"
extern CDXGraphics g_con;
extern CAudioManager g_Audio;

cGameStateScore::cGameStateScore(void)
{
}

cGameStateScore::~cGameStateScore(void)
{

}

void cGameStateScore::initialize(){

}

void cGameStateScore::activate(){
}


cGameStateObject* cGameStateScore::update(double timeDifference){
  m_event = GO_NO_WHERE;

  if(keyDown(VK_RETURN)){
    m_event = GO_MAIN;
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }
  return 0;
}

void cGameStateScore::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}

std::string cGameStateScore::convertInteger(int num){
  std::ostringstream oss;
  std::string s;
  oss.str("");
  oss << num;
  s = oss.str();
  return s;
}

void cGameStateScore::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();

  g_con.Draw2DText("High Score", F_V16B, 200,320,255,0, 0);
  //g_con.Draw2DTextValue("Player Direction", g_player.GetTankDir(), F_V16B, 200,340,255,255,0);
        
  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateScore::deactivate(){
}
void cGameStateScore::resume(){
}
void cGameStateScore::pause(){
}
void cGameStateScore::save(){
}

void cGameStateScore::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}