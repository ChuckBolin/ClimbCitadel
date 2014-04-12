//CTrain.h
#ifndef CTRAIN_H
#define CTRAIN_H

#include <vector>
#include "CPhysics.h"

const float C_MAX_ANGLE = 0.05; //radians
const float C_TIE_STEP = 12;   //spacing between railroad ties
const float C_TIE_MOUSE_DISTANCE = 20;
const int C_TIE_MAX_TEMP = 62;

struct RR_TIE{
  float x;  //was int 
  float y;
  float angle;
};


const int C_TRAIN_TYPE_ENGINE = 0;
const int C_TRAIN_TYPE_COALCAR = 1;

struct TRAIN_COMPONENT{
  float x;    //center of component
  float y;
  float fx;   //forward wheel base
  float fy;
  float rx;   //rear wheel base
  float ry;
  float direction; //angle about x,y  0 rad is north
  int type;
  int id;  //unique id
};

class CTrain{
public:
  CTrain();
  void Initialize(int x, int y, float angle);
  int Size();
  int TempSize();
  RR_TIE GetRailroadTieTemp(int i);
  RR_TIE GetRailroadTieActual(int i);
  void update(int x, int y);          //used only laying out ties
  void AddTempRailTies();
  void AddComponent(float x, float y, float direction);
  TRAIN_COMPONENT GetComponent(int id);
  int ComponentSize();
  bool IsEnabled(){ return m_bEnabled;}

private:
  CPhysics m_Physics;            //used for various calculations
  std::vector<RR_TIE> m_rrTie;   //stores actual railroad
  std::vector<RR_TIE> m_rrTemp; //used for drawing
  float tieStep;
  std::vector<TRAIN_COMPONENT> m_component; //stores all components
  bool m_bEnabled;
};


#endif