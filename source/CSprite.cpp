#include "..\includes\CSprite.h"

CSprite::CSprite(){
}

CSprite::~CSprite(){
}

void CSprite::AddSprite( GRAPHIC_IMAGE &gi ){
  m_image.push_back(gi);
}

GRAPHIC_IMAGE CSprite::GetSpriteData(int spriteID){
  GRAPHIC_IMAGE temp;
  if(spriteID < 0 || spriteID > m_image.size())
    return temp;

  return m_image[spriteID];
}

int CSprite::Size(){
  return m_image.size();
}


void CSprite::EnableSprite(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return;
  m_image[spriteID].enable = true;
}

void CSprite::DisableSprite(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return;
  m_image[spriteID].enable = false;
}

bool CSprite::IsEnabled(int spriteID){
  if(spriteID < 0 || spriteID > m_image.size())
    return false;

  return  m_image[spriteID].enable;
}

//this animates sprites
void CSprite::IncrementSpriteFrameCount(int spriteID, double timeDifference){
  GRAPHIC_IMAGE temp;
  if(spriteID < 0 || spriteID > m_image.size())
    return ;

  if(m_image[spriteID].frameMax == 0)  //don't inc if not animated
    return ;

  //animate if time is correct
  m_image[spriteID].time += timeDifference;
  if(m_image[spriteID].time > m_image[spriteID].updateInterval){
    m_image[spriteID].time = 0;

    if(m_image[spriteID].frameMax > 0){ //clockwise direction
      m_image[spriteID].frameCount++;
      if (m_image[spriteID].frameCount > m_image[spriteID].frameMax - 1)
        m_image[spriteID].frameCount = 0;
    }
    else if(m_image[spriteID].frameMax < 0){ //clockwise direction
      m_image[spriteID].frameCount--;
      if (m_image[spriteID].frameCount < m_image[spriteID].frameMax + 1 )
        m_image[spriteID].frameCount = 0;
    }

  }
}