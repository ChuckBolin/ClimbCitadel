#ifndef CPHYSICS_H
#define CPHYSICS_H
#include <cmath>
//#include "CTank.h"
#include "Graphics.h"
#include "Log.h"

//constants and structures
//boundaries of the world
const int CWORLD_LEFT_LIMIT = 0;
const int CWORLD_TOP_LIMIT  = 0;
const int CWORLD_RIGHT_LIMIT = 20190;
const int CWORLD_BOTTOM_LIMIT = 20480;
const float PI = 3.14159; 

//class CTank;

struct VECTOR2D{ 
  float x; 
  float y; 
}; 

struct QUAD2D{ 
  VECTOR2D v1, v2, v3, v4; //4 verticies of quad 
}; 

struct CIRCLE2D{ 
  VECTOR2D center; 
  float radius; 
}; 

struct TILE{
  int leftEdge4;
  int rightEdge4;
  int leftEdge5;
  int rightEdge5;
  int leftEdge6;
  int rightEdge6;
  float slope4;
  float slope5;
  float slope6;
  int x4;  //left point
  int y4;
  int x5;
  int y5;
  int x6;
  int y6;
};

class CPhysics{
public:
  CPhysics();
  void Clear();
  int GetLeftWorldBoundary(){ return CWORLD_LEFT_LIMIT;}
  int GetRightWorldBoundary(){ return CWORLD_RIGHT_LIMIT;}
  int GetTopWorldBoundary(){ return CWORLD_TOP_LIMIT;}
  int GetBottomWorldBoundary(){ return CWORLD_BOTTOM_LIMIT;}
  float GetDistance(float x1, float y1, float x2, float y2);
  float GetAngle(float x1, float y1, float x2, float y2);
  float GetVectorMagnitude(VECTOR2D &vec); 
  float GetDotProduct(VECTOR2D &vec1, VECTOR2D &vec2); 
  float GetAngleTwoVectors(VECTOR2D &vec1, VECTOR2D &vec2); 
  bool IsPointInsideQuad(QUAD2D &q, VECTOR2D &ref); //
  bool QuadQuadIntersection(QUAD2D &q1, QUAD2D &q2); 
  bool IsPointInsideCircle(CIRCLE2D &c, VECTOR2D &v); //
  VECTOR2D CreatePointOnLine(VECTOR2D &p0, float t, VECTOR2D &v); 
  VECTOR2D RotateVector2D(VECTOR2D &pt, float angle); 
  QUAD2D RotateQuad(QUAD2D &q, VECTOR2D &refPt, float angle); 
  VECTOR2D NormalizeVector2D(VECTOR2D &v);
  float GetBearing(float x1, float y1, float x2, float y2);
  int QuadOverlapQuad(QUAD2D &q1, QUAD2D &q2);

  int GetTileSize();
  int GetGroundPosition(int x, int y, int layer);
  float GetGroundSlope(int x, int layer);
  void AddTile(int tileNumber, int layer, TILE tile);
  int GetTileX(int tileNumber, int layer);
  int GetTileY(int tileNumber, int layer);
  float GetTileSlope(int tileNumber, int layer);

private:
  std::vector<TILE> m_Tile;
};

#endif