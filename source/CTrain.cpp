#include "..\includes\CTrain.h"

CTrain::CTrain(){
  m_bEnabled = false;
}

//not quite angle..
//0 = up
//1 = right
//2 = down
//3 = left
void CTrain::Initialize(int x, int y, float angle){
  RR_TIE temp;
  temp.x = x;
  temp.y = y;

  int ref = (int)angle;
  if(ref == 0)
    temp.angle = 0;
  else if(ref == 1)
    temp.angle = 1.57;
  else if(ref == 2)
    temp.angle = 3.14;
  else if(ref == 3)
    temp.angle = 4.71;
  else{
    ref = 3;
    temp.angle = 4.71;
  }

  m_rrTie.push_back(temp);

  for(int i = 0; i < 25; i++){
    if(ref == 0){
      temp.x = x;    
      temp.y -= C_TIE_STEP;
    }
    if(ref == 1){
      temp.x += C_TIE_STEP;    
      temp.y = y;
    }
    if(ref == 2){
      temp.x = x;    
      temp.y += C_TIE_STEP;
    }
    if(ref == 3){
      temp.x -= C_TIE_STEP;    
      temp.y = y;
    }

    m_rrTie.push_back(temp); 
  }

  m_bEnabled = true;
}
int CTrain::Size(){
  return m_rrTie.size();

}

int CTrain::TempSize(){
  return m_rrTemp.size();
}

RR_TIE CTrain::GetRailroadTieActual(int i){
  
  RR_TIE temp;
  if(i < 0 || i > m_rrTie.size()-1)
    return temp;

  return m_rrTie[i];
}

RR_TIE CTrain::GetRailroadTieTemp(int i){

  RR_TIE temp;

  if(i < 0 || i > m_rrTemp.size()-1)
    return temp;

  return m_rrTemp[i];
}

void CTrain::AddTempRailTies(){
  RR_TIE temp;

  for(int i=0; i< m_rrTemp.size(); i++){
    m_rrTie.push_back(m_rrTemp[i]);
  }

}

//draw 50 temporary ties unless close
void CTrain::update(int x, int y){
  m_rrTemp.clear();
  RR_TIE last;
  RR_TIE next;
  float dx, dy;
  float bearing;

  last = m_rrTie[m_rrTie.size() - 1];  //last actual tie...to connect more ties
   
  tieStep = C_TIE_STEP;
  
  //add only 10 ties
  int count = 0;
  float bearingTemp = 0;
  float angleTemp = 0;

  while(count < C_TIE_MAX_TEMP){
    bearing = m_Physics.GetBearing(last.x, last.y, x, y);

    //this handles the problem when last.angle or bearing cross 6.28 and becomes 0.
    bearingTemp = bearing;
    angleTemp = last.angle;
    if(abs(bearing - last.angle) > PI){
      if(bearing - last.angle > 0)
        angleTemp += 2 * PI;
      else if(bearing - last.angle < 0)
        bearingTemp += 2 * PI;

      if(bearingTemp  > angleTemp)
        next.angle = last.angle + C_MAX_ANGLE;
      else if(bearingTemp < angleTemp )
        next.angle = last.angle - C_MAX_ANGLE;
    }
    else{  
      if(bearing  > last.angle)
        next.angle = last.angle + C_MAX_ANGLE;
      else if(bearing < last.angle )
        next.angle = last.angle - C_MAX_ANGLE;
    }

    //convert from c++ math coordinates frame (0 radians = East) to DX (0 radians = North)
    last.angle = last.angle - 7.85;
    dx = cos(last.angle) * tieStep;
    dy = sin(last.angle) * tieStep;

    //stores this data in temp vector
    next.x = last.x + dx;
    next.y = last.y + dy;
    m_rrTemp.push_back(next);
    last = next;  //this becomes the next 'end of the track'

    //stop creating temp layout
    if(m_Physics.GetDistance(next.x, next.y, x, y) < C_TIE_MOUSE_DISTANCE)
      break;

    count++;
  }
}

void CTrain::AddComponent(float x, float y, float direction){
  TRAIN_COMPONENT temp;
  temp.x = x;
  temp.y = y;
  temp.direction = direction;
  m_component.push_back(temp);
}

TRAIN_COMPONENT CTrain::GetComponent(int id){
  TRAIN_COMPONENT temp;

  if(id < 0 ||  id > m_component.size())
    return temp;

  return m_component[id];
}
 
int CTrain::ComponentSize(){
  return m_component.size();
}
