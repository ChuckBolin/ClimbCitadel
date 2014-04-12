#include "..\includes\CGame.h"


CGame::CGame(){
  ResetGame();
}

void CGame::ResetGame(){
  m_GameState = GAME_START;
  m_Health = 100;
  m_Score = 0;
  m_GameLevel = 1;
  m_LevelTime = 0;
  m_GameTime = 0;
  m_PlayerName = "";
  m_TimeTotal = 0;
  m_bGameOver = false;
}

void CGame::RestoreGame(int state, int health, int score, int level, 
                        int levelTime, int gameTime, std::string playerName){
  m_GameState = state;
  m_Health = health;
  m_Score = score;
  m_GameLevel = level;
  m_LevelTime = levelTime;
  m_GameTime = gameTime;
  m_PlayerName = playerName;   
  m_TimeTotal = 0;
}

void CGame::Update(double timeDifference){
  m_TimeTotal += timeDifference;

  if(m_TimeTotal > 1.0){
    m_LevelTime++;
    m_GameTime++;
  }
}

void CGame::IncrementGameState(){
  m_GameState++;
  m_LevelTime = 0;
}
  
void CGame::SetState(int state){
  m_GameState = state;
}
int CGame::GetState(){
  return m_GameState;
}
  
void CGame::SetHealth(int health){
  m_Health = health;
}
void CGame::DecHealth(int health){
  m_Health -= health;
}

int CGame::GetHealth(){
  return m_Health;
}

void CGame::SetScore(int score){
  m_Score = score;
}

int CGame::GetScore(){
  return m_Score;
}

void CGame::IncScore(int score){
  m_Score += score;
}

void CGame::SetLevel(int level){
  m_GameLevel = level;
}
int CGame::GetLevel(){
  return m_GameLevel;
}

void CGame::SetPlayerName(std::string name){
  m_PlayerName = name;
}
std::string CGame::GetPlayerName(){
  return m_PlayerName;
}