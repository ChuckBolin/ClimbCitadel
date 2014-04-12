//cGameStateQuit.cpp
#include "..\includes\cgamestatequit.h"
#include "..\includes\keystatus.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern Log g_Log;
extern BOOL g_bRunning;
extern CGlobal g_Global;
extern CSprite g_Sprite; 
extern HWND g_hWnd;

cGameStateQuit::cGameStateQuit(void)
{
}

cGameStateQuit::~cGameStateQuit(void)
{
}

void cGameStateQuit::initialize(){

  m_snow.SetWidthHeight(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
  m_snow.CreateSnow(200);
  m_snow.SetEnvironment(5, 2, 4);

  SCROLL_MSG msg;
  msg.red = 255; msg.green = 255; msg.blue = 255;  
  msg.red = 127; msg.green = 127; msg.blue = 127;  
  msg.msg ="Climbing the Citadel";   m_msg.push_back(msg);
  msg.msg = "Created by";                m_msg.push_back(msg);
  msg.red = 255; msg.green = 0; msg.blue = 0;  
  msg.msg  = "The Team - Byl, Roland and Chuck"; m_msg.push_back(msg);
  msg.msg  = ""; m_msg.push_back(msg);
  msg.msg  = "December 16th, 2007";          m_msg.push_back(msg);
  msg.msg  = ""; m_msg.push_back(msg);
  msg.msg  = ""; m_msg.push_back(msg);
  msg.red = 0; msg.green = 255; msg.blue = 0;  
  msg.msg  = "Official Team Entry";            m_msg.push_back(msg);
  msg.msg  = "Game Challenge IV (Winter Theme)";  m_msg.push_back(msg);
  msg.msg  = "www.gameinstitute.com";     m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.red = 255; msg.green = 255; msg.blue = 0;  
  msg.msg  = "Byl and Chuck would like to thank Grace Bolin for suggesting this game idea";  m_msg.push_back(msg);
  msg.msg  = "and allowing her husband (Chuck) to devote all these hours to the project.";           m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Inspired by the book \"Banner in the Sky\" copyright 1951";                          m_msg.push_back(msg);
  msg.msg  = "Written by James Ramsey Ullman"; m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.red = 0; msg.green = 255; msg.blue = 255;    
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Byl Walker.............. Graphic Artist";     m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Roland Shaw............Musical Composer (polyroly@hotmail.com)";               m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Chuck Bolin..............Programmer/Team Leader"; m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Grace Bolin..............Creative Lead and Q&A";   m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Thanks to Steve Jones for Engineering Support.";   m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Yodeling.............http://www.ilovewavs.com/Effects/People/Yodel.wav";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Wind Sound Effects.....http://www.partnersinrhyme.com/soundfx/Weather.shtml";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Program testers who complained about our program. =)";  m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Gape";     m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Richard Holtom";     m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Jammy";     m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Thanks to the gang at GameInstitute.com for their encouragement,";   m_msg.push_back(msg);
  msg.msg  = "and both their solicited and unsolicited advice. =)";   m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Thanks to Jon Hoult for running the forums at www.gameinstitute.com.";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "";                          m_msg.push_back(msg);
  msg.msg  = "Game Challenge #5 coming January 18th, 2008";                          m_msg.push_back(msg);
  msg.msg  = "See you there.................";                          m_msg.push_back(msg);


  m_timer.initialize(); 
  m_scrollValue = 0;

}

void cGameStateQuit::activate(){
   m_scrollValue = 0;
//   if(g_Audio.IsPlaying(C_AUDIO_MUSIC_CREDITS) == false)
//     g_Audio.PlaySoundClip(C_AUDIO_MUSIC_CREDITS,0);

  if(g_Audio.IsPlaying(1) == true)
    g_Audio.StopSoundClip(1);
  if(g_Audio.IsPlaying(2) == true)
    g_Audio.StopSoundClip(2);   
  if(g_Audio.IsPlaying(3) == false)
    g_Audio.PlaySoundClip(3,0); //intro

}

cGameStateObject* cGameStateQuit::update(double timeDifference){
  m_event = GO_NO_WHERE;
  static double timeDiffTotal = 0;

  if((keyDown(VK_SPACE)==true  && ::GetActiveWindow() == g_hWnd) && m_scrollValue > 50){
//    if(g_Audio.IsPlaying(C_AUDIO_MUSIC_CREDITS) == true)
//     g_Audio.StopSoundClip(C_AUDIO_MUSIC_CREDITS);
//    else{
      m_event = GO_QUIT;
      g_bRunning = false;
//    }
  }

  //tracks time
  timeDiffTotal += timeDifference;
  m_snow.Update(timeDifference);//Difference);
  if(timeDiffTotal > 0.05){

    timeDiffTotal = 0;
    m_scrollValue += 2;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateQuit::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 60, 0);
  g_con.BeginDrawing();
/*
  GRAPHIC_IMAGE gi;
  gi.fileID = GF_SPLASH;
  gi.sourceX = 0;
  gi.sourceY = 0;
  gi.width = 1273;
  gi.height = 348;
  gi.rotationAngle = 0;
  gi.rotationX = 636;
  gi.rotationY = 174;
  gi.scale = (float)g_con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
  gi.alpha = 120;
*/
  GRAPHIC_IMAGE gi;
  gi = g_Sprite.GetSpriteData(22);
  gi.scale = (float)g_con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
  gi.alpha = 120;
  g_con.RenderGraphicModulate(g_con.GetScreenCenterX() -  ((float)(gi.width/2) * gi.scale) , g_con.GetScreenCenterY() - (gi.height * gi.scale), gi, 255, 255, 255);

  int posY = 0;

  for(int i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(posY < g_con.GetScreenHeight() + 20){
      g_con.Draw2DText(m_msg[i].msg, F_V20, 20, g_con.GetScreenHeight() - posY, m_msg[i].red, m_msg[i].green, m_msg[i].blue);
    }
  }

  //render snowflakes
  VECTOR2D pos;
  gi = g_Sprite.GetSpriteData(8);
  //gi.scale = 0.2;
  for(int i = 0; i < m_snow.Size(); i++){
    pos = m_snow.GetPosition(i);
    gi.scale = m_snow.GetScale(i);
    g_con.RenderGraphic(pos.x, pos.y, gi);
  }

  if(m_scrollValue > 50)
    g_con.Draw2DText("Press SPACEBAR to Quit", F_V20,g_con.GetScreenCenterX() + 150 , g_con.GetScreenCenterY(),255,255,255);
  

  // SMJ - 11/13/2007 Draw FPS on screen
  if(g_Global.g_bDisplayFramerate == true)
    g_con.Draw2DText(g_Global.g_strFPS, F_V20, 0, 0, 0,0,255);

  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateQuit::deactivate(){
}
void cGameStateQuit::resume(){
}
void cGameStateQuit::pause(){
}
void cGameStateQuit::save(){
}

void cGameStateQuit::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}