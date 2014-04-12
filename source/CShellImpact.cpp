#include "..\includes\CShellImpact.h"

CShellImpact::CShellImpact(){
  m_maxCount = 100;

  //load image locations
  m_imageRect[0].left=1;  m_imageRect[0].top = 182;
  m_imageRect[1].left=33;  m_imageRect[1].top = 182;
  m_imageRect[2].left=65;  m_imageRect[2].top = 182;
  m_imageRect[3].left=97;  m_imageRect[3].top = 182;
  m_imageRect[4].left=1;  m_imageRect[4].top = 214;
  m_imageRect[5].left=33;  m_imageRect[5].top = 214;
  m_imageRect[6].left=65;  m_imageRect[6].top = 214;
  m_imageRect[7].left=97;  m_imageRect[7].top = 214;
  m_imageRect[8].left=1;  m_imageRect[8].top = 246;
  m_imageRect[9].left=33;  m_imageRect[9].top = 246;
  m_imageRect[10].left=65;  m_imageRect[10].top = 246;
  m_imageRect[11].left=97;  m_imageRect[11].top = 246;
  m_imageRect[12].left=1;  m_imageRect[12].top = 278;
  m_imageRect[13].left=33;  m_imageRect[13].top = 278;
  m_imageRect[14].left=65;  m_imageRect[14].top = 278;
  m_imageRect[15].left=97;  m_imageRect[15].top = 278;
  m_imageRect[16].left=1;  m_imageRect[16].top = 121;
  m_imageRect[17].left=31;  m_imageRect[17].top = 121;
  m_imageRect[18].left=61;  m_imageRect[18].top = 121;
  m_imageRect[19].left=121;  m_imageRect[19].top = 121;  
}

RECT CShellImpact::GetImpactDestination(int id){
  RECT temp;
  if(id < 0 || id > m_exp.size()){
    temp.left = m_imageRect[0].left + (m_exp[id].frameOffset*128);
    temp.top = m_imageRect[0].top;
    temp.right = temp.left + 32;
    temp.bottom = temp.top + 32;
    return temp;
  }

  if(m_exp[id].active == true){
    temp.left = m_exp[id].pt.x;
    temp.top = m_exp[id].pt.y;
    temp.right = temp.left + 32;
    temp.bottom = temp.top + 32;
    return temp;
  }
}

RECT CShellImpact::GetImageSource(int id){
  
  RECT temp;
  if(id < 0 || id > m_exp.size()){
    temp.left = m_imageRect[0].left + (m_exp[id].frameOffset*128);
    temp.top = m_imageRect[0].top;
    temp.right = temp.left + 32;
    temp.bottom = temp.top + 32;
    return temp;
  }

  if(m_exp[id].active == true){
    if(m_exp[id].frameCount > 15){

    }
    else{
      temp.left = m_imageRect[m_exp[id].frameCount].left + (m_exp[id].frameOffset*128);
      temp.top = m_imageRect[m_exp[id].frameCount].top;
      temp.right = temp.left + 30;
      temp.bottom = temp.top + 30;
    }
  }
  else{//crater
    temp.left = m_imageRect[15 + m_exp[id].crater].left;
    temp.top = m_imageRect[15 + m_exp[id].crater].top;
    temp.right = temp.left + 26;
    temp.bottom = temp.top + 26;
  }
  return temp;
}

float CShellImpact::GetImpactX(int id){
  if(id < 0 || id > m_exp.size())
    return 0;
  return m_exp[id].pt.x;
}
float CShellImpact::GetImpactY(int id){
  if(id < 0 || id > m_exp.size())
    return 0;
  return m_exp[id].pt.y;
}

void CShellImpact::Update(int id){
  if(id < 0 || id > m_exp.size())
    return;

  //update framecount
  if(m_exp[id].active == true){
    m_exp[id].frameCount++;
    if(m_exp[id].frameCount>15)
      m_exp[id].active = false;
  }
}

void CShellImpact::MouseClick(const POINT p, int type){
  EXPLOSION ex;
  ex.active = true;
  ex.frameCount = 0;
  ex.pt = p;
  ex.type = 0;
  ex.crater = (rand() % 3) + 1;
  ex.frameOffset = (rand() % 2) + 1;
  if (m_exp.size()> m_maxCount){
    m_exp.erase(m_exp.begin());
  }
  m_exp.push_back(ex);

}

void CShellImpact::ImpactExplosion(const POINT p, int type){
  EXPLOSION ex;
  ex.active = true;
  ex.frameCount = 0;
  ex.pt = p;
  ex.type = 0;
  ex.crater = (rand() % 3) + 1;
  ex.frameOffset = (rand() % 2) + 1;
  if (m_exp.size()> m_maxCount){
    m_exp.erase(m_exp.begin());
  }
  m_exp.push_back(ex);
}

void CShellImpact::Clear(){
  m_exp.clear();
}

int CShellImpact::Size(){
  return m_exp.size();
}

