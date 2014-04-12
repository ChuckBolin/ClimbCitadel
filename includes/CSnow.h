//CSnow.h
#ifndef CSNOW_H
#define CSNOW_H

#include <vector>
#include "CTimer.h"
#include "CPhysics.h"

struct SNOW_FLAKE{
  VECTOR2D pos;
  VECTOR2D vel;
  float scale;
  float angle;
  VECTOR2D originalPos;
};

class CSnow{
public:
  CSnow();
  int Size();
  void SetWidthHeight(int width, int height);
  void Update(double timeDifference);
  VECTOR2D GetPosition(int index);
  void Clear();
  void CreateSnow(int number);
  
  float GetScale(int index);
  float GetAngle(int index);
  void SetEnvironment(float windX, float windY, float gravity);

private:
  CTimer m_snowTimer;
  VECTOR2D m_windVel;
  float m_gravity;
  std::vector<SNOW_FLAKE> m_snow;
  int m_screenWidth;
  int m_screenHeight;
  int m_maxDiameter;
  float m_angle;

};

#endif