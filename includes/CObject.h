#ifndef COBJECT_H
#define COBJECT_H
#include <windows.h>

class CObject{
public:
  void SetType(int type);
  int GetType();
  void SetStatus(int status);
  int GetStatus();

private:
  int m_type;  //0=player, 1=enemy tank, 2=obstacle
  int m_status;  //-1 = dead, 0 = ok, 1=collides, 2=hit
  float m_posX;
  float m_posY;
  float m_vel;
  float m_dir;
  float m_az;
  float m_elev;
  POINT m_v0, m_v1, m_v2, m_v3;

};

#endif
