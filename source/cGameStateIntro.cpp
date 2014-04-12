#include "..\includes\cgamestateintro.h"
#include "..\includes\CGlobal.h"

//objects
extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern Log g_Log;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern HWND g_hWnd;

cGameStateIntro::cGameStateIntro(void){
}

cGameStateIntro::~cGameStateIntro(void)
{
	m_TE.clear();
}

void cGameStateIntro::initialize(){
  m_timer.initialize();
  m_bSplashComplete = false;
  m_red = 0;
  m_green = 0;
  m_blue = 0;

  //initialize snowflakes
  //m_snow.SetWidthHeight(g_con.GetScreenWidth(), g_con.GetScreenHeight());
  m_snow.SetWidthHeight(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
  m_snow.CreateSnow(800);
  m_snow.SetEnvironment(5, 2, 4);
}

void cGameStateIntro::activate(){
}

cGameStateObject* cGameStateIntro::update(double timeDifference){
  m_event = GO_NO_WHERE;
  static bool bOneShot = false;
  static double timeDiffTotal = 0;

  if(g_Audio.IsPlaying(0) == false)
    g_Audio.PlaySoundClip(0,1);//wind

  if(keyDown(VK_SPACE) && m_bSplashComplete == true  && ::GetActiveWindow() == g_hWnd){
    m_event = GO_MAIN;
    ::ShowCursor(true);
  }
    
  //tracks time
  timeDiffTotal += timeDifference;
  
  //responds to mouse and keyboard
  if(timeDiffTotal > 0.01){
    //snowfall
    m_snow.Update(timeDiffTotal);//Difference);
    timeDiffTotal = 0;

  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateIntro::render(){
 
  //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();

  static int nCount = 0;

  if(m_timer.getTimer(0.01) == true){
    nCount += 6;
    if(nCount > 255){ //change to 25 to delay splash
      nCount = 255;
      m_bSplashComplete = true;
    }
    m_red = nCount;
    m_green = nCount;
    m_blue = nCount;
  }

  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(22);
  gi.scale = (float)g_con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
  g_con.RenderGraphicModulate(g_con.GetScreenCenterX() -  ((float)(gi.width * gi.scale)/2) , g_con.GetScreenCenterY() - (gi.height * gi.scale), gi, m_red, m_green, m_blue);

  //render snowflakes
  VECTOR2D pos;
  gi = g_Sprite.GetSpriteData(8);
  for(int i = 0; i < m_snow.Size(); i++){
    pos = m_snow.GetPosition(i);
    gi.scale = m_snow.GetScale(i);
    if(pos.x > 0 && pos.x < g_con.GetScreenWidth() && pos.y > 0 && pos.y < g_con.GetScreenHeight())
      g_con.RenderGraphic(pos.x, pos.y, gi);
  }

  if(m_bSplashComplete == true)
    g_con.Draw2DText("Press SPACEBAR to continue...", F_V20, g_con.GetScreenCenterX() - 150, g_con.GetScreenHeight() - 50, 130, 130, 130);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    g_con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateIntro::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateIntro::deactivate(){
  g_Audio.KillAll();
}
void cGameStateIntro::resume(){
}
void cGameStateIntro::pause(){
}
void cGameStateIntro::save(){
}

void cGameStateIntro::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}