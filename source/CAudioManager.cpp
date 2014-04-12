#include "..\includes\CAudioManager.h"

extern Log g_Log;

//initialize sound system
CAudioManager::CAudioManager(){
  m_bValidAudio = false;

  if( FSOUND_Init(44000,64,0) == false ){
		m_bValidAudio = false;
  }
  else{
		m_bValidAudio = true;
    m_MaxChannels = FSOUND_GetMaxChannels();
  }
}

//close sound system if it initialized correctly
CAudioManager::~CAudioManager(){
  if(m_bValidAudio == true){

    for(int i = 0; i < m_AudioClip.size(); i++)
      FSOUND_Sample_Free(m_AudioClip[i].handle);

    FSOUND_Close();
  }
}

void CAudioManager::PlaySoundClip(int id, unsigned int mode){
  if(m_bValidAudio == false)
    return;

  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id){
      if(mode == 0)
        FSOUND_Sample_SetMode(m_AudioClip[i].handle, FSOUND_LOOP_OFF);
      else
        FSOUND_Sample_SetMode(m_AudioClip[i].handle, FSOUND_LOOP_NORMAL);
        
      //m_AudioClip[i].channelPlaying = FSOUND_PlaySound(FSOUND_FREE, m_AudioClip[i].handle);
      m_AudioClip[i].channelPlaying = FSOUND_PlaySound(id, m_AudioClip[i].handle);
    }
  }
}

void CAudioManager::PauseAll(){
  if(m_bValidAudio == false)
    return;

  for(int i = 0; i < m_AudioClip.size(); i++){
    if(FSOUND_IsPlaying(m_AudioClip[i].channelPlaying) == true)
      FSOUND_SetPaused(m_AudioClip[i].channelPlaying, true);
  }
}

void CAudioManager::UnPauseAll(){
  if(m_bValidAudio == false)
    return;

  for(int i = 0; i < m_AudioClip.size(); i++){
    if(FSOUND_GetPaused(m_AudioClip[i].channelPlaying) == true)
      FSOUND_SetPaused(m_AudioClip[i].channelPlaying, false);
  }
}

int CAudioManager::GetVolume(int id){
  if(m_bValidAudio == false)
    return 0;

  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id && IsPlaying(id)==true){
      return FSOUND_GetVolume(m_AudioClip[i].channelPlaying);
    }
  }
  return 0;
}

void CAudioManager::SetVolume(int id, int vol){
  if(m_bValidAudio == false)
    return;
  if(vol < 0)
    vol = 0;
  if(vol > 255)
    vol = 255;

  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id && IsPlaying(id) == true){
      if(!FSOUND_SetVolumeAbsolute(m_AudioClip[i].channelPlaying, vol)){
        return;
      }
      else
        return;
    }
  }//for
}

bool CAudioManager::IsPlaying(int id){
  if(m_bValidAudio == false)
    return false;

  FSOUND_SAMPLE *handle;
  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id && m_AudioClip[i].channelPlaying > -1){
      if(FSOUND_IsPlaying(m_AudioClip[i].channelPlaying)==true){
        return true;
      }
      else{
        m_AudioClip[i].channelPlaying = -1;
        return false;
      }
    }//if
  }//for
 
  return false;
}

void CAudioManager::KillAll(){
  if(m_bValidAudio == false)
    return;

  for(int i = 0; i < m_AudioClip.size(); i++){
    //if(IsPlaying(i) == true){
    if(FSOUND_IsPlaying(i)==true){
      g_Log.Write("Playing", i);
      StopSoundClip(i);
    }
    else{
      g_Log.Write("Stopped", i);
    }
  }

  //FSOUND_StopSound(FSOUND_ALL);
  //::Sleep(2000);
}

bool CAudioManager::IsValidSound(){
  return m_bValidAudio;
}

int  CAudioManager::GetMaxChannels(){
  return m_MaxChannels;
}

void CAudioManager::LoadSample(int id, std::string filename){
  if(m_bValidAudio == false)
    return;

  AUDIO_VEC temp;
  FSOUND_SAMPLE *handle;
  //handle=FSOUND_Sample_Load (FSOUND_FREE,filename.c_str(),0,0,0);
  handle=FSOUND_Sample_Load (id,filename.c_str(),0,0,0);
  if(handle == NULL){ //error
    return;
  }
  else{               //good
    temp.AudioID = id;
    temp.handle = handle;
    temp.filename = filename;
    temp.channelPlaying = -1;
    m_AudioClip.push_back(temp);
  }
}

void CAudioManager::StopSoundClip(int id){
  if(FSOUND_IsPlaying(id)==true)
    FSOUND_StopSound(id);
  return;

  if(m_bValidAudio == false)
    return;
  FSOUND_SAMPLE *handle;
  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id){
      for(int j=0; j < GetMaxChannels(); j++){
        handle = FSOUND_Sample_Get(j);
        
        if(handle == m_AudioClip[i].handle){
          if(j > -1)
            FSOUND_StopSound(j - 1);
        }
      }//for
    }//if
  }
}

void CAudioManager::ListAll(){
  if(m_bValidAudio == false)
    return;
  
  for(int i = 0; i < m_AudioClip.size(); i++){
    std::cout << "Clip: (" << i << ") " << m_AudioClip[i].AudioID
              << ", " << m_AudioClip[i].handle << ", " 
              << m_AudioClip[i].filename << std::endl;
  }  
}

int CAudioManager::Size(){
  return m_AudioClip.size();
}

std::string CAudioManager::GetFilename(int id){
  if(m_bValidAudio == false)
    return "";
  
  for(int i = 0; i < m_AudioClip.size(); i++){
    if(m_AudioClip[i].AudioID == id){
      //std::string name = FSOUND_Sample_GetName(m_AudioClip[i].filename);// .handle);
      std::string name = m_AudioClip[i].filename;
      return name;
    }
  }  
  return "";
}
