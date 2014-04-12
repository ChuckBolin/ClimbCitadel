/*
  cGameStateMain.cpp - Controls the user interface and display for the main menu. 

*/
#include "..\includes\cgamestatemain.h"
#include "..\includes\keystatus.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
bool g_bResetGame;
extern POINTS g_mouse;
extern bool g_bLeftMouseDown;
extern bool g_bRightMouseDown;
extern std::string g_strFPS;
extern HWND g_hWnd;

cGameStateMain::cGameStateMain(void)
{
}

cGameStateMain::~cGameStateMain(void)
{
	m_TE.clear();
}

void cGameStateMain::initialize(){
  m_selection = 0;
  m_timer.initialize();
}

void cGameStateMain::activate(){
  ::ShowCursor(false);

}

cGameStateObject* cGameStateMain::update(double timeDifference){
  m_event = GO_NO_WHERE;
  
  POINT pt;
  pt.x = g_Global.g_mouse.x;
  pt.y = g_Global.g_mouse.y;

  if(g_Audio.IsPlaying(1) == true)
    g_Audio.StopSoundClip(1);// .PlaySoundClip(2,1); //intro
  if(g_Audio.IsPlaying(0) == true)
    g_Audio.StopSoundClip(0);// .PlaySoundClip(2,1); //intro
  if(g_Audio.IsPlaying(3) == true)
    g_Audio.StopSoundClip(3);// .PlaySoundClip(2,1); //intro

  if(g_Audio.IsPlaying(2) == false)
    g_Audio.PlaySoundClip(2,1); //intro

  if(m_timer.getTimer(0.1)==true){
    if(keyDown(VK_UP)==true && ::GetActiveWindow() == g_hWnd){
      m_selection--;
      if(m_selection < 0)
        m_selection = 5;
    }

    if(keyDown(VK_DOWN)==true && ::GetActiveWindow() == g_hWnd){
      m_selection++;
      if(m_selection > 5)
        m_selection = 0;
    }
  }

  //must calculate scale first since screen size may change and therefore the menu too
  float scale = (float)g_con.GetScreenWidth() / 1024; //width game is designed to be played

  if(keyDown(VK_RETURN)== true || g_Global.g_bLeftMouseDown == true){
    switch(m_selection){
      case 0://New Mission
        m_event = GO_PLAY;
        g_bResetGame = true;
        break;
      case 1://Replay Level
        //don't change g_gameLevel
        //m_event = GO_PLAY;
        //g_bResetGame = true;
        break;
      case 2://Instructions
        m_event = GO_HELP;
        break;
      case 3://High Score
        //m_event = GO_SCORE;
        break;
      case 4://Config
        break;
      case 5://Quit
        m_event = GO_QUIT;
        break;
    }
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

void cGameStateMain::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();
  
  float scale = (float)g_con.GetScreenWidth() / 1024;
  float scaleY = (float)g_con.GetScreenHeight()/768;
  GRAPHIC_IMAGE gi;

  //Main Menu header
  gi = g_Sprite.GetSpriteData(0);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - (float)(gi.width)/2 * scale, 50 * scaleY, gi);//200

  //Play
  gi = g_Sprite.GetSpriteData(1);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 150 * scaleY + gi.height, gi);//300

  //Reload game
  gi = g_Sprite.GetSpriteData(2);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 225 * scaleY + gi.height, gi);

  //How to play
  gi = g_Sprite.GetSpriteData(3);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 300 * scaleY + gi.height, gi);

  //Richest top 10
  gi = g_Sprite.GetSpriteData(4);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 375 * scaleY + gi.height, gi);

  //Game setup
  gi = g_Sprite.GetSpriteData(5);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 450 * scaleY + gi.height, gi);

  //quit game 
  gi = g_Sprite.GetSpriteData(6);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), 525 * scaleY + gi.height, gi);

  //Selection outline
  gi = g_Sprite.GetSpriteData(7);
  gi.scale = scale;
  g_con.RenderGraphic(g_con.GetScreenCenterX() - ((float)(gi.width/2) * scale), (200 * scaleY)  + (m_selection * 75 * scaleY ), gi);

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    g_con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateMain::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}

void cGameStateMain::deactivate(){

}
void cGameStateMain::resume(){
}
void cGameStateMain::pause(){
}
void cGameStateMain::save(){
}

void cGameStateMain::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}