//CThing.h

#include <string>
#include <vector>

struct CTHING{
  float velocityX;
  float velocityY;
  int positionX;
  int positionY;
  int centerX;
  int centerY;
  float radius;
  int layer;
  int spriteID;
  float scale;
  float angle;
  float slope;
  int layerY;
  bool status;
};

class CThing{
public:

  CThing();
  ~CThing();
  
  //individual methods
  int GetPositionX(int index);
  int GetPositionY(int index);
  int GetCenterX(int index);
  int GetCenterY(int index);
  void SetSlope(int index, float slope);
  void SetLayerY(int index, int y);
  bool IsAlive(int index);
  int GetSpriteID(int index);
  float GetAngle(int index);
  float GetSlope(int index);
  float GetScale(int index);
  int GetLayer(int index);
  void Clear();
  void Disable(int index);
  float GetRadius(int index);
  void SetTop(int top){m_top = top;}

  //collection methods
  void SetGravity(float gravity);
  void Update(double timeDifference);
  void SetBoundary(int left, int right);
  void AddThing(CTHING cthing);
  int Size();
  int GetTotalThingsAlive();

private:
  std::vector<CTHING> m_Thing;
  float m_Gravity;
  int m_LeftBoundary;
  int m_RightBoundary;
  double m_TimeDiff;
  int m_top; //represents Y value where items are dropped
};