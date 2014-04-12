#include "..\includes\CPhysics.h"

extern CDXGraphics g_con;
extern Log g_Log;

//return 0 if no pts overlap other quads
//return 1 if q1 pt in q2
//return 2 if q2 pt in q1
int CPhysics::QuadOverlapQuad(QUAD2D &q1, QUAD2D &q2){
  //look for quad 1 pts in quad 2
  if(IsPointInsideQuad(q2, q1.v1) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v2) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v3) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v4) == true)
    return 1;

  //look for quad 2 pt in quad 1
  else if(IsPointInsideQuad(q1, q2.v1) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v2) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v3) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v4) == true)
    return 1;

  return 0;
}

float CPhysics::GetDistance(float x1, float y1, float x2, float y2){
  return sqrt( (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

float CPhysics::GetAngle(float x1, float y1, float x2, float y2){
  float angle = 0;
  float dx;
  float dy;

  dy = y2 - y1;
  dx = x2 - x1;
  if(dx == 0){

  }
  else{
    angle = atan(-dy/dx);
  }
  if(dy > 0 && dx > 0){  //bottom-right OK
    angle = PI/2 - angle;
  }
  else if(dy > 0 && dx < 0) {//bottom-left OK
    angle = 5*PI/2 -(angle + PI);
  }
  else if(dy < 0 && dx < 0){ //top-left OK
    angle = 3*PI/2 - angle;
  }
  else if(dy < 0 && dx > 0){ //top-right
    angle = 5*PI/2 -(angle + 2*PI);
  }
  
  return angle;
}

//North = 0 radians
//East = 1.57
//South = 3.14
//West = 4.71
float CPhysics::GetBearing(float x1, float y1, float x2, float y2){
  float angle = 0;
  float dx;
  float dy;

  dy = y2 - y1;
  dx = x2 - x1;
  if(dx == 0){

  }
  else{
    angle = atan(-dy/dx);
  }

  if(dy > 0 && dx > 0){  //bottom-right OK
    angle = PI/2 - angle;
  }
  else if(dy > 0 && dx < 0) {//bottom-left OK
    angle = 5*PI/2 -(angle + PI);
  }
  else if(dy < 0 && dx < 0){ //top-left OK
    angle = 3*PI/2 - angle;
  }
  else if(dy < 0 && dx > 0){ //top-right
    angle = 5*PI/2 -(angle + 2*PI);
  }

  if(angle > 2 * PI)
    angle = 0.0f;
  if(angle < 0.0f)
    angle = 2 * PI;

  return angle;
}


//returns magnitude of vector 
float CPhysics::GetVectorMagnitude(VECTOR2D &vec){ 
  return sqrt( (vec.x * vec.x) + (vec.y * vec.y)); 
} 

//calcs and returns dot product 
float CPhysics::GetDotProduct(VECTOR2D &vec1, VECTOR2D &vec2){ 
  return vec1.x * vec2.x + vec1.y * vec2.y; 
} 

//returns angle between two vectors 
float CPhysics::GetAngleTwoVectors(VECTOR2D &vec1, VECTOR2D &vec2){ 
  return (180/PI) * acos( GetDotProduct(vec1, vec2)/(GetVectorMagnitude(vec1)* 
                          GetVectorMagnitude(vec2))); 
} 

//returns true if point is inside quad 
bool CPhysics::IsPointInsideQuad(QUAD2D &q, VECTOR2D &ref){ 
  float angle = 0; 
  VECTOR2D a, b; 

  a.x = q.v1.x - ref.x; 
  a.y = q.v1.y - ref.y; 
  b.x = q.v2.x - ref.x; 
  b.y = q.v2.y - ref.y; 
  angle = GetAngleTwoVectors(a, b); 

  a.x = q.v2.x - ref.x; 
  a.y = q.v2.y - ref.y; 
  b.x = q.v3.x - ref.x; 
  b.y = q.v3.y - ref.y; 
  angle += GetAngleTwoVectors(a, b); 

  a.x = q.v3.x - ref.x; 
  a.y = q.v3.y - ref.y; 
  b.x = q.v4.x - ref.x; 
  b.y = q.v4.y - ref.y; 
  angle += GetAngleTwoVectors(a, b); 

  a.x = q.v4.x - ref.x; 
  a.y = q.v4.y - ref.y; 
  b.x = q.v1.x - ref.x; 
  b.y = q.v1.y - ref.y; 

  angle += GetAngleTwoVectors(a, b); 

  if(angle < 359.9) 
    return false; 
  if(angle > 360.1) 
    return false; 
  
  return true; 
} 

bool CPhysics::QuadQuadIntersection(QUAD2D &q1, QUAD2D &q2){ 
  if(IsPointInsideQuad(q1, q2.v1)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v2)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v3)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v4)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v1)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v2)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v3)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v4)) 
    return true; 

  return false; 
} 

bool CPhysics::IsPointInsideCircle(CIRCLE2D &c, VECTOR2D &v){ 
  VECTOR2D vec; 
  vec.x = c.center.x - v.x; 
  vec.y = c.center.y - v.y; 

  float distance = GetVectorMagnitude(vec); 

  if(distance < c.radius) 
    return true; 

  return false; 
} 

//given point p0, and directin vector v, and increment t 
//project line based upon t..used for ray casting 
VECTOR2D CPhysics::CreatePointOnLine(VECTOR2D &p0, float t, VECTOR2D &v){ 
  VECTOR2D temp; 
  temp.x = p0.x + (t * v.x); 
  temp.y = p0.y + (t * v.y); 
  return temp; 
} 

VECTOR2D CPhysics::NormalizeVector2D(VECTOR2D &v){ 
  VECTOR2D vec; 
  float mag = GetVectorMagnitude(v); 
  vec.x = v.x/mag; 
  vec.y = v.y/mag; 
  return vec; 
} 

//rotates point about reference point 
VECTOR2D CPhysics::RotateVector2D(VECTOR2D &pt, float angle){ 
  VECTOR2D vec; 
  vec.x = pt.x * cos(angle) - pt.y * sin(angle); 
  vec.y = pt.x * sin(angle) + pt.y * cos(angle); 
  return vec; 
} 

//rotates quad about reference point 
QUAD2D CPhysics::RotateQuad(QUAD2D &q, VECTOR2D &refPt, float angle){ 
  QUAD2D quad; 
  QUAD2D temp; 
  temp.v1.x = q.v1.x - refPt.x; 
  temp.v1.y = q.v1.y - refPt.y; 
  quad.v1 = RotateVector2D(temp.v1, angle); 
  quad.v1.x += refPt.x;
  quad.v1.y += refPt.y;
  temp.v2.x = q.v2.x - refPt.x; 
  temp.v2.y = q.v2.y - refPt.y; 
  quad.v2 = RotateVector2D(temp.v2, angle); 
  quad.v2.x += refPt.x;
  quad.v2.y += refPt.y;
  temp.v3.x = q.v3.x - refPt.x; 
  temp.v3.y = q.v3.y - refPt.y; 
  quad.v3 = RotateVector2D(temp.v3, angle); 
  quad.v3.x += refPt.x;
  quad.v3.y += refPt.y;
  temp.v4.x = q.v4.x - refPt.x; 
  temp.v4.y = q.v4.y - refPt.y; 
  quad.v4 = RotateVector2D(temp.v4, angle); 
  quad.v4.x += refPt.x;
  quad.v4.y += refPt.y;
  
  return quad; 
} 

//Tile stuff added for ClimbingTheCitadel
int CPhysics::GetTileSize(){
  return m_Tile.size();
}

//returns ground vertical value
int CPhysics::GetGroundPosition(int x, int y, int layer){
  if(x < CWORLD_LEFT_LIMIT || x > CWORLD_RIGHT_LIMIT)
    return -1;

  int tile = x / 128;
  int xOffset = x - (tile * 128);
  float slope;
  if(tile < 0 || tile > m_Tile.size() - 1)
    return -1;

  //y = mx  m is slope, x = position on slope
  if(layer == 4){
    slope = m_Tile[tile].slope4;
    if(slope > 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y4 - m_Tile[tile].leftEdge4 ;
    else if(slope < 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y4 ;
  }
  else if(layer == 5){
    slope = m_Tile[tile].slope5;
    if(slope > 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y5 - m_Tile[tile].leftEdge5 ;
    else if(slope < 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y5 ;
  }
  else if(layer == 6){
    slope = m_Tile[tile].slope6;
    if(slope > 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y6 - m_Tile[tile].leftEdge6 ;
    else if(slope < 0)
      return (int)(( slope * (float)(xOffset))) + m_Tile[tile].y6 ;//+ m_Tile[tile].leftEdge5 ;
  }
  return -1;
}

//returns slope
float CPhysics::GetGroundSlope(int x, int layer){
  if(x < CWORLD_LEFT_LIMIT || x > CWORLD_RIGHT_LIMIT)
    return 0;

  int tile = x / 128;
  int xOffset = x - (tile * 128);
  float slope;
  if(tile < 0 || tile > m_Tile.size() - 1)
    return 0;

  //y = mx  m is slope, x = position on slope
  if(layer == 4){
    slope = m_Tile[tile].slope4;
    return slope;
  }
  else if(layer == 5){
    slope = m_Tile[tile].slope5;
    return slope;
  }
  else if(layer == 6){
    slope = m_Tile[tile].slope6;
    return slope;
  }
  return 0;
}


//adds tiles
void CPhysics::AddTile(int tileNumber, int layer, TILE tile){
  if(tileNumber < 0 || tileNumber > 159)
    return;

  if(layer < 4 || layer > 6)
    return;

  if(layer == 4){
    m_Tile[tileNumber].leftEdge4 = tile.leftEdge4;
    m_Tile[tileNumber].rightEdge4 = tile.rightEdge4;
    m_Tile[tileNumber].slope4 = tile.slope4;
    m_Tile[tileNumber].x4 = tile.x4;
    m_Tile[tileNumber].y4 = tile.y4;
  }
  else if(layer == 5){
    m_Tile[tileNumber].leftEdge5 = tile.leftEdge5;
    m_Tile[tileNumber].rightEdge5 = tile.rightEdge5;
    m_Tile[tileNumber].slope5 = tile.slope5;
    m_Tile[tileNumber].x5 = tile.x5;
    m_Tile[tileNumber].y5 = tile.y5;
  }
  else if(layer == 6){
    m_Tile[tileNumber].leftEdge6 = tile.leftEdge6;
    m_Tile[tileNumber].rightEdge6 = tile.rightEdge6;
    m_Tile[tileNumber].slope6 = tile.slope6;
    m_Tile[tileNumber].x6 = tile.x6;
    m_Tile[tileNumber].y6 = tile.y6;
  }
  
}

//ctor
CPhysics::CPhysics(){
  TILE temp;
  
  //preload m_Tile with empty tiles
  for(int i = 0; i < 160; i++){
    m_Tile.push_back(temp);
    
  }
}

void CPhysics::Clear(){
  TILE temp;
  
  //preload m_Tile with empty tiles
  //for(int i = 0; i < 160; i++){
  //  m_Tile[i] = temp;    
  //} 

}

float CPhysics::GetTileSlope(int tileNumber, int layer){
  if(tileNumber < 0 || tileNumber > 159)
    return -1;
  if(layer < 4 || layer > 6)
    return 0;

  if(layer == 4)
    return m_Tile[tileNumber].slope4;
  else if(layer == 5)
    return m_Tile[tileNumber].slope5;
  else if(layer == 6)
    return m_Tile[tileNumber].slope6;

}

int CPhysics::GetTileX(int tileNumber, int layer){
  if(tileNumber < 0 || tileNumber > 159)
    return -1;
  if(layer < 4 || layer > 6)
    return 0;


  if(layer == 4)
    return m_Tile[tileNumber].x4;
  else if(layer == 5)
    return m_Tile[tileNumber].x5;
  else if(layer == 6)
    return m_Tile[tileNumber].x6;
}

int CPhysics::GetTileY(int tileNumber, int layer){
  if(tileNumber < 0 || tileNumber > 159)
    return -1;
  if(layer < 4 || layer > 6)
    return 0;

  if(layer == 4)
    return m_Tile[tileNumber].y4;
  else if(layer == 5)
    return m_Tile[tileNumber].y5;
  else if(layer == 6)
    return m_Tile[tileNumber].y6;
}
