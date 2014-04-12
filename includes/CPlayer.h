//CPlayer.h
#ifndef CPLAYER_H
#define CPLAYER_H

class CPlayer{
public:
  CPlayer();
  int GetPlayerX(){return m_PlayerX;}
  int GetPlayerY(){return m_PlayerY;}
  int GetLayer(){return m_Layer;}
  void SetPlayerPosition(int x, int y);
  void SetLayerYPositions(int layer4, int layer5, int layer6);
  float GetScale(){return m_Scale;}
  void Update(double timeDifference);
  void SetCurrentLayer(int layer);
  void MovePlayerRight();
  void MovePlayerLeft();
  void MoveBack();  
  void MoveForward();
  void JumpToLayer(int layer);
  void SetBoundary(int left, int right);
  int GetPlayerElevation(int level);
  void SetPlayerXPosition(int x);
  int GetDirection(){return m_Direction;}
  int GetLayerY(int layer);
  void SetSlopes(float slope4, float slope5, float slope6);
  int GetVelocity(){return m_Velocity;}
  float GetSlope(){return m_Slope;}

private:
  int m_PlayerX;  //player position
  int m_PlayerY;
  int m_Layer4;   //position of layers 4, 5 and 6
  int m_Layer5;
  int m_Layer6;
  float m_Slope4;
  float m_Slope5;
  float m_Slope6;
  float m_Slope; //current slope
  int m_Layer;    //current layer number
  float m_gravity;
  float m_Scale;
  int m_LeftBoundary;
  int m_RightBoundary;
  double m_TimeDiff;
  int m_LayerVel;
  int m_LastLayer;
  int m_NextLayer;
  bool m_bFalling;
  int m_Direction;
  int m_Velocity;

  int m_Elevation;

};

#endif
