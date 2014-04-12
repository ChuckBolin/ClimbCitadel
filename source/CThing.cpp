#include "..\includes\CThing.h"

CThing::CThing(){
}

CThing::~CThing(){
}
  
//individual methods
int CThing::GetPositionX(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return - 1;

  return m_Thing[index].positionX;
}

int CThing::GetPositionY(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return - 1;

  return m_Thing[index].positionY;
}

void CThing::Clear(){
  m_Thing.clear();
}

int CThing::GetCenterX(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return - 1;

  return m_Thing[index].centerX;
}

int CThing::GetCenterY(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return - 1;

  return m_Thing[index].centerY;
}

float CThing::GetRadius(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return - 1;

  return m_Thing[index].radius;
}

void CThing::SetSlope(int index, float slope){
  if(index < 0 || index > m_Thing.size() - 1)
    return;

  m_Thing[index].slope = slope;
}

void CThing::Disable(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return;

  m_Thing[index].status = false;
}

void CThing::SetLayerY(int index, int y){
  if(index < 0 || index > m_Thing.size() - 1)
    return;

  m_Thing[index].layerY = y;
}

bool CThing::IsAlive(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return false;

  return m_Thing[index].status;
}

int CThing::GetSpriteID(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return -1;

  return m_Thing[index].spriteID;
}

int CThing::GetLayer(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return -1;

  return m_Thing[index].layer;
}

float CThing::GetAngle(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return -1;

  return m_Thing[index].angle;
}

float CThing::GetSlope(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return -1;

  return m_Thing[index].slope;// .angle;
}


float CThing::GetScale(int index){
  if(index < 0 || index > m_Thing.size() - 1)
    return -1;

  return m_Thing[index].scale;
}

//collection methods
void CThing::SetGravity(float gravity){
  m_Gravity = gravity;
}

void CThing::Update(double timeDifference){
    
  //only move objects on layers 4, 5 and 6
  for(int i = 0; i < m_Thing.size(); i++){
    if(m_Thing[i].status == true  && m_Thing[i].layer > 3 && m_Thing[i].layer < 7){
      if(m_Thing[i].spriteID < 14 || m_Thing[i].spriteID > 17){//don't draw player sprite used for bug fix

        if(m_Thing[i].spriteID == 92 || m_Thing[i].spriteID == 93){ //wolf and ram
          m_Thing[i].positionX -= 1;
          if(m_Thing[i].slope < 0)//-.5)
            m_Thing[i].positionY = m_Thing[i].layerY - 55;//55;
          else
            m_Thing[i].positionY = m_Thing[i].layerY - 5;
          if(m_Thing[i].positionX < m_LeftBoundary){
            m_Thing[i].positionX = 10000 + (rand() % 10000);
          }
        }
        else if(m_Thing[i].spriteID == 85){ //sled
          m_Thing[i].positionX -= 3;
            m_Thing[i].positionY = m_Thing[i].layerY - 70;
          if(m_Thing[i].positionX < m_LeftBoundary){
            m_Thing[i].positionX = 10000 + (rand() % 10000);
          }
        }
        else{
          m_Thing[i].velocityY += m_Gravity;
          if(m_Thing[i].velocityX == 0)
            m_Thing[i].velocityX = 0.5;
          m_Thing[i].positionX += m_Thing[i].velocityX;
          m_Thing[i].positionY += m_Thing[i].velocityY;
          
          if(m_Thing[i].positionY > m_Thing[i].layerY - 40){
            m_Thing[i].positionY = m_Thing[i].layerY - 40;
            m_Thing[i].velocityY = - m_Thing[i].velocityY * 0.5;
            m_Thing[i].velocityX =  - m_Thing[i].velocityY * m_Thing[i].slope * 2;
          }

          if(m_Thing[i].positionX < m_LeftBoundary){
            m_Thing[i].positionX = 10000 + (rand() % 10000);
            m_Thing[i].positionY = 75;
            m_Thing[i].velocityX = -2;
            m_Thing[i].velocityY = m_Gravity * 5;
          }

          if(m_Thing[i].positionX > m_RightBoundary){
            m_Thing[i].positionX = m_RightBoundary - 25;
            m_Thing[i].velocityX = -2;
          }
        }
      }//if(m_Thing..
    }
  }//for
}

void CThing::SetBoundary(int left, int right){
  if(left > right)
    return;

  m_LeftBoundary = left;
  m_RightBoundary = right;
}

void CThing::AddThing(CTHING cthing){
  m_Thing.push_back(cthing);
}

int CThing::Size(){
  return m_Thing.size();
}

int CThing::GetTotalThingsAlive(){
  int count = 0;

  for(int i = 0; i < m_Thing.size(); i++){
    if(m_Thing[i].status == true)
      count++;
  }

  return count;
}
