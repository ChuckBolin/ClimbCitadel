//CHighScore.h
#ifndef CHIGHSCORE_H
#define CHIGHSCORE_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct SCORE_DATA{
  std::string name;
  int score;
};

class CHighScore{
public:
  CHighScore(std::string sFilename, int nLimit);
  
  int size();
  std::string getName(int id);
  int getScore(int id);  
  int getHighScore();
  int getLowScore();
  void addScore(std::string name, int score);
private:
  void sort();
  void saveData();
  std::vector<SCORE_DATA> m_Score;
  std::string m_Filename;
  int m_Limit;

};

#endif
