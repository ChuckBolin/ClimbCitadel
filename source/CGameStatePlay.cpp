#include "..\includes\CGameStatePlay.h"
#include "..\includes\keystatus.h"
#include "..\includes\CObject.h"

extern Log g_Log;
extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern CSprite g_Sprite; 
extern CGlobal g_Global;
extern bool g_bResetGame;
extern HWND g_hWnd;

CGameStatePlay::CGameStatePlay(void){
  m_bPaused = false;
  srand(time(0));
}

CGameStatePlay::~CGameStatePlay(void)
{
	m_TE.clear();
}

/*****************************************
 Initializes game state, loads objects to
 be displayed in the game world
*****************************************/
void CGameStatePlay::initialize(){ 
  m_Game.ResetGame();
  m_pauseTimer.initialize();
  m_LevelTimer.initialize();
  m_LevelCounter = 0;
  m_winTimer.initialize(); //updates animation of you win
  m_gameTimer.initialize();  //used for each level
  
}

void CGameStatePlay::activate(){
  static bool activateFirst = false;
  
  if(activateFirst == false || m_Game.GetState() == GAME_START){
    activateFirst = true;
    ::ShowCursor(false);
    m_Game.ResetGame();
    m_gameTimerCount = 0;
    //m_Game.SetLevel(4);
    
    //used for testing screenshot capture
    m_screenCapture.initialize();
    m_bCaptureScreen = false;

    //m_locationY = m_Physics.GetGroundPosition(m_locationX, 5000, 5);
    m_Player.SetCurrentLayer(5);
    m_Player.SetPlayerPosition(m_locationX, m_locationY);
    m_Player.SetBoundary(CWORLD_LEFT_LIMIT, CWORLD_RIGHT_LIMIT);

    //LoadLevel(m_GameLevel);
    LoadLevel(m_Game.GetLevel());

  }
}

//********************************************
//Loads data corresponding to game level
//********************************************
void CGameStatePlay::LoadLevel(int level){
  if(level < 1 || level > 5)
    return;

  //clears stuff out 
  m_Thing.Clear();
  m_Physics.Clear();
  m_snow.Clear();
  m_object.clear();

  //load landscape.dat file..contains trees, rocks, hills, tiles
  
  CFileReader* cfr = new CFileReader;
  switch(level){
  case 1:
    m_locationX = 128;
    m_locationY = 5000;
    cfr->LoadFile("data\\level1.dat");
    g_Log.Write("************ loading game level1.dat ************");
    m_Game.IncrementGameState();
    m_Game.IncrementGameState();
    m_gameTimerCount = 60 + (level * 20);
    break;
  case 2:
    m_locationX = 1;
    m_locationY = 7500;
    cfr->LoadFile("data\\level2.dat");
    g_Log.Write("************ loading game level2.dat ************");
    m_gameTimerCount = 60 + (level * 20);
    break;
  case 3:
    m_locationX = 1;
    m_locationY = 10000;
    cfr->LoadFile("data\\level3.dat");
    g_Log.Write("************ loading game level3.dat ************");
    m_gameTimerCount = 60 + (level * 20);
    break;
  case 4:
    m_locationX = 1;
    m_locationY = 12500;
    cfr->LoadFile("data\\level4.dat");
    g_Log.Write("************ loading game level4.dat ************");
    m_gameTimerCount = 60 + (level * 20);
    break;
  case 5:
    m_locationX = 1;
    m_locationY = 15000;
    cfr->LoadFile("data\\level5.dat");
    g_Log.Write("************ loading game level5.dat ************");
    m_gameTimerCount = 60 + (level * 20);
    break;
  }  
  m_Thing.SetTop(0);
  GAME_OBJECT object;
  CTHING cthing;
  GRAPHIC_IMAGE gi;
  TILE tile;
  std::string sValue;
  std::string sType;

  int index = 0;

  if(cfr->IsValid()== true){
    g_Log.Write("Numbers of lines in file",cfr->GetNumberOfLines());
    for(int j=0; j< cfr->GetNumberOfLines();j++){
      sType = cfr->GetTerm(cfr->GetLineFromFile(j), 1);
      index = atoi(sType.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),2);
      object.x = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),3);
      object.y = atoi(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),4);
      object.scale = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),5);
      object.angle = atof(sValue.c_str());
      sValue =cfr->GetTerm(cfr->GetLineFromFile(j),6);
      object.spriteID = atoi(sValue.c_str());
      
      gi = g_Sprite.GetSpriteData(object.spriteID);

      //these are snow wedges
      if(gi.width == 128){ //this is a tile
        if(gi.layer == 4){
          tile.leftEdge4 = gi.rotationX;
          tile.rightEdge4 = gi.rotationY;
          tile.x4 = object.x;
          tile.y4 = object.y;
          tile.slope4 = (float)(tile.rightEdge4 - tile.leftEdge4)/128.0f;
          object.y -= tile.leftEdge4;
          if(index > -1 && index < 160)
            m_Physics.AddTile(index, gi.layer, tile);
        }
        else if(gi.layer == 5){
          tile.leftEdge5 = gi.rotationX;
          tile.rightEdge5 = gi.rotationY;
          tile.x5 = object.x;
          tile.y5 = object.y;
          tile.slope5 = (float)(tile.rightEdge5 - tile.leftEdge5)/128;
          object.y -= tile.leftEdge5;
          if(index > -1 && index < 160)
            m_Physics.AddTile(index, gi.layer, tile);
        }
        else if(gi.layer == 6){
          //loads initial position
          if(index == 1){
            m_locationX = object.x;
          }

          //stored end of this level X,Y
          if(index == 157){
            m_endX = object.x;
            m_endY = object.y;
          }
          tile.leftEdge6 = gi.rotationX;
          tile.rightEdge6 = gi.rotationY;
          tile.x6 = object.x;
          tile.y6 = object.y;
          tile.slope6 = (float)(tile.rightEdge6 - tile.leftEdge6)/128;
          object.y -= tile.leftEdge6;
          if(index > -1 && index < 160)
            m_Physics.AddTile(index, gi.layer, tile);
        }
        m_object.push_back(object);
      } 
      //these are obstacles
      else{
        cthing.positionX = object.x;
        cthing.positionY = object.y;
        cthing.angle = object.angle;
        cthing.scale = object.scale;
        cthing.spriteID = object.spriteID;
        if(object.spriteID == 92 || object.spriteID == 93 || object.spriteID == 85)
          cthing.layer = 4 + rand() % 3;
        else
          cthing.layer = gi.layer;
        cthing.velocityX = 0;
        cthing.velocityY = 0;
        cthing.status = true;
        cthing.radius = gi.width/4;
        if(object.spriteID == 21)
          cthing.radius = 40;

        cthing.centerX = gi.width/2;
        cthing.centerY = gi.height/2;

        m_Thing.AddThing(cthing);

        if(object.spriteID == 80){  //CP level sprite
          m_levelX = object.x;
          m_levelY = object.y;
        }

        else if(object.spriteID == 79){  //CP level sprite
          m_healthX = object.x;
          m_healthY = object.y;
        }

        else if(object.spriteID == 81){  //CP score sprite
          m_scoreX = object.x;
          m_scoreY = object.y;
        }
      }
    }
  }
  else{
    g_Log.Write("WARNING******************* Invalid landscapees.dat file");
  }
  delete cfr;

  //setup player
  m_Player.SetCurrentLayer(6);
  m_Player.SetPlayerPosition(m_locationX, m_locationY);
  m_Player.SetBoundary(CWORLD_LEFT_LIMIT, CWORLD_RIGHT_LIMIT);

  //setup things
  m_Thing.SetBoundary(CWORLD_LEFT_LIMIT, CWORLD_RIGHT_LIMIT);
  m_Thing.SetGravity(.01);//0.005

  if(g_Global.g_bGameSnow == true){
    m_snow.SetWidthHeight(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
    m_snow.CreateSnow(150);
    m_snow.SetEnvironment(1.4, .2, 2.5);
  }

  m_pauseCount = 0;
  m_bOneshot = false;
  m_Game.SetHealth(100);
  m_bFirstPassWin = false;
}

/*************************************  U P D A T E *****************************************/
cGameStateObject* CGameStatePlay::update(double timeDifference){
  m_event = GO_NO_WHERE;
  static double timeDiffTotal = 0;
  static double thingTimeDiff = 0;
  GRAPHIC_IMAGE gi;
  int spriteID;
  int frameMax  = 0;
  int frameCount  = 0;

  if(g_Audio.IsPlaying(2) == true)//intro
    g_Audio.StopSoundClip(2);//
  if(g_Audio.IsPlaying(0) == false){//enable wind
    g_Audio.PlaySoundClip(0,1);
  }
  if(g_Audio.IsPlaying(3) == false){//main_loop
    g_Audio.PlaySoundClip(3,1);
  }

  if(keyDown(VK_ESCAPE) == true && ::GetActiveWindow() == g_hWnd){
    m_event = GO_MAIN;
    m_Game.ResetGame();
  }

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }

  //pause code
  //*******************************************
  if(m_pauseTimer.getTimer(0.1) == true)    
    m_pauseCount++;

  if(keyDown('P')== true && m_bPaused == false && m_pauseCount > 5 && ::GetActiveWindow() == g_hWnd ){
    m_bPaused = true;
    m_pauseCount = 0;
  }
  else if(keyDown('P') == false && m_bPaused == true && ::GetActiveWindow() == g_hWnd){
    m_bOneshot = true;
  }
  else if(keyDown('P') == true && m_bPaused == true && m_bOneshot == true && m_pauseCount > 5 && ::GetActiveWindow() == g_hWnd){
    m_bOneshot = false;
    m_bPaused = false;
    m_pauseCount = 0;
  }

  if(m_bPaused == true)
    return this;
  if(m_Game.IsOver() == true)
    return this;

  //screencapture test
  if(keyDown('C')==true && ::GetActiveWindow() == g_hWnd)
    m_bCaptureScreen = true;

  //manage level stuff
  if(m_Game.GetState() == GAME_BEGIN_LEVEL2 || 
     m_Game.GetState() == GAME_BEGIN_LEVEL3 ||
     m_Game.GetState() == GAME_BEGIN_LEVEL4 || 
     m_Game.GetState() == GAME_BEGIN_LEVEL5) {
       if(m_LevelTimer.getTimer(1))
         m_LevelCounter++;
       if(m_LevelCounter > 2)
         m_Game.IncrementGameState();
     }
  else
    m_LevelCounter = 0;

  //see if player has reached end of level
  if(m_Physics.GetDistance((float)m_Player.GetPlayerX(),(float)m_Player.GetPlayerY(), m_endX, m_endY) < 60){
    switch(m_Game.GetLevel()){
      case 1:
        m_Game.IncScore((60 + (m_Game.GetLevel() * 20) - m_gameTimerCount) * 50);
        m_Game.IncScore((m_Game.GetHealth()) * 100);
        m_Game.SetState(GAME_END_LEVEL1);
        m_Game.SetLevel(2);
        LoadLevel(2);
        m_LevelTimer.ResetTimer();
        m_LevelCounter = 0;
        m_Game.SetState(GAME_BEGIN_LEVEL2);
        break;
      case 2:
        m_Game.IncScore((60 + (m_Game.GetLevel() * 20) - m_gameTimerCount) * 50);
        m_Game.IncScore((m_Game.GetHealth()) * 100);
        m_Game.SetState(GAME_END_LEVEL2);
        m_Game.SetLevel(3);
        LoadLevel(3);
        m_LevelTimer.ResetTimer();
        m_LevelCounter = 0;
        m_Game.SetState(GAME_BEGIN_LEVEL3);
        break;
      case 3:
        m_Game.IncScore((60 + (m_Game.GetLevel() * 20) - m_gameTimerCount) * 50);
        m_Game.IncScore((m_Game.GetHealth()) * 100);
        m_Game.SetState(GAME_END_LEVEL3);
        m_Game.SetLevel(4);
        LoadLevel(4);
        m_LevelTimer.ResetTimer();
        m_LevelCounter = 0;
        m_Game.SetState(GAME_BEGIN_LEVEL4);
        break;
      case 4:
        m_Game.IncScore((60 + (m_Game.GetLevel() * 20) - m_gameTimerCount) * 50);
        m_Game.IncScore(( m_Game.GetHealth()) * 100);
        m_Game.SetState(GAME_END_LEVEL4);
        m_Game.SetLevel(5);
        LoadLevel(5);
        m_LevelTimer.ResetTimer();
        m_LevelCounter = 0;
        m_Game.SetState(GAME_BEGIN_LEVEL5);
        break;
      case 5:
        m_Game.SetState(GAME_WIN);
        if(m_bFirstPassWin == false){
          m_Game.IncScore((60 + (m_Game.GetLevel() * 20) - m_gameTimerCount) * 50);
          m_Game.IncScore((m_Game.GetHealth()) * 100);
          m_bFirstPassWin = true;
        }
        if(g_Audio.IsPlaying(3) == true){//main_loop
          g_Audio.StopSoundClip(3);// .PlaySoundClip(3,1);
        }    
        if(g_Audio.IsPlaying(0) == false){//wind
          g_Audio.StopSoundClip(0);// .PlaySoundClip(3,1);
        }
        if(g_Audio.IsPlaying(1) == false){
          //g_Audio.SetVolume(1,255);         
          g_Audio.PlaySoundClip(1,1);
        }
        break;
    }
  }

  //only allow updating with game play
  if(m_Game.GetState() == GAME_PLAY_LEVEL1 ||
    m_Game.GetState() == GAME_PLAY_LEVEL2 ||
    m_Game.GetState() == GAME_PLAY_LEVEL3 ||
    m_Game.GetState() == GAME_PLAY_LEVEL4 ||
    m_Game.GetState() == GAME_PLAY_LEVEL5){

    //tracks time
    timeDiffTotal += timeDifference;

    if(timeDiffTotal > 0.025){
      timeDiffTotal = 0;
      if(keyDown(VK_LEFT) && ::GetActiveWindow() == g_hWnd){
        if(g_Audio.IsPlaying(1) == true){ //disable sound if it was started
         g_Audio.StopSoundClip(1);
        }
        g_Sprite.EnableSprite(14);
        g_Sprite.EnableSprite(15);
        g_Sprite.EnableSprite(16);
        g_Sprite.EnableSprite(17);
        m_Player.MovePlayerLeft();
      }
      else if(keyDown(VK_RIGHT)&& ::GetActiveWindow() == g_hWnd){
        if(g_Audio.IsPlaying(1) == true){ //disable sound if it was started
         g_Audio.StopSoundClip(1);
        }
        g_Sprite.EnableSprite(14);
        g_Sprite.EnableSprite(15);
        g_Sprite.EnableSprite(16);
        g_Sprite.EnableSprite(17);
        m_Player.MovePlayerRight();
      }
      else{
        g_Sprite.DisableSprite(14);
        g_Sprite.DisableSprite(15);
        g_Sprite.DisableSprite(16);
        g_Sprite.DisableSprite(17);
      }

      if(keyDown(VK_UP)&& ::GetActiveWindow() == g_hWnd )
        m_Player.MoveBack();
      if(keyDown(VK_DOWN)&& ::GetActiveWindow() == g_hWnd )
        m_Player.MoveForward();
      if(keyDown(VK_HOME)&& ::GetActiveWindow() == g_hWnd)
        m_Player.SetPlayerXPosition(200);
      if(keyDown(VK_END) && ::GetActiveWindow() == g_hWnd)
        m_Player.SetPlayerXPosition(20000);
    }

    m_Player.SetLayerYPositions(m_Physics.GetGroundPosition(m_Player.GetPlayerX(),0, 4),
                                m_Physics.GetGroundPosition(m_Player.GetPlayerX(),0, 5),
                                m_Physics.GetGroundPosition(m_Player.GetPlayerX(),0, 6));
    m_Player.SetSlopes(m_Physics.GetGroundSlope(m_Player.GetPlayerX(), 4),
                        m_Physics.GetGroundSlope(m_Player.GetPlayerX(), 5),
                        m_Physics.GetGroundSlope(m_Player.GetPlayerX(), 6));

    //increment all animated sprites
    for(int i = 0; i < m_Thing.Size();i++){
      m_Thing.SetLayerY(i, m_Physics.GetGroundPosition(m_Thing.GetPositionX(i),0,m_Thing.GetLayer(i)));
      m_Thing.SetSlope(i, m_Physics.GetGroundSlope(i,m_Thing.GetLayer(i)));
      spriteID = m_Thing.GetSpriteID(i);
      if(spriteID > 13 && spriteID < 18){
        if(g_Sprite.IsEnabled(spriteID) == true)
          g_Sprite.IncrementSpriteFrameCount(spriteID, timeDifference);//timeDiffTotal);
      }
      else{
          g_Sprite.IncrementSpriteFrameCount(spriteID, timeDifference);//timeDiffTotal);
      }

      if(m_Thing.GetLayer(i) > 3 && m_Thing.GetLayer(i) < 7){// == 5){
        m_Thing.SetLayerY(i, (m_Physics.GetGroundPosition(m_Thing.GetPositionX(i),0, m_Thing.GetLayer(i))));
        m_Thing.SetSlope(i, (m_Physics.GetGroundSlope(m_Thing.GetPositionX(i),m_Thing.GetLayer(i))));
      }
    }
      
    //update things  
    thingTimeDiff += timeDifference;
    if(thingTimeDiff > 0.01){
      thingTimeDiff = 0;
      m_Thing.Update(timeDifference);
    }

    //decrease remaining time, stop game music 
    if(m_gameTimer.getTimer(1.0)){
      m_gameTimerCount--;
      if(m_gameTimerCount < 1){
        m_gameTimerCount = 0;
        if(m_Game.GetState() < 16){
          m_Game.SetGameOver();
          m_Game.SetState(GAME_OVER);
         if(g_Audio.IsPlaying(3) == true){//main_loop
           g_Audio.StopSoundClip(3);// .PlaySoundClip(3,1);
  }
        }
      }
    }

    m_Player.Update(timeDiffTotal);

    //disable things if contact player
    //collision detection
    //////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    //////////////////////////////////////////////
    ///////////////////////////////////
    for(int i = 0; i < m_Thing.Size();i++){
      if(m_Thing.IsAlive(i) == true){

        //check collision with upper half of body
        if(m_Thing.GetSpriteID(i) != 19 && (m_Thing.GetSpriteID(i) < 94 || m_Thing.GetSpriteID(i) > 118) && m_Thing.GetSpriteID(i) &&
          m_Physics.GetDistance(m_Thing.GetPositionX(i) + (float)m_Thing.GetCenterX(i)* m_Thing.GetScale(i) , m_Thing.GetPositionY(i) + (float)m_Thing.GetCenterY(i) * m_Thing.GetScale(i) ,
          m_Player.GetPlayerX(), m_Player.GetPlayerY() - 75) <  m_Thing.GetRadius(i)* m_Thing.GetScale(i) + 25){
          m_Thing.Disable(i);
          if(m_Thing.GetSpriteID(i) == 18)//wreath
            m_Game.IncScore(1000);
          else if(m_Thing.GetSpriteID(i) == 121)//present
            m_Game.IncScore(2000);
          else if(m_Thing.GetSpriteID(i) == 119)//present
            m_Game.IncScore(3000);
          else{
            if(m_Game.GetLevel() < 4)
              m_Game.DecHealth(2);
            else
              m_Game.DecHealth(3);
          }
        }
        //check collision with lower half of body
        else if(m_Thing.GetSpriteID(i) != 19 &&  (m_Thing.GetSpriteID(i) < 94 || m_Thing.GetSpriteID(i) > 118) && m_Thing.GetSpriteID(i) &&
          m_Physics.GetDistance(m_Thing.GetPositionX(i) + (float)m_Thing.GetCenterX(i)* m_Thing.GetScale(i) , m_Thing.GetPositionY(i) + (float)m_Thing.GetCenterY(i) * m_Thing.GetScale(i) ,
          m_Player.GetPlayerX(), m_Player.GetPlayerY() - 25) <  m_Thing.GetRadius(i)* m_Thing.GetScale(i) + 25){
          m_Thing.Disable(i);  
          if(m_Thing.GetSpriteID(i) == 18)//wreath
            m_Game.IncScore(1000);
          else if(m_Thing.GetSpriteID(i) == 121)//present
            m_Game.IncScore(2000);
          else if(m_Thing.GetSpriteID(i) == 119)//present
            m_Game.IncScore(3000);
          else{
            if(m_Game.GetLevel() < 4)
              m_Game.DecHealth(2);
            else
              m_Game.DecHealth(3);
          }
        }
      }
    }

    //check health status
    if(m_Game.GetHealth() < 1){
      m_Game.SetGameOver();
      m_Game.SetState(GAME_OVER);
      if(g_Audio.IsPlaying(3) == true){//main_loop
        g_Audio.StopSoundClip(3);// .PlaySoundClip(3,1);
     }
   }

    //snowfall
    m_snow.Update(timeDiffTotal);
  }//update during game..not game win

  //animate winning sprite dude
  else if(m_Game.GetState() == GAME_WIN){
    //int i = 122;
   //for(int i = 122; i < 123;i++){
      //m_Thing.SetLayerY(i, m_Physics.GetGroundPosition(m_Thing.GetPositionX(i),0,m_Thing.GetLayer(i)));
      //m_Thing.SetSlope(i, m_Physics.GetGroundSlope(i,m_Thing.GetLayer(i)));
      //spriteID = m_Thing.GetSpriteID(i);
      //if(spriteID > 13 && spriteID < 18){
        //if(g_Sprite.IsEnabled(122) == true)
          g_Sprite.IncrementSpriteFrameCount(122, timeDifference);//timeDiffTotal);
      //}
      //else{
      //    g_Sprite.IncrementSpriteFrameCount(spriteID, timeDifference);//timeDiffTotal);
      //}

      //if(m_Thing.GetLayer(i) > 3 && m_Thing.GetLayer(i) < 7){// == 5){
      //  m_Thing.SetLayerY(i, (m_Physics.GetGroundPosition(m_Thing.GetPositionX(i),0, m_Thing.GetLayer(i))));
      //  m_Thing.SetSlope(i, (m_Physics.GetGroundSlope(m_Thing.GetPositionX(i),m_Thing.GetLayer(i))));
      //}
    //}


  }


  return 0;
}

/******************************************************************************************/
/******************************************************************************************/
/************************************** R E N D E R ***************************************/
/******************************************************************************************/
/******************************************************************************************/
void CGameStatePlay::render(){

  int dx = 0; //used to convert object coordinates to screen coordinates
  int dy = 0;
  GRAPHIC_IMAGE gi;
  HRESULT hr;
  float playerAngle;
  int lastHeight = 0; //used for adding hanging icicles
  int skyOffsetY = 0; //moves sky mountain
  int skyOffsetX = 0;
  std::ostringstream oss;
  static int m_winFrame = 0;//used to animate You Win

  //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 30, 0);
  g_con.BeginDrawing();

  //render tile ONLY if it is viewable
  int leftBoundary = m_Player.GetPlayerX() - g_con.GetScreenWidth()/2 - 512;
  int rightBoundary = m_Player.GetPlayerX() + g_con.GetScreenWidth()/2 + 512;
  int topBoundary = m_Player.GetPlayerY() - g_con.GetScreenHeight()/2 - 512;
  int bottomBoundary = m_Player.GetPlayerY() + g_con.GetScreenHeight()/2 + 512;

  // 000000000000 1111111111111 000000000000000 111111111111111111
  //render things - fixed to screen
  for(int i = 0; i < m_Thing.Size(); i++){
    if(m_Thing.GetLayer(i) <  2){

      gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
      gi.scale = (float)g_con.GetScreenWidth() / (float)gi.width;// 1273;// .75;
      gi.rotationAngle = m_Thing.GetAngle(i);
      if(m_Thing.GetSpriteID(i) == 25){ //sky
        skyOffsetY = 280 - (2500 + (m_Game.GetLevel() * 2500) - m_Player.GetPlayerY())/50;
        skyOffsetX = m_Player.GetPlayerX()/100;
        gi.scale =  m_Thing.GetScale(i);
        g_con.RenderGraphic(m_Thing.GetPositionX(i) - skyOffsetX, m_Thing.GetPositionY(i) - skyOffsetY, gi);//0,0,gi);
      }
      else
        g_con.RenderGraphic(m_Thing.GetPositionX(i), m_Thing.GetPositionY(i), gi);//0,0,gi);
     // }
    }
  }

  // 22222222222222 333333333333333 2222222222222222222 33333333333333333
  //render things - background that move
  for(int i = 0; i < m_Thing.Size(); i++){
    if(m_Thing.GetLayer(i) > 1 && m_Thing.GetLayer(i) < 4){
      if(m_Thing.GetPositionX(i) > leftBoundary && m_Thing.GetPositionX(i) < rightBoundary &&
         m_Thing.GetPositionY(i) > topBoundary && m_Thing.GetPositionY(i) < bottomBoundary){
         
         gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
         gi.scale =  m_Thing.GetScale(i);
         gi.rotationAngle = m_Thing.GetAngle(i);
         g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i), gi);
      }
    }
  }

  // 4444444444444444444444444444444444444444444444444444444444444444444444
  //layers  4 
  int offsetY;
  for(int i = 0; i < m_object.size(); i++){
    for(int layer = 4; layer < 5; layer++){
      if(m_object[i].x > leftBoundary && m_object[i].x < rightBoundary && m_object[i].y > topBoundary && m_object[i].y < bottomBoundary){
        gi = g_Sprite.GetSpriteData(m_object[i].spriteID);
        gi.scale = m_object[i].scale;
        gi.rotationAngle = m_object[i].angle;
        if(gi.layer == layer){
          g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y , gi);
          
          //hanging snow
          lastHeight = gi.height - 2;
          if(gi.width == 128){
            if(gi.slope > 0){
              gi = g_Sprite.GetSpriteData(86);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x - 30, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
            else{
              gi = g_Sprite.GetSpriteData(89);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x - 2, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
          }

        }
      }
    }
  }

  //render player's sprite - level 4
  if(m_Player.GetLayer() == 4){
    
    if(m_Player.GetDirection() == 1){  //move right
      if(m_Player.GetSlope() < - 0.5){//steep uphill slow down
        gi = g_Sprite.GetSpriteData(17);
      }
      else{
        gi = g_Sprite.GetSpriteData(15); //level small incline
      }
    }
    else{                              //move left  
      if(m_Player.GetSlope() > 0.5){
        gi = g_Sprite.GetSpriteData(16); //steep uphill slow down
      }
      else{
        gi = g_Sprite.GetSpriteData(14); //level small incline
      }
    }
    gi.scale = m_Player.GetScale();
    playerAngle = -m_Physics.GetGroundSlope(m_Player.GetPlayerX(), m_Player.GetLayer());
    gi.rotationAngle = playerAngle * .125;
    g_con.RenderGraphic(g_con.GetScreenCenterX()- gi.width/2, g_con.GetScreenCenterY() - gi.height + 12 , gi);
    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 25, 0, 0, 255, 25);// gi.width/2);
    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 75, 0, 0, 255, 25);// gi.width/2);

  }
  
  //render things
  for(int i = 0; i < m_Thing.Size(); i++){
    if(m_Thing.GetLayer(i) == 4  && m_Thing.IsAlive(i) == true){
      if(m_Thing.GetPositionX(i) > leftBoundary && m_Thing.GetPositionX(i) < rightBoundary &&
         m_Thing.GetPositionY(i) > topBoundary && m_Thing.GetPositionY(i) < bottomBoundary){
         
         gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
         gi.scale =  m_Thing.GetScale(i);
         gi.rotationAngle = m_Thing.GetSlope(i) * .125;
         //gi.rotationAngle = m_Thing.GetAngle(i);
         g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i), gi);
         //g_con.DrawCircle((g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i) + m_Thing.GetCenterX(i) * m_Thing.GetScale(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i)  + m_Thing.GetCenterY(i) * m_Thing.GetScale(i),0,200,0,m_Thing.GetRadius(i)* m_Thing.GetScale(i) );
      }
    }
  }

  // 55555555555555555555555555555555555555555555555555555555555555555555555555555
  //layer 5 - snow wedges
  for(int i = 0; i < m_object.size(); i++){
    for(int layer = 5; layer < 6; layer++){
      if(m_object[i].x > leftBoundary && m_object[i].x < rightBoundary && m_object[i].y > topBoundary && m_object[i].y < bottomBoundary){
        gi = g_Sprite.GetSpriteData(m_object[i].spriteID);
        gi.scale = m_object[i].scale;
        gi.rotationAngle = m_object[i].angle;
        if(gi.layer == layer){
          g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y , gi);

          //hanging snow
          lastHeight = gi.height - 2;
          if(gi.width = 128){
            if(gi.slope > 0){
              gi = g_Sprite.GetSpriteData(87);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x - 30, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
            else{
              gi = g_Sprite.GetSpriteData(90);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x  - 2, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
          }
        }
      }
    }
  }

  //render player's sprite - level 5
  if(m_Player.GetLayer() == 5){
    if(m_Player.GetDirection() == 1){  //move right
      if(m_Player.GetSlope() < - 0.5){//steep uphill slow down
        gi = g_Sprite.GetSpriteData(17);
      }
      else{
        gi = g_Sprite.GetSpriteData(15); //level small incline
      }
    }
    else{                              //move left  
      if(m_Player.GetSlope() > 0.5){
        gi = g_Sprite.GetSpriteData(16); //steep uphill slow down
      }
      else{
        gi = g_Sprite.GetSpriteData(14); //level small incline
      }
    }
    gi.scale = m_Player.GetScale();
    playerAngle = -m_Physics.GetGroundSlope(m_Player.GetPlayerX(), m_Player.GetLayer());
    gi.rotationAngle = playerAngle * .25;//.125
    g_con.RenderGraphic(g_con.GetScreenCenterX()- gi.width/2, g_con.GetScreenCenterY() - gi.height + 12 , gi);
    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 25, 0, 0, 255, 25);// gi.width/2);
    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 75, 0, 0, 255, 25);// gi.width/2);

  }

  //render things
  for(int i = 0; i < m_Thing.Size(); i++){
//if( (m_Thing.GetLayer(i) == 6 || m_Thing.GetLayer(i) == 7)){
    if(m_Thing.GetLayer(i) == 5  && m_Thing.IsAlive(i) == true){
      if(m_Thing.GetPositionX(i) > leftBoundary && m_Thing.GetPositionX(i) < rightBoundary &&
         m_Thing.GetPositionY(i) > topBoundary && m_Thing.GetPositionY(i) < bottomBoundary){
         
         gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
         gi.scale =  m_Thing.GetScale(i);
         gi.rotationAngle = m_Thing.GetSlope(i) * .125;
         //gi.rotationAngle = m_Thing.GetAngle(i);
         g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i), gi);
         //g_con.DrawCircle((g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i) + m_Thing.GetCenterX(i) * m_Thing.GetScale(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i)  + m_Thing.GetCenterY(i) * m_Thing.GetScale(i),0,200,0,m_Thing.GetRadius(i)* m_Thing.GetScale(i) );
       }
    }
  }
  // 66666666666666666666666666666666666666666666666666666666666666666666666
  //layer 6 snow wedges
  for(int i = 0; i < m_object.size(); i++){
    for(int layer = 6; layer < 7; layer++){
      if(m_object[i].x > leftBoundary && m_object[i].x < rightBoundary && m_object[i].y > topBoundary && m_object[i].y < bottomBoundary){
        gi = g_Sprite.GetSpriteData(m_object[i].spriteID);
        gi.scale = m_object[i].scale;
        gi.rotationAngle = m_object[i].angle;
        if(gi.layer == layer){
          g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y , gi);
          //hanging snow
          lastHeight = gi.height - 2;
          if(gi.width = 128){
            if(gi.slope > 0){
              gi = g_Sprite.GetSpriteData(88);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x - 30, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
            else{
              gi = g_Sprite.GetSpriteData(91);
              g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_object[i].x - 2, (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) + m_object[i].y + lastHeight, gi);
            }
          }
        }
      }
    }
  }

  //render player's sprite - level 6
  if(m_Player.GetLayer() == 6){
    if(m_Game.GetState() != GAME_WIN){
      if(m_Player.GetDirection() == 1){  //move right
        if(m_Player.GetSlope() < - 0.5){//steep uphill slow down
          gi = g_Sprite.GetSpriteData(17);
        }
        else{
          gi = g_Sprite.GetSpriteData(15); //level small incline
        }
      }
      else{                              //move left  
        if(m_Player.GetSlope() > 0.5){
          gi = g_Sprite.GetSpriteData(16); //steep uphill slow down
        }
        else{
          gi = g_Sprite.GetSpriteData(14); //level small incline
        }
      }
      gi.scale = m_Player.GetScale();
      playerAngle = -m_Physics.GetGroundSlope(m_Player.GetPlayerX(), m_Player.GetLayer());
      gi.rotationAngle = playerAngle * .125;
      g_con.RenderGraphic(g_con.GetScreenCenterX()- gi.width/2, g_con.GetScreenCenterY() - gi.height + 12, gi); //-8
    }
    //animated victory sprite
    else if(m_Game.GetState() == GAME_WIN){
      gi = g_Sprite.GetSpriteData(122);
      g_con.RenderGraphic(g_con.GetScreenCenterX()- gi.width/2, g_con.GetScreenCenterY() - gi.height + 12, gi);
    }


    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 25, 0, 0, 255, 25);// gi.width/2);
    //g_con.DrawCircle(g_con.GetScreenCenterX(), g_con.GetScreenCenterY() - 75, 0, 0, 255, 25);// gi.width/2);
  }
  
  //render things
  for(int i = 0; i < m_Thing.Size(); i++){
    if( (m_Thing.GetLayer(i) == 6 || m_Thing.GetLayer(i) == 7) && m_Thing.IsAlive(i) == true){
      if(m_Thing.GetPositionX(i) > leftBoundary && m_Thing.GetPositionX(i) < rightBoundary &&
         m_Thing.GetPositionY(i) > topBoundary && m_Thing.GetPositionY(i) < bottomBoundary){
         
         gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
         gi.scale =  m_Thing.GetScale(i);
         if(m_Thing.GetLayer(i) == 6)
           gi.rotationAngle = m_Thing.GetSlope(i) * .125;
         else if(m_Thing.GetLayer(i) == 7)
           gi.rotationAngle = m_Thing.GetAngle(i);
         if(m_Thing.GetSpriteID(i) != 122)
           g_con.RenderGraphic( (g_con.GetScreenCenterX()- m_Player.GetPlayerX()) + m_Thing.GetPositionX(i), (g_con.GetScreenCenterY()- m_Player.GetPlayerY()) +  m_Thing.GetPositionY(i), gi);
       }
    }
  }

  //render snowflakes
  VECTOR2D pos;
  gi = g_Sprite.GetSpriteData(8);
  
  for(int i = 0; i < m_snow.Size(); i++){
    pos = m_snow.GetPosition(i);
    gi.scale = m_snow.GetScale(i);
    gi.rotationAngle = m_snow.GetAngle(i);
    if(pos.x > 0 && pos.x < g_con.GetScreenWidth() && pos.y > 0 && pos.y < g_con.GetScreenHeight())
      g_con.RenderGraphic(pos.x, pos.y, gi);
  }

  //render things - fixed to screen
  for(int i = 0; i < m_Thing.Size(); i++){
    if(m_Thing.GetLayer(i) > 7){
      gi = g_Sprite.GetSpriteData(m_Thing.GetSpriteID(i));
      gi.scale =  m_Thing.GetScale(i);
      gi.rotationAngle = m_Thing.GetAngle(i);
      g_con.RenderGraphic( m_Thing.GetPositionX(i), m_Thing.GetPositionY(i), gi);
    }
  }

  //draw level circles
  switch(m_Game.GetLevel()){
    case 1:
      g_con.DrawCircle( m_levelX + 41, m_levelY + 70,255,0,0,15);
      g_con.DrawCircle( m_levelX + 41, m_levelY + 70,255,0,0,16);
      g_con.DrawCircle( m_levelX + 41, m_levelY + 70,255,0,0,17);
      break;
    case 2:
      g_con.DrawCircle( m_levelX + 73, m_levelY + 64,255,0,0,15);
      g_con.DrawCircle( m_levelX + 73, m_levelY + 64,255,0,0,16);
      g_con.DrawCircle( m_levelX + 73, m_levelY + 64,255,0,0,17);
      break;
    case 3:
      g_con.DrawCircle( m_levelX + 99, m_levelY + 53,255,0,0,15);
      g_con.DrawCircle( m_levelX + 99, m_levelY + 53,255,0,0,16);
      g_con.DrawCircle( m_levelX + 99, m_levelY + 53,255,0,0,17);
      break;
    case 4:
      g_con.DrawCircle( m_levelX + 135, m_levelY + 43,255,0,0,15);
      g_con.DrawCircle( m_levelX + 135, m_levelY + 43,255,0,0,16);
      g_con.DrawCircle( m_levelX + 135, m_levelY + 43,255,0,0,17);
      break;
    case 5:
      g_con.DrawCircle( m_levelX + 156, m_levelY + 25,255,0,0,15);
      g_con.DrawCircle( m_levelX + 156, m_levelY + 25,255,0,0,16);
      g_con.DrawCircle( m_levelX + 156, m_levelY + 25,255,0,0,17);
      break;
  }
  
  //display state stuff
  if(m_Game.GetState() == GAME_BEGIN_LEVEL2 || 
     m_Game.GetState() == GAME_BEGIN_LEVEL3 ||
     m_Game.GetState() == GAME_BEGIN_LEVEL4 || 
     m_Game.GetState() == GAME_BEGIN_LEVEL5) {
       switch(m_LevelCounter){
        case 2:
          g_con.RenderGraphic(g_con.GetScreenCenterX() - 277, g_con.GetScreenCenterY() - 176, g_Sprite.GetSpriteData(116));
          break;
        case 1:
          g_con.RenderGraphic(g_con.GetScreenCenterX() - 277, g_con.GetScreenCenterY() - 176, g_Sprite.GetSpriteData(117));
          break;
        case 0:
          g_con.RenderGraphic(g_con.GetScreenCenterX() - 277, g_con.GetScreenCenterY() - 176, g_Sprite.GetSpriteData(118));
          break;
       }
  }

  //displays health red bar on health thermometer
  gi = g_Sprite.GetSpriteData(104);
  gi.height = (float)(m_Game.GetHealth() * 9)/11;
  g_con.RenderGraphic(m_healthX + 26, m_healthY + 120 - gi.height  , gi);

  //draws paused graphic
  if(m_bPaused == true){
      g_con.RenderGraphic(g_con.GetScreenCenterX() - 228, g_con.GetScreenCenterY() - 53, g_Sprite.GetSpriteData(20));
  }
  
  if(m_Game.IsOver() == true){
    g_con.RenderGraphic(g_con.GetScreenCenterX() - 343, g_con.GetScreenCenterY() - 140, g_Sprite.GetSpriteData(106));
  }

  if(m_Game.GetState() == GAME_WIN){
    //animated victory sprite
    gi = g_Sprite.GetSpriteData(122);
    g_con.RenderGraphic(g_con.GetScreenCenterX()- gi.width/2, g_con.GetScreenCenterY() - gi.height + 12, gi);
  
    //YOU WIN sprite
    g_con.RenderGraphic(g_con.GetScreenCenterX() - 395, g_con.GetScreenCenterY() - 300, g_Sprite.GetSpriteData(107 + m_winFrame));

    //animates code to show star effect
    if(m_winTimer.getTimer(0.200)){
      m_winFrame++;
      if(m_winFrame > 8)
        m_winFrame = 0;
    }
  }

  //display score
  oss << m_Game.GetScore();
  g_con.Draw2DText(oss.str() ,F_V20, m_scoreX + 110, m_scoreY + 7, 255, 0, 0, 255);

  //display framerate on main game page
  oss.str("");
  oss << m_gameTimerCount;//"240"
  g_con.Draw2DText(oss.str() ,F_V20, 975, 15, 255, 0, 255, 255);
  g_con.Draw2DText(g_Global.g_strFPS, F_V20, g_con.GetScreenCenterX() - 50, g_con.GetScreenHeight() - 40,0, 150,0);
  g_con.Draw2DTextValue("Position", m_Player.GetPlayerX(), F_V20, g_con.GetScreenCenterX() - 65, g_con.GetScreenHeight() - 70,0, 150,0);

  //display frame rate if specififed in config.ini
  if(g_Global.g_bDisplayFramerate == true){
    g_con.Draw2DText(g_Global.g_strFPS, F_V20, g_con.GetScreenCenterX() - 50, g_con.GetScreenHeight() - 40,0, 150,0);
    
  //  g_con.Draw2DTextValue("slope", m_Player.GetSlope(), F_V20, 0, 120, 0, 255, 255, 255);
  /*  g_con.Draw2DTextValue("playerY", m_Player.GetPlayerY(), F_V20, 0, 140, 0, 255, 255, 255);
    g_con.Draw2DTextValue("endX", m_endX, F_V20, 0, 160, 0, 255, 255, 255);
    g_con.Draw2DTextValue("endY", m_endY, F_V20, 0, 180, 0, 255, 255, 255);
    g_con.Draw2DTextValue("state", m_Game.GetState(), F_V20, 0, 200, 255, 255, 0, 255);
    g_con.Draw2DTextValue("Game Level",m_Game.GetLevel(), F_V20, 0, 220, 255, 0, 255, 255);
    g_con.Draw2DTextValue("Level Counter",m_LevelCounter, F_V20, 0, 240, 255, 0, 255, 255);
    g_con.Draw2DTextValue("Health", m_Game.GetHealth(), F_V20, g_con.GetScreenCenterX() - 10,10, 0, 255, 255, 255);
*/
    
  }
  
  g_con.EndDrawing ();
  g_con.Present();

    //used for testing screenshot capture
  if(m_bCaptureScreen == true && m_screenCapture.getTimer(1.0) == true){
    hr = g_con.ScreenGrab("screenshot1.jpg");
    m_bCaptureScreen == false;
    m_screenCapture.ResetTimer();

    if(hr == 0)
      g_Log.Write("Screenshot saved successfully!");
    else
      g_Log.Write("Screenshot save FAILURE",hr);

  }
}

void CGameStatePlay::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}
void CGameStatePlay::deactivate(){

}
void CGameStatePlay::resume(){
}
void CGameStatePlay::pause(){
}
void CGameStatePlay::save(){
}

void CGameStatePlay::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
