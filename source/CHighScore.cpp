//CHighScore.cpp
#include "..\includes\CHighScore.h"

CHighScore::CHighScore(std::string sFilename, int nLimit){
  m_Filename = sFilename;
  if(nLimit < 1)
    nLimit = 0;
  m_Limit = nLimit;
  
  std::ifstream file;
  std::string sLine;
  std::string sName;
  std::string sScore;
  int nScore = 0;
  size_t nPos = 0;
  file.open(m_Filename.c_str());
  SCORE_DATA sd;

  if(file.is_open()){
    while (! file.eof() )
    {
      getline (file,sLine);
      nPos = sLine.find(",");
      if(nPos != std::string::npos){
        sName = sLine.substr(0, nPos);
        sScore = sLine.substr(nPos + 1);
        nScore = atoi(sScore.c_str());
        sd.name = sName;
        sd.score = nScore;
        m_Score.push_back(sd);
      }
    }
  }
  file.close();
  sort();
}

void CHighScore::addScore(std::string name, int score){
  SCORE_DATA sd;
  sd.name = name;
  sd.score = score; 
  m_Score.push_back(sd);
  sort();
  saveData();
}

void CHighScore::saveData(){
  std::ofstream file;

  file.open(m_Filename.c_str());
  if(file.is_open()){
    for(int i = 0; i < m_Score.size(); i++){
      file << m_Score[i].name << ", " << m_Score[i].score << "\n";
    }
  file.close();
  }

}

int CHighScore::size(){
  return m_Score.size();
}

std::string CHighScore::getName(int id){
  if(id < 0 || id > m_Score.size())
    return "";
  return m_Score[id].name;
}

int CHighScore::getHighScore(){
  if(m_Score.size() < 0)
    return 0;
  return m_Score[0].score;
}

int CHighScore::getLowScore(){
  if(m_Score.size() < 1)
    return 0;
  if(m_Score.size() > m_Limit)  //only consider top 10 scores
    return m_Score[m_Limit - 1].score;
  else  
    return m_Score[m_Score.size() - 1].score;
}

int CHighScore::getScore(int id){
  if(id < 0 || id > m_Score.size())
    return 0;
  return m_Score[id].score;
}

void CHighScore::sort(){
  SCORE_DATA sd;
  sd.name = "";
  sd.score = 0;

  for (int i = 0; i < m_Score.size(); i++){
    for(int j = 0; j < m_Score.size(); j++){
      if(i != j){
        if(m_Score[i].score > m_Score[j].score){
          sd = m_Score[i];
          m_Score[i] = m_Score[j];
          m_Score[j] = sd;
        }
      }
    }
  }
}
