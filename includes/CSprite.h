/******************************************************************************
  Class: CSprite - Stores data for a sprites used in game.
  Written: Nov. 7,2007 by Chuck Bolin
  Project: 4E6 Entry
******************************************************************************/

//CSprite.h
#ifndef CSPRITE_H
#define CSPRITE_H
#include <vector>
#include <string>

//used with all graphic images
struct GRAPHIC_IMAGE{
  int fileID;
  int sourceX;
  int sourceY;
  int width;
  int height;
  int rotationX;
  int rotationY;
  float scale;
  int alpha;
  float rotationAngle;
  std::string name;
  int layer;
  int frameMax;
  int frameCount;
  double updateInterval;
  double time;
  float slope;
  bool enable;
};

class CSprite{
public:
  CSprite();
  ~CSprite();

  void AddSprite( GRAPHIC_IMAGE &gi );
  GRAPHIC_IMAGE GetSpriteData(int spriteID);
  void IncrementSpriteFrameCount(int spriteID, double timeDifference);
  int Size();
  void EnableSprite(int spriteID);
  void DisableSprite(int spriteID);
  bool IsEnabled(int spriteID);

private:
  std::vector<GRAPHIC_IMAGE> m_image;
};

#endif
