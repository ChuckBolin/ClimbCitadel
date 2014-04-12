#include "..\includes\CPlayer.h"

CPlayer::CPlayer(){
  m_Direction = 1;
}

void CPlayer::SetPlayerPosition(int x, int y){
  m_PlayerX = x;
  m_PlayerY = y;
  m_TimeDiff = 0;
  m_LayerVel = 0;
  m_bFalling = false;
}

void CPlayer::SetLayerYPositions(int layer4, int layer5, int layer6){
  m_Layer4 = layer4;
  m_Layer5 = layer5;
  m_Layer6 = layer6;
}

void CPlayer::SetPlayerXPosition(int x){
  if(x < m_LeftBoundary || x > m_RightBoundary)
    return;

  m_PlayerX = x;
}

void CPlayer::SetCurrentLayer(int layer){
  if(layer < 4 || layer > 6)
    layer = 5;

  m_Layer = layer;

}

void CPlayer::MovePlayerRight(){
  if(m_LayerVel != 0)
    return;

  if(m_Slope <= -.5) //steep uphill
    m_Velocity = m_Layer * 2;//1
  else if(m_Slope >= .5) //steep downhill
    m_Velocity = m_Layer * 4;// 2;
  else
    m_Velocity = m_Layer * 6;//3

  m_PlayerX += m_Velocity;

  if(m_PlayerX > m_RightBoundary)
    m_PlayerX = m_RightBoundary;
  m_Direction = 1;

}

void CPlayer::MovePlayerLeft(){
  if(m_LayerVel != 0)
    return;

  if(m_Slope <= -.5) //steep downhill
    m_Velocity = m_Layer * 4; //2
  else if(m_Slope >= .5) //steep uphill
    m_Velocity = m_Layer * 2;// 1;
  else
    m_Velocity = m_Layer * 6;//3

  m_PlayerX -= m_Velocity;

  if(m_PlayerX < m_LeftBoundary)
    m_PlayerX = m_LeftBoundary;
  m_Direction = -1;

}

void CPlayer::JumpToLayer(int layer){
  if(layer < 4 || layer > 6)
    return;

  if (layer > m_Layer)
    m_Layer = layer;
  else if(layer < m_Layer)
    m_Layer = layer;

  //if( m_Layer == 4)
  //  m_Scale = 0.9;
  //else if(m_Layer == 5)
    m_Scale = 1.0;
  //else if(m_Layer == 6)
  //  m_Scale = 1.2;
   
}

void CPlayer::SetSlopes(float slope4, float slope5, float slope6){
  m_Slope4 = slope4;
  m_Slope5 = slope5;
  m_Slope6 = slope6;

  if( m_Layer == 4)
    m_Slope = m_Slope4;
  else if(m_Layer == 5)
    m_Slope = m_Slope5;
  else if(m_Layer == 6)
    m_Slope = m_Slope6;
 
}

void CPlayer::SetBoundary(int left, int right){
  if(left > right)
    return;

  m_LeftBoundary = left;
  m_RightBoundary = right;
}

//update position of player
void CPlayer::Update(double timeDifference){
  m_TimeDiff += timeDifference;
  int nextLayerPos = 0;
   
  //changing layers at transition
  if(m_LayerVel != 0){

    if(m_NextLayer == 4)
      nextLayerPos = m_Layer4;
    else if(m_NextLayer == 5)
      nextLayerPos = m_Layer5;
    else if(m_NextLayer == 6)
      nextLayerPos = m_Layer6;
    else
      nextLayerPos = 3000;
    
    m_PlayerY += m_LayerVel;

    if(m_LayerVel < 0){//moves back
      if(m_PlayerY < nextLayerPos){
        m_LayerVel = 0;
        m_Layer = m_NextLayer;
        m_PlayerY = nextLayerPos;
      }
    }
    else if(m_LayerVel > 0){//moves front
      if(m_PlayerY > nextLayerPos){
        m_LayerVel = 0;
        m_Layer = m_NextLayer;
        m_PlayerY = nextLayerPos;
      }
    }
  }
  
  //not changing layers
  else{
    //m_PlayerX += m_Velocity;
    if(m_Layer == 4){
      if(m_Layer4 < 0)
        m_PlayerY++;
      else
        m_PlayerY = m_Layer4;        
    }
    else if(m_Layer == 5){
      if(m_Layer5 < 0)
        m_PlayerY++;
      else
        m_PlayerY = m_Layer5;
    }
    else if(m_Layer == 6){
      if(m_Layer6 < 0)
        m_PlayerY++;
      else
        m_PlayerY = m_Layer6;
    }
  }

  if( m_Layer == 4)
    m_Scale = 0.95;
  else if(m_Layer == 5)
    m_Scale = 1.0;
  else if(m_Layer == 6)
    m_Scale = 1.05;

}

//************************************************
// MoveBack()
void CPlayer::MoveBack(){
  if(m_LayerVel != 0)
    return;
  m_LastLayer = m_Layer;
  m_NextLayer = m_Layer;

  //player is on level 5
  if(m_Layer5 < m_PlayerY && m_PlayerY < m_Layer6)
    m_NextLayer = 5;
  else if(m_Layer6 < m_PlayerY && m_PlayerY < m_Layer5)
    m_NextLayer = 6;
  else if(m_Layer4 < m_PlayerY && m_PlayerY < m_Layer5)
    m_NextLayer = 4;
  else if(m_Layer5 < m_PlayerY && m_PlayerY < m_Layer4)
    m_NextLayer = 5;
  else if(m_Layer4 < m_PlayerY && m_PlayerY < m_Layer6)
    m_NextLayer = 4;
  else if(m_Layer6 < m_PlayerY && m_PlayerY < m_Layer4)
    m_NextLayer = 6;

  //player is on level 6
  else if(m_Layer5 < m_Layer6 && m_Layer6 < m_PlayerY)
    m_NextLayer = 6;
  else if(m_Layer6 < m_Layer5 && m_Layer5 < m_PlayerY)
    m_NextLayer = 5;  
  else if(m_Layer4 < m_Layer5 && m_Layer5 < m_PlayerY)
    m_NextLayer = 5;
  else if(m_Layer5 < m_Layer4 && m_Layer5 < m_PlayerY)
    m_NextLayer = 4;  
  else if(m_Layer4 < m_Layer6 && m_Layer6 < m_PlayerY)
    m_NextLayer = 6;
  else if(m_Layer6 < m_Layer4 && m_Layer4 < m_PlayerY)
    m_NextLayer = 4;  

  m_LayerVel = -m_Layer * 3;//-10;  
}

//************************************************
// MoveForward()
void CPlayer::MoveForward(){
  if(m_LayerVel != 0)
    return;
  m_LastLayer = m_Layer;
  m_NextLayer = m_Layer;

  //player is on level 4
  if(m_PlayerY < m_Layer5 && m_Layer5 < m_Layer6)
    m_NextLayer = 5;
  else if(m_PlayerY < m_Layer6 && m_Layer6 < m_Layer5)
    m_NextLayer = 6;
  else if(m_PlayerY < m_Layer4 && m_Layer4 < m_Layer6)
    m_NextLayer = 4;
  else if(m_PlayerY < m_Layer6 && m_Layer6 < m_Layer4)
    m_NextLayer = 6;
  else if(m_PlayerY < m_Layer4 && m_Layer4 < m_Layer5)
    m_NextLayer = 4;
  else if(m_PlayerY < m_Layer5 && m_Layer5 < m_Layer4)
    m_NextLayer = 5;
  
  //player is on level 5
  else if(m_Layer5 < m_PlayerY && m_PlayerY < m_Layer6)
    m_NextLayer = 6;
  else if(m_Layer6 < m_PlayerY && m_PlayerY < m_Layer5)
    m_NextLayer = 5;
  else if(m_Layer4 < m_PlayerY && m_PlayerY < m_Layer6)
    m_NextLayer = 6;
  else if(m_Layer6 < m_PlayerY && m_PlayerY < m_Layer4)
    m_NextLayer = 4;
  else if(m_Layer4 < m_PlayerY && m_PlayerY < m_Layer5)
    m_NextLayer = 5;
  else if(m_Layer5 < m_PlayerY && m_PlayerY < m_Layer4)
    m_NextLayer = 4;

  m_LayerVel = m_Layer * 4;
  //m_LayerVel = 20;  
}

int CPlayer::GetPlayerElevation(int level){
  if(level < 1 || level > 5)
    return 0;

  switch(level){
  case 1:
    return 16000 + ((5000 - m_PlayerY)/10);
  case 2:
    return 16500 + ((7500 - m_PlayerY)/10);
  case 3:
    return 17250 + ((10000 - m_PlayerY)/10);
  case 4:
    return 18250 + ((12500 - m_PlayerY)/10);
  case 5:
    return 19500 + ((15000 - m_PlayerY)/10);
  }

}

int CPlayer::GetLayerY(int layer){
  if(layer < 4 || layer > 6)
    return 0;

  if(layer == 4)
    return m_Layer4;
  else if(layer == 5)
    return m_Layer5;
  else if(layer == 6)
    return m_Layer6;
}