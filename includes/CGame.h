//CGame.h
#include <string>

const int GAME_START = 0;

//level 1
 const int GAME_BEGIN_LEVEL1 = 1;
 const int GAME_PLAY_LEVEL1 = 2;
 const int GAME_END_LEVEL1 = 3;

//level 2
 const int GAME_BEGIN_LEVEL2 = 4;
 const int GAME_PLAY_LEVEL2 = 5;
 const int GAME_END_LEVEL2 = 6;

//level 3
 const int GAME_BEGIN_LEVEL3 = 7;
 const int GAME_PLAY_LEVEL3 = 8;
 const int GAME_END_LEVEL3 = 9;

//level 4
 const int GAME_BEGIN_LEVEL4 = 10;
 const int GAME_PLAY_LEVEL4 = 11;
 const int GAME_END_LEVEL4 = 12;

//level 5
 const int GAME_BEGIN_LEVEL5 = 13;
 const int GAME_PLAY_LEVEL5 = 14;
 const int GAME_END_LEVEL5 = 15;

//variations to game state
 const int GAME_WIN = 16;
 const int GAME_OVER = 17;

class CGame{
public:
  CGame();
  void ResetGame();
  void IncrementGameState();
  void CGame::RestoreGame(int state, int health, int score, int level, 
                        int levelTime, int gameTime, std::string playerName);
  void Update(double timeDifference);
  
  void SetState(int state);
  int GetState();
  
  void SetHealth(int health);
  int GetHealth();
  void DecHealth(int health);

  void SetScore(int score);
  int GetScore();
  void IncScore(int score);

  void SetLevel(int level);
  int GetLevel();

  void SetPlayerName(std::string name);
  std::string GetPlayerName();

  bool IsOver(){return m_bGameOver;}
  void SetGameOver(){m_bGameOver = true;}

private:
  int m_GameState;
  int m_Health;
  int m_Score;
  int m_GameLevel;
  int m_LevelTime;
  int m_GameTime;
  std::string m_PlayerName;
  float m_TimeTotal;
  bool m_bGameOver;

};