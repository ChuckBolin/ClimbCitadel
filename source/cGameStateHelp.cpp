//cGameStateHelp.cpp
#include "..\includes\cgamestatescore.h"
#include "..\includes\keystatus.h"
#include "..\includes\CGameStateHelp.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern std::string g_strFPS;
extern CSprite g_Sprite; 

cGameStateHelp::cGameStateHelp(void)
{
}

cGameStateHelp::~cGameStateHelp(void)
{
	m_TE.clear();
}

void cGameStateHelp::initialize(){
}

void cGameStateHelp::activate(){
}


cGameStateObject* cGameStateHelp::update(double timeDifference){
  m_event = GO_NO_WHERE;

  if(keyDown(VK_SPACE)){
    m_event = GO_MAIN;
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }
  return 0;
}

void cGameStateHelp::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}


void cGameStateHelp::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 90, 90, 0);
  g_con.BeginDrawing();

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(105);
  gi.scale = (float)g_con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
  g_con.RenderGraphicModulate(0, 0, gi, 255, 255, 255);//m_red, m_green, m_blue);


  g_con.EndDrawing ();
  g_con.Present();
 }

void cGameStateHelp::deactivate(){
}
void cGameStateHelp::resume(){
}
void cGameStateHelp::pause(){
}
void cGameStateHelp::save(){
}

void cGameStateHelp::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}