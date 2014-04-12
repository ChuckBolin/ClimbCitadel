//****************************************************************************
// Class: CAudioManager
// Purpose: Manages FMOD Library for Chuck's Game, September 23, 2007
// Websites used as refernces:
//		http://www.fmod.org/docs/HTML/	FMOD Organization
//		http://www.robthebloke.org/		Tutorials
//
//link to fmod.lib
//add fmod.dll to current folder for distributions
//****************************************************************************

//CAudioManager.h
#ifndef CAUDIOMANAGER_H
#define CAUDIOMANAGER_H

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <cstring>
#include <vector>
#include "fmod.h"
#include "fmod_errors.h"
#include "CTimer.h"
#include "Log.h"

struct AUDIO_VEC{
  int AudioID;
  FSOUND_SAMPLE *handle;
  std::string filename;
  int channelPlaying;
};

class CAudioManager{
public:
  CAudioManager();
  ~CAudioManager();
  bool IsValidSound();
  int GetMaxChannels();
  void LoadSample(int id, std::string filename);
  void PlaySoundClip(int id, unsigned int mode);
  void StopSoundClip(int id);
  void KillAll();
  void ListAll();
  bool IsPlaying(int id);
  std::string GetFilename(int id);
  void PauseAll();
  void UnPauseAll();
  void SetVolume(int id, int vol);
  int GetVolume(int id);
  int Size();


private:
 bool m_bValidAudio;
 int m_MaxChannels;
 std::vector<AUDIO_VEC> m_AudioClip;
};

#endif

/*
  FSOUND_SAMPLE *handle2;
  handle2=FSOUND_Sample_Load (0,"test1.wav",0,0,0);
  if(handle2 == NULL){
    cout << "Error";
    system("pause");
    return 0;
  }
  FSOUND_PlaySound (4,handle2);
  system("pause");
  
  for(int i=0; i<5; i++){
    for(int j=0; j<256; j++){
      FSOUND_SetVolume (4, j);
      Sleep(30);
    }
  }
  system("pause");
  FSOUND_SetPaused (4, true);
  system("pause");
  FSOUND_SetPaused (4, false);
  system("pause");
  FSOUND_StopSound (4);
  system("pause");
  FSOUND_Sample_Free (handle2);

}*/