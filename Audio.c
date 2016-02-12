/******************************************************************
Author        :  Matt Chasengnou
File          :  Audio.c
Created       :  1/25/16
Description   :  Provides functionality for working with audio
ChangeLog
  -2/3/16     : Added LoadSoundSample, LoadSoundStream, PlaySound,
                and StopSound functions.
  -2/9/16     : Fixed filepaths for the sound loading functions.
                Sounds now play.
                Added functionality to play music from a stream
                Changed LoadSoundStream to LoadMusic.
******************************************************************/
#include "Audio.h"
#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

#include <stdlib.h>  /* malloc, free */
#include <string.h>  /* strcat */

#include <windows.h>  /* shows error messages */
/*-----------------------------------------------------
DEFINES
-----------------------------------------------------*/
#define true 1
#define false 0
//#define TRUE true
//#define FALSE false

#define SOUND_FOLDER_NAME "./sounds/"
#define PATH_MAX_LEN 256 /* Maximum number of char allowed in path */

typedef int bool;
/*-----------------------------------------------------
END DEFINES
------------------------------------------------------*/


/*------------------------------------------------------
GLOBAL VARIABLES
------------------------------------------------------*/
FMOD_SYSTEM              *fmodSystem;
FMOD_SOUND               *sound, *musicLib, *musicToPlay;
FMOD_CHANNEL             *channel = 0;
FMOD_RESULT               result;
unsigned int              version;
void                     *extraDriverData = 0;
int               numsubsounds;
/*------------------------------------------------------
END GLOBAL VARIABLES
------------------------------------------------------*/


/* 
  Initialize the audio system. Instructions for initialization taken
  FMOD site:
http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
*/
void InitializeAudio(void)
{
  /* Max number of audio channels to use. Limit is 4093. */
  int channels = 32;

  /* Create an FMOD system. */
  result = FMOD_System_Create(&fmodSystem);
  FMOD_ErrorString(result);

  /* check the version. */
  result = FMOD_System_GetVersion(fmodSystem, &version);
  FMOD_ErrorString(result);

  /* If not the correct version, fatal error is thrown. */
  //if (version < FMOD_VERSION)
  //{
  //  Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
  //}

  /* Initialize the FMOD system. */
  result = FMOD_System_Init(fmodSystem, channels, FMOD_INIT_NORMAL, extraDriverData);
  /* Check for errors. */
  FMOD_ErrorString(result);
}

/*
Closes the audio system and frees it from memory.
*/
void FreeAudio(void)
{
  FMOD_System_Release(fmodSystem);
  FMOD_System_Close(fmodSystem);
}

/*
  Loads a sound as a sample from a file.
  Include extensions in name. ex: "drums.wav"
  pSound is pointer to the sound object you will store the sound in.
*/
void LoadSoundSample(const char *sName, FMOD_SOUND **pSound)
{
  /* Generate the path for the sound file. */
  char *path = (char *)calloc(PATH_MAX_LEN, sizeof(char));

  strcat_s(path, PATH_MAX_LEN * sizeof(char), SOUND_FOLDER_NAME);
  strcat_s(path, PATH_MAX_LEN * sizeof(char), sName);

  result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, NULL, pSound);
  FMOD_ErrorString(result);
  
  if (result != FMOD_OK)
  {
    int msgboxID = MessageBox(
      NULL,
      FMOD_ErrorString(result),
      "LoadSoundSample error.\n",
      MB_ICONEXCLAMATION | MB_YESNO
      );
  }
}

/*
  Loads a music file as a stream.
  Include extensions in name. ex: "drums.wav"
  pSound is pointer to the sound object you will store the sound in.
*/
void LoadMusic(const char *sName, FMOD_SOUND **pSound)
{
  /* Generate the path for the sound file. */
  char *path = (char *)calloc(PATH_MAX_LEN, sizeof(char));

  strcat_s(path, PATH_MAX_LEN * sizeof(char), SOUND_FOLDER_NAME);
  strcat_s(path, PATH_MAX_LEN * sizeof(char), sName);

  result = FMOD_System_CreateStream(fmodSystem, path, FMOD_LOOP_NORMAL, NULL, pSound);
  FMOD_ErrorString(result);

  if (result != FMOD_OK)
  {
    int msgboxID = MessageBox(
      NULL,
      FMOD_ErrorString(result),
      "LoadSoundSample error.\n",
      MB_ICONEXCLAMATION | MB_YESNO
      );
  }

  result = FMOD_Sound_GetNumSubSounds(*pSound, &numsubsounds);//result = sound->getNumSubSounds(&numsubsounds);
  //ERRCHECK(result);

  if (numsubsounds)
  {
    FMOD_Sound_GetSubSound(*pSound, 0, &musicToPlay);//sound->getSubSound(0, &sound_to_play);
    //ERRCHECK(result);
  }
  else
  {
    musicToPlay = musicLib;//sound_to_play = sound;
  }

}

/*
  Plays a sound.
  pSound is pointer to sound you want to play.
*/
void PlayAudio(FMOD_SOUND *pSound)
{
  result = FMOD_System_PlaySound(fmodSystem, pSound, NULL, false, &channel);
  FMOD_ErrorString(result);
}

/* 
  Stops playback of a sound.
*/
void StopSound(FMOD_SOUND *pSound)
{
  result = FMOD_System_PlaySound(fmodSystem, pSound, NULL, true, &channel);
  FMOD_ErrorString(result);
}

void FreeSound()
{
  result = FMOD_Sound_Release(sound);//result = sound1->release();
  FMOD_ErrorString(result);//ERRCHECK(result);
  result = FMOD_Sound_Release(musicLib);
  FMOD_ErrorString(result);
  result = FMOD_System_Close(fmodSystem);//result = sound2->release();
  FMOD_ErrorString(result);//ERRCHECK(result);
  result = FMOD_System_Release(fmodSystem);//result = sound3->release();
  FMOD_ErrorString(result);//ERRCHECK(result);
  //result = system->close();
  //ERRCHECK(result);
  //result = system->release();
  //ERRCHECK(result);
}

void TestAudioINIT(void)
{
  InitializeAudio();
  LoadSoundSample("sample.ogg", &sound);
  LoadMusic("music_sample.ogg", &musicLib);

  PlayAudio(sound);
  PlayAudio(musicToPlay);
}

void UpdateAudio()
{
  //Common_Update();

  //if (Common_BtnPress(BTN_ACTION1))
  //{
  //  result = system->playSound(sound1, 0, false, &channel);
  //  ERRCHECK(result);
  //}

  //if (Common_BtnPress(BTN_ACTION2))
  //{
  //  result = system->playSound(sound2, 0, false, &channel);
  //  ERRCHECK(result);
  //}

  //if (Common_BtnPress(BTN_ACTION3))
  //{
  //  result = system->playSound(sound3, 0, false, &channel);
  //  ERRCHECK(result);
  //}

  result = FMOD_System_Update(fmodSystem);//result = system->update();
  FMOD_ErrorString(result);//ERRCHECK(result);

  {
    unsigned int ms = 0;
    unsigned int lenms = 0;
    bool         playing = 0;
    bool         paused = 0;
    int          channelsplaying = 0;

    if (channel)
    {
      FMOD_SOUND *currentsound = 0;

      result = FMOD_Channel_IsPlaying(channel, &playing);//result = channel->isPlaying(&playing);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        FMOD_ErrorString(result);//ERRCHECK(result);
      }

      result = FMOD_Channel_IsPlaying(channel, &paused);//result = channel->getPaused(&paused);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        FMOD_ErrorString(result);//ERRCHECK(result);
      }

      result = FMOD_Channel_GetPosition(channel, &ms, FMOD_TIMEUNIT_MS);//result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        FMOD_ErrorString(result);//ERRCHECK(result);
      }

      //channel->getCurrentSound(&currentsound);
      if (currentsound)
      {
        result = FMOD_Sound_GetLength(currentsound, &lenms, FMOD_TIMEUNIT_MS);//result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
          FMOD_ErrorString(result);//ERRCHECK(result);
        }
      }
    }

    FMOD_System_GetChannelsPlaying(fmodSystem, &channelsplaying);// system->getChannelsPlaying(&channelsplaying);
  }
}