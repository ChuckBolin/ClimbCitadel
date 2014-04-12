#include "..\includes\CSnow.h"

CSnow::CSnow(){
  m_snowTimer.initialize();
  m_windVel.x = 0;//4;
  m_windVel.y = 1;//1;
  m_gravity = 6;
  m_maxDiameter = 2;
  m_angle = 0;

}

int CSnow::Size(){
  return m_snow.size();
}

void CSnow::SetWidthHeight(int width, int height){
  m_screenWidth = width;
  m_screenHeight = height;
}

void CSnow::Update(double timeDifference){
  static double timeSum = 0;

  //timeDifference += m_snowTimer.getTimeDifference();
  timeSum += timeDifference;
  if(timeSum > 0.01){
    timeSum = 0;

        for(int i = 0; i < m_snow.size(); i++){
      m_snow[i].pos.x = m_snow[i].pos.x + m_snow[i].vel.x + m_windVel.x;
      m_snow[i].pos.y = m_snow[i].pos.y + m_snow[i].vel.y + m_windVel.y;

      if(m_snow[i].pos.y > m_screenHeight){
        m_snow[i].pos.y = - 10;
        m_snow[i].pos.x = - 800 + (rand() % (m_screenWidth + 800));
        m_snow[i].scale = .1 + (float)((rand() % 5) * .1);
      }
    }
    /*
    for(int i = 0; i < m_snow.size(); i++){
      m_snow[i].pos.x = m_snow[i].pos.x +  m_windVel.x;
      m_snow[i].pos.y = m_snow[i].pos.y +  m_windVel.y + m_gravity;

      if(m_snow[i].pos.y > m_screenHeight){
//        m_snow[i].pos.y = - 512;
//        m_snow[i].pos.x = - 512 + (rand() % (m_screenWidth + 512));

        m_snow[i].vel.x = - .1 + (rand() * 5) * .2;// m_windVel.x;
        m_snow[i].vel.y = m_windVel.y;
        m_snow[i].scale = .8;//.9 + (float)((rand() % m_maxDiameter) * .2);
        m_snow[i].angle = (float)(rand() % 628) * .01;
        m_snow[i].pos.x = m_snow[i].originalPos.x;//  - 540;// - (rand() % 1000);// -512 * (1 + (rand() % 2));//  - (int)((float)512 * m_snow[i].scale)- (rand() % 512);
        m_snow[i].pos.y = m_snow[i].originalPos.y;// -400 + (rand() % 1200);// 0;//- (int)((float)512 * m_snow[i].scale) + (rand() % (m_screenWidth + (int)((float)512 * m_snow[i].scale)));

        //m_snow[i].vel.x = m_windVel.x;
        //m_snow[i].vel.y = m_windVel.y;
        //m_snow[i].scale = .9 + (float)((rand() % m_maxDiameter) * .2);
        //m_snow[i].angle = (float)(rand() % 628) * .01;
        //m_snow[i].pos.y = - (int)((float)512 * m_snow[i].scale)- (rand() % 512);
        //m_snow[i].pos.x = - (int)((float)512 * m_snow[i].scale) + (rand() % (m_screenWidth + (int)((float)512 * m_snow[i].scale)));
      }
    }
  */


  }
}

void CSnow::CreateSnow(int number){
  if(number < 1) 
    return;

  int x;
  int y;
  float scale;
  float angle;
  SNOW_FLAKE temp;

  for(int i = 0; i < number; i++){
    //x = - 512 + (rand() % (m_screenWidth + 512));;//rand() % m_screenWidth;
    //y = - (rand() % 1024);//m_screenHeight - 512;
    //scale = .9 + (float)((rand() % m_maxDiameter) * .1);
    angle  = (float)(rand() % 628) * .01;
    
    x = rand() % m_screenWidth;
    y = rand() % m_screenHeight;
    scale = .1 + (float)((rand() % 5) * .1);    
    
    temp.pos.x = x;
    temp.pos.y = y;
    temp.vel.y = (int)m_gravity;
    temp.vel.x = -1 + (rand() % 8);
    temp.scale = scale;
    temp.angle = angle;

    
    /*temp.vel.x = m_windVel.x;
    temp.vel.y = m_windVel.y;
    temp.scale = .8;//.9 + (float)((rand() % m_maxDiameter) * .2);
    temp.angle = (float)(rand() % 628) * .01;
    temp.pos.y = y;// - (rand() % 2000);// -512 * (1 + (rand() % 2));//  - (int)((float)512 * m_snow[i].scale)- (rand() % 512);
    temp.pos.x = x;//- (int)((float)512 * m_snow[i].scale) + (rand() % (m_screenWidth + (int)((float)512 * m_snow[i].scale)));
    temp.originalPos.x = temp.pos.x;
    temp.originalPos.y = temp.pos.y;
*/

    m_snow.push_back(temp);
  }
}

void CSnow::Clear(){
  m_snow.clear();
}

VECTOR2D CSnow::GetPosition(int index){
  VECTOR2D temp;
  temp.x = 0;
  temp.y = 0;
  if(index < 0 || index > m_snow.size() - 1)
    return temp;

  return m_snow[index].pos;
}

float CSnow::GetScale(int index){
  if(index < 0 || index > m_snow.size() - 1)
    return 1.0f;

  return m_snow[index].scale;
}

float CSnow::GetAngle(int index){
  if(index < 0 || index > m_snow.size() - 1)
    return 1.0f;

  return m_snow[index].angle;
}

void CSnow::SetEnvironment(float windX, float windY, float gravity){
  m_windVel.x = windX;
  m_windVel.y = windY;
  m_gravity = gravity;
}

/*
private:
  CTimer m_snowTimer;
  VECTOR2D m_windVel;
  float m_gravity;
  std::vector<SNOW_FLAKE> m_snow;
  int m_screenWidth;
  int m_screenHeight;*/