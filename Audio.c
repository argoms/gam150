/**************************************************************************************************
Project       :  EPOCH
File          :  Audio.c
Author        :  Matt Chasengnou
Created       :  1/25/16
Description   :  Provides functionality for working with audio
ChangeLog
  -2/3/16     : Added LoadSoundSample, LoadSoundStream, PlaySound, and StopSound functions.
  -2/9/16     : Fixed filepaths for the sound loading functions.
                Sounds now play.
                Added functionality to play music from a stream
                Changed LoadSoundStream to LoadMusic.
  -2/14/16    : Added data structures to store sound info.
                Changed names of functions to Audio_FunctionName to make it easier to pick out
                functions from this module.
  -2/16/16    : Added basic error checking for back-end FMOD functions.
                Modified functions to provide simpler interfaces.
                Cleaned up code, removed deprecated code.
  -3/8/16     : Added automatic initialization of sound assets

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
**************************************************************************************************/

#include "Audio.h"
#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

#include <stdio.h>   /* fopen, FILE */
#include <stdlib.h>  /* malloc, free */
#include <string.h>  /* strcat */

#include <windows.h>  /* shows error messages */


/*-------------------------------------------------------------------------------------------------
DEFINES
-------------------------------------------------------------------------------------------------*/

#define true 1
#define false 0

/* File used for initializing sound asset libraries */
#define INIT_FILE "./sounds/misc/audio_init.txt"

/* Asset directory paths */
#define SOUND_SAMPLE_DIRECTORY "./sounds/samples/"
#define SOUND_MUSIC_DIRECTORY "./sounds/music/"

#define PATH_MAX_LEN 256                /* Maximum number of char allowed in path */

#define HASH_KEY 23                     /* Used by the hash code generator */
#define NO_HASH  -1                     /* Argument to pass in search functions if hash code isn't known. */

typedef int bool;

typedef struct SOUND_ENTRY SOUND_ENTRY;
typedef struct SOUND_LIBRARY SOUND_LIBRARY;

/* Used during init for asset libraries */
enum INIT_MODES
{
  INIT_ADD_SAMPLE,    /* Tells init to add samples */
  INIT_ADD_MUSIC      /* Tells init to add music/streams */
};
/*-------------------------------------------------------------------------------------------------
END DEFINES
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
DATA STRUCTURE DEFINITIONS
-------------------------------------------------------------------------------------------------*/

/*
Sound entry type to use with the sound library.
*/
struct SOUND_ENTRY
{
  char                  *name;      /* The name of the file as it appears in the directory. */
  unsigned int           hashCode;  /* The hash code (address) of the sound entry. */
  bool                   loaded;    /* If the sound is currently loaded into memory or not. */
  FMOD_SOUND            *sound;     /* The FMOD sound object associated with this entry. */
  SOUND_ENTRY           *next;      /* The next entry in the list. */
};

/*
The sound library data structure.
Will be implemented using a hash table with separate chaining.
*/
struct SOUND_LIBRARY
{
  SOUND_ENTRY         **table;    /* Where the sound entries are being stored. */
  unsigned int          size;     /* The size of the hash table. */
};
/*-------------------------------------------------------------------------------------------------
END DATA STRUCTURE DEFINITIONS
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
GLOBAL VARIABLES
-------------------------------------------------------------------------------------------------*/

static FMOD_SYSTEM              *fmodSystem;            /* The sound system */
static FMOD_CHANNEL             *channel = 0;           /* The current channel */
static FMOD_RESULT               result;                /* For error-checking */
static unsigned int              version;               /* FMOD version number */
static void                     *extraDriverData = 0;   /* Extra driver data for other hardware */

static SOUND_LIBRARY             sampleLibrary;         /* Holds SFX and other sample-type sounds */
static SOUND_LIBRARY             musicLibrary;          /* Holds music and other streamed sounds */

static int                       sampleLibSize = 100;   /* The size of the sample library */
static int                       musicLibSize = 100;    /* The size of the sample library */
/*-------------------------------------------------------------------------------------------------
END GLOBAL VARIABLES
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
SWITCHES
-------------------------------------------------------------------------------------------------*/

/* Set to true if you want to display error messages */
static bool displayErrors = true;
/*-------------------------------------------------------------------------------------------------
END SWITCHES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
FORWARD DECLARATIONS
-------------------------------------------------------------------------------------------------*/

void Audio_Initialize_FMOD(int channels);
void Audio_FreeSystem(void);
void Audio_AddSoundSample(char *sound);
void Audio_AddMusic(char *sound);
void Audio_LoadSoundSample(const char *sName, FMOD_SOUND **pSound);
void Audio_LoadMusic(const char *sName, FMOD_SOUND **pSound);
void Audio_PlaySoundSample(char *name, bool looping);
void Audio_PlayMusicStream(char *name, bool looping);
void Audio_PauseSound(char *name);

void Audio_Library_Init(SOUND_LIBRARY *library, unsigned int size);
SOUND_ENTRY *Audio_SearchHashTable(SOUND_LIBRARY *library, const char *name, unsigned int hash_code);
unsigned int Audio_CreateHashCode(unsigned int mod, const char *string);
/*-------------------------------------------------------------------------------------------------
END FORWARD DECLARATIONS
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
ERROR CHECKING FUNCTIONS
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : Audio_CheckFmodErrors
Description   : Checks for FMOD errors. If errors are found, opens error dialogue.
Input         : fmod_result is the result to test against.
                error_string specifies where the error was made or other details.
Output        : No output.
**************************************************************************************************/
void Audio_CheckFmodErrors(FMOD_RESULT fmod_result, char *error_string)
{
  /* If there is an error, convert the error code to a string and display it */
  if (displayErrors && result != FMOD_OK)
  {
    int msgboxID= MessageBox(
      NULL,
      FMOD_ErrorString(result),
      error_string,
      MB_ICONEXCLAMATION | MB_YESNO
      );
  }
}

/**************************************************************************************************
Function      : Audio_DisplayError
Description   : Opens error dialogue.
Input         : header is the header for the error box,
                error_string specifies details about the error.
Output        : No output.
**************************************************************************************************/
void Audio_DisplayError(char *header, char *error_string)
{
  /* If there is an error, convert the error code to a string and display it */
  if (displayErrors)
  {
    int msgboxID = MessageBox(
      NULL,
      error_string,
      header,
      MB_ICONEXCLAMATION | MB_YESNO
      );
  }
}
/*-------------------------------------------------------------------------------------------------
END ERROR CHECKING FUNCIONS
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : Audio_Initialize
Description   : Initializes the audio engine. Call during initialization of the system.
Input         : channels is the number of audio channels to use. Maximum is 4093.
Output        : No output.
**************************************************************************************************/
void Audio_Initialize(int channels)
{
  FILE *fp;                     /* File handler for init file */
  char buffer[PATH_MAX_LEN];   /* Buffer for reading from file */
  
  /*
    Mode for asset adding. 1 means add sample, 2 means 
  */
  int mode;

  /* Set up FMOD. */
  Audio_Initialize_FMOD(channels);

  /* Initialize the libraries. */
  Audio_Library_Init(&sampleLibrary, sampleLibSize);
  Audio_Library_Init(&musicLibrary, musicLibSize);

  /* Open the init file for read. */
  fopen_s(&fp, INIT_FILE, "rt");

  /* If file doesn't open properly, display error message and exit. */
  if (!fp)
  {
    Audio_DisplayError("Audio_Initialize error.\n", "Init file didn't open.\n");
    exit(-5);
  }

  /* Else, start adding assets the the libraries. */

  while (fread_s(buffer, PATH_MAX_LEN, sizeof(char), PATH_MAX_LEN, fp))
  {
    if (buffer)
    {
      /* If we get the signal to add samples, go to sample adding mode. */
      if (strcmp(buffer, "SAMPLES") == 0)
      {
        mode = INIT_ADD_SAMPLE;
        continue;
      }

      /* Else if we get the signal to add music, go to music adding mode. */
      else if (strcmp(buffer, "MUSIC") == 0)
      {
        mode = INIT_ADD_MUSIC;
        continue;
      }

      /* Else if we read in a new line, read again. */
      else if (strcmp(buffer, "\n") == 0)
        continue;

      /* Else, what we read is a file name. */

      /* If adding in sample mode, add a sample to the sample library. */
      if (mode == INIT_ADD_SAMPLE)
        Audio_AddSoundSample(buffer);

      /* Else if adding in music mode, add a music stream to the music library. */
      else if (mode == INIT_ADD_MUSIC)
        Audio_AddMusic(buffer);
    }
  }

  /* Close the file. */
  fclose(fp);

}

/*-------------------------------------------------------------------------------------------------
DATA STRUCTURE FUNCTIONALITY
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : Audio_Library_Init
Description   : Initializes a sound library by allocating memory for it and setting its size.
Input         : library is the pointer to the sound library to initialize,
                size is the size of the table to create.
Output        : No output.
**************************************************************************************************/
static void Audio_Library_Init(SOUND_LIBRARY *library, unsigned int size)
{
  /* Allocate memory for the hash table to use for our library. */
  SOUND_ENTRY **newLibrary = calloc(size, sizeof(SOUND_ENTRY *));

  /* If the allocation is successful, set the library's hash table and size. */
  if (newLibrary)
  {
    library->table = newLibrary;
    library->size = size;
  }
}

/**************************************************************************************************
Function      : Audio_SearchHashTable
Description   : Searches in a library for an item.
Input         : library is the sound library to search in,
                name is the name of the sound entry. Example: "sample.wav",
                hash_code is the sound entry's hash code. If not known, use NO_HASH as argument.
Output        : (SOUND_ENTRY *) Returns the address of the sound entry if found.
                Returns NULL if not found.
**************************************************************************************************/
static SOUND_ENTRY *Audio_SearchHashTable(SOUND_LIBRARY *library, const char *name, unsigned int hash_code)
{
  SOUND_ENTRY *entry;  /* Iterator for the entry list */

  /* If there isn't a hash code, figure out what it is. */
  if (hash_code == NO_HASH)
  {
    hash_code = Audio_CreateHashCode(library->size, name);
  }

  /* Get the first entry at the hashed index. */
  entry = library->table[hash_code];

  /* If there isn't an entry where we are looking, then the entry doesn't exist. Return NULL pointer. */
  if (!entry)
    return NULL;

  /* Otherwise, check the contents of the entry. */
  
  do
  {
    /* If the entry is the one that we are looking for, then return the address of this entry. */
    if (strcmp(entry->name, name) == 0)
      return entry;

    /* Else, move onto the next entry if it exists. */
    //if (entry->next)
      entry = entry->next;

  } while (entry);

  /* The item wasn't found. Return the NULL pointer. */
  return NULL;
}

/**************************************************************************************************
Function      : Audio_CreateHashCode
Description   : Generates a hash code from a string using a modular division.
Input         : mod is the number to use for modular division,
                string is the string to generate the code from.
Output        : (unsigned int) The hash code for the sound.
**************************************************************************************************/
static unsigned int Audio_CreateHashCode(unsigned int mod, const char *string)
{
  unsigned int code = 0;              /* The hash code. */
  int stringLenth = strlen(string);   /* The number of characters to process */
  int i;                              /* Iterator for the string. */

  /* Process each character in the string into the code. */
  for (i = 0; i < stringLenth; ++i)
    code = (code * HASH_KEY + string[i]) % mod;

  /* Return the generated hash code. */
  return code;
}

/**************************************************************************************************
Function      : Audio_CreateSoundEntry
Description   : Creates a new object of type SOUND_ENTRY.
Input         : name is the name of the file as it appears in the directory. Example: "sample.wav"
                sound is the FMOD sound object to put into this entry.
Output        : (SOUND_ENTRY *) Returns a pointer to the newly created sound entry.
                If unsuccessful, returns NULL.
**************************************************************************************************/
static SOUND_ENTRY *Audio_CreateSoundEntry(char *name, FMOD_SOUND *sound)
{
  /* Allocate memory for the new entry. */
  SOUND_ENTRY *newEntry = malloc(sizeof(SOUND_ENTRY));

  /* If the memory allocation is successful, continue constructing the entry. */
  if (newEntry)
  {
    /* Set the name of the new entry. */
    newEntry->name = name;
    //strcpy_s(newEntry->name, strlen(name) * sizeof(char), name);
    /* Set the FMOD sound. */
    newEntry->sound = sound;
    
    /* Set the rest of the data members to default values. */
    newEntry->hashCode = NO_HASH;
    newEntry->loaded = true;
    newEntry->next = NULL;
  }

  /* Return the pointer to the entry. */
  return newEntry;
}

/**************************************************************************************************
Function      : Audio_Library_AddEntry
Description   : Adds a SOUND_ENTRY into a sound library.
Input         : library is the sound library to add the sound to,
                sound_entry is the entry to add to the library.
Output        : No return.
**************************************************************************************************/
static void Audio_Library_AddEntry(SOUND_LIBRARY *library, SOUND_ENTRY *sound_entry)
{
  /* If the entry does not have a hash code, generate one for it. */
  if (sound_entry->hashCode == NO_HASH)
    sound_entry->hashCode = Audio_CreateHashCode(library->size, sound_entry->name);

  /* Push this entry onto the front of whatever is currently at the desired index. */

  /* Move everything behind this entry. */
  sound_entry->next = library->table[sound_entry->hashCode];

  /* Assign this entry to this position. */
  library->table[sound_entry->hashCode] = sound_entry;
}

/**************************************************************************************************
Function      : Audio_AddSoundSample
Description   : Adds a new sound sample to the SFX sample library.
Input         : sound is the name of the sound. Example: "sample.wav"
Output        : No return.
**************************************************************************************************/
void Audio_AddSoundSample(char *sound)
{
  FMOD_SOUND *tmpSound;   /* The FMOD sound to create. */

  /* Search in the sound library for the sound. */
  SOUND_ENTRY *searchResult = Audio_SearchHashTable(&sampleLibrary, sound, NO_HASH);

  /* If the sound is already in the sample library, then return. */
  if (searchResult)
    return;

  /* Else, create the sound and add it to the sample library. */
  
  Audio_LoadSoundSample(sound, &tmpSound);

  SOUND_ENTRY *newEntry = Audio_CreateSoundEntry(sound, tmpSound);

  /* Put the entry into the sample library. */
  Audio_Library_AddEntry(&sampleLibrary, newEntry);
}

/**************************************************************************************************
Function      : Audio_AddMusic
Description   : Adds a new stream to the music/stream library.
Input         : sound is the name of the sound. Example: "sample.wav"
Output        : No return.
**************************************************************************************************/
void Audio_AddMusic(char *sound)
{
  FMOD_SOUND *tmpSound;   /* The FMOD sound to create. */

  /* Search in the sound library for the sound. */
  SOUND_ENTRY *searchResult = Audio_SearchHashTable(&sampleLibrary, sound, NO_HASH);

  /* If the sound is already in the music library, then return. */
  if (searchResult)
    return;

  /* Else, create the sound and add it to the music library. */

  Audio_LoadMusic(sound, &tmpSound);

  SOUND_ENTRY *newEntry = Audio_CreateSoundEntry(sound, tmpSound);

  /* Put the entry into the music library. */
  Audio_Library_AddEntry(&musicLibrary, newEntry);
}

/**************************************************************************************************
Function      : Audio_Library_Free
Description   : Releases all of the sounds in a library from memory.
Input         : library is the sound library to free.
Output        : No return.
**************************************************************************************************/
static void Audio_Library_Free(SOUND_LIBRARY *library)
{
  /* Free all entries in the library. */
  unsigned int i;
  for (i = 0; i < library->size; ++i)
  {
    SOUND_ENTRY *entry = library->table[i];
    while (entry)
    {
      FMOD_Sound_Release(entry->sound);
      entry = entry->next;
    }
  }
}
/*-------------------------------------------------------------------------------------------------
END DATA STRUCTURE FUNCTIONALITY
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
BACK-END FMOD STUFF 
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : Audio_Initialize_FMOD
Description   : Initializes FMOD.
Input         : channels is the number of audio channels to use. Maximum is 4093.
Output        : No output.
**************************************************************************************************/
void Audio_Initialize_FMOD(int channels)
{
  /* Create an FMOD system. */
  result = FMOD_System_Create(&fmodSystem);
  Audio_CheckFmodErrors(result, "Audio_Initialize_FMOD error 1.\n");

  /* Check the version. */
  result = FMOD_System_GetVersion(fmodSystem, &version);
  Audio_CheckFmodErrors(result, "Audio_Initialize_FMOD error 2.\n");

  /* If not the correct version, fatal error is thrown. */
  // CREATE THE FATAL THROW
  //if (version < FMOD_VERSION)
  //{
  //  Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
  //}

  /* Initialize the FMOD system. */
  result = FMOD_System_Init(fmodSystem, channels, FMOD_INIT_NORMAL, extraDriverData);
  Audio_CheckFmodErrors(result, "Audio_Initialize_FMOD error 3.\n");
}

/**************************************************************************************************
Function      : Audio_FreeSystem
Description   : Frees the audio libraries, then closes the audio system 
                and then releases it from memory.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Audio_FreeSystem(void)
{
  // Free the audio libraries
  Audio_Library_Free(&sampleLibrary);
  Audio_Library_Free(&musicLibrary);

  /* Close the system. */
  result = FMOD_System_Close(fmodSystem);
  Audio_CheckFmodErrors(result, "Audio_FreeSystem error1.\n");

  /* Release the system from memory. */
  result = FMOD_System_Release(fmodSystem);
  Audio_CheckFmodErrors(result, "Audio_FreeSystem error2.\n");
}

/**************************************************************************************************
Function      : Audio_LoadSoundSample
Description   : Loads a sound as a sample from a file.
Input         : sName is the name of the sound file including extensions. Example: "sample.wav"
                pSound is pointer to the FMod sound object to use for the sound.
Output        : No output.
**************************************************************************************************/
void Audio_LoadSoundSample(const char *sName, FMOD_SOUND **pSound)
{
  /* Generate the path for the sound file. */

  char *path = (char *)calloc(PATH_MAX_LEN, sizeof(char));
  strcat_s(path, PATH_MAX_LEN * sizeof(char), SOUND_SAMPLE_DIRECTORY);
  strcat_s(path, PATH_MAX_LEN * sizeof(char), sName);

  /* Create a sound using the generated path. */
  result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, NULL, pSound);
  Audio_CheckFmodErrors(result, "Audio_LoadSoundSample error.\n");
}

/**************************************************************************************************
Function      : Audio_LoadMusic
Description   : Loads a music file as a stream.
Input         : sName is the name of the sound file including extensions. Example: "sample.wav"
                pSound is pointer to the FMod sound object to use for the sound.
Output        : No output.
**************************************************************************************************/
void Audio_LoadMusic(const char *sName, FMOD_SOUND **pSound)
{
  /* Generate the path for the sound file. */
  char *path = (char *)calloc(PATH_MAX_LEN, sizeof(char));
  strcat_s(path, PATH_MAX_LEN * sizeof(char), SOUND_MUSIC_DIRECTORY);
  strcat_s(path, PATH_MAX_LEN * sizeof(char), sName);

  /* Create a sound using the generated path. */
  result = FMOD_System_CreateStream(fmodSystem, path, FMOD_LOOP_NORMAL, NULL, pSound);
  Audio_CheckFmodErrors(result, "Audio_LoadMusic error.\n");
}

/**************************************************************************************************
Function      : Audio_PlaySoundSample
Description   : Plays a sound sample. Use for sound effects.
Input         : name is the name of the sound to play. Example: "sample.wav"
                loop is whether or not to loop the sound.
Output        : No output.
**************************************************************************************************/
void Audio_PlaySoundSample(char *name, bool loop)//FMOD_SOUND *pSound)
{
  if (!Audio_SearchHashTable(&sampleLibrary, name, NO_HASH))
    Audio_AddSoundSample(name);

  /* Get the FMOD sound object. */
  FMOD_SOUND *sound = Audio_SearchHashTable(&sampleLibrary, name, NO_HASH)->sound;

  /* Set the looping mode of the sound. */
  if (loop)
    FMOD_Sound_SetMode(sound, FMOD_LOOP_NORMAL);
  else
    FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF);

  /* Play the sound. */
  result = FMOD_System_PlaySound(fmodSystem, sound, NULL, false, &channel);
  Audio_CheckFmodErrors(result, "Audio_PlaySound error.\n");
}

/**************************************************************************************************
Function      : Audio_PlayMusicStream
Description   : Plays a sound stream. Use for music.
Input         : name is the name of the sound stream to play. Example: "sample.wav"
                loop is whether or not to loop the stream.
Output        : No output.
**************************************************************************************************/
void Audio_PlayMusicStream(char *name, bool loop)//FMOD_SOUND *pSound)
{
  /* If the music is not in the library, add it */
  if (!Audio_SearchHashTable(&musicLibrary, name, NO_HASH))
    Audio_AddMusic(name);

  /* Get the FMOD sound object. */
  FMOD_SOUND *sound = Audio_SearchHashTable(&musicLibrary, name, NO_HASH)->sound;

  /* Set the looping mode of the sound. */
  if (loop)
    FMOD_Sound_SetMode(sound, FMOD_LOOP_NORMAL);
  else
    FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF);

  /* Play the sound. */
  result = FMOD_System_PlaySound(fmodSystem, sound, NULL, false, &channel);
  Audio_CheckFmodErrors(result, "Audio_PlaySound error.\n");
}

/**************************************************************************************************
Function      : Audio_PauseSound
Description   : Pauses playback of a sound.
Input         : name is the name of the sound to pause. Example: "sample.wav"
Output        : No output.
**************************************************************************************************/
void Audio_PauseSoundSample(char *name)
{
  if (!Audio_SearchHashTable(&sampleLibrary, name, NO_HASH))
    return;

  /* Get the FMOD sound object. */
  FMOD_SOUND *sound = Audio_SearchHashTable(&sampleLibrary, name, NO_HASH)->sound;

  /* Pause playback of the sound. */
  result = FMOD_System_PlaySound(fmodSystem, sound, NULL, true, &channel);
  Audio_CheckFmodErrors(result, "Audio_PauseSound error.\n");
}

/**************************************************************************************************
Function      : Audio_PauseMusicStream
Description   : Pauses playback of a stream.
Input         : name is the name of the stream to pause. Example: "sample.wav"
Output        : No output.
**************************************************************************************************/
void Audio_PauseMusicStream(char *name)
{
  if (!Audio_SearchHashTable(&musicLibrary, name, NO_HASH))
    return;

  /* Get the FMOD sound object. */
  FMOD_SOUND *sound = Audio_SearchHashTable(&musicLibrary, name, NO_HASH)->sound;

  /* Pause playback of the sound. */
  result = FMOD_System_PlaySound(fmodSystem, sound, NULL, true, &channel);
  Audio_CheckFmodErrors(result, "Audio_PauseSound error.\n");
}

/**************************************************************************************************
Function      : Audio_UpdatePlayback
Description   : Updates the audio system for playback.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Audio_UpdatePlayback(void)
{
  /* Update the FMod audio system. */
  result = FMOD_System_Update(fmodSystem);
  Audio_CheckFmodErrors(result, "Audio_UpdatePlayback error 1.\n");

  {
    unsigned int ms = 0;                /* The current position of the sound being played in milliseconds. */
    unsigned int lenms = 0;             /* The length of the sound being played in milliseconds. */  
    bool         playing = 0;           /* The channel's playback status. */
    bool         paused = 0;            /* The channel's paused status. */
    int          channelsplaying = 0;   /* The number of channels that are currently playing. */

    /* Check the channels, make sure they're okay. */
    if (channel)
    {
      FMOD_SOUND *currentsound = 0;

      result = FMOD_Channel_IsPlaying(channel, &playing);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        Audio_CheckFmodErrors(result, "Audio_UpdatePlayback error 2.\n");
      }

      result = FMOD_Channel_IsPlaying(channel, &paused);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        Audio_CheckFmodErrors(result, "Audio_UpdatePlayback error 3.\n");
      }

      result = FMOD_Channel_GetPosition(channel, &ms, FMOD_TIMEUNIT_MS);
      if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
      {
        Audio_CheckFmodErrors(result, "Audio_UpdatePlayback error 4.\n");
      }

      FMOD_Channel_GetCurrentSound(channel, &currentsound);
      if (currentsound)
      {
        result = FMOD_Sound_GetLength(currentsound, &lenms, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
          Audio_CheckFmodErrors(result, "Audio_UpdatePlayback error 5.\n");
        }
      }
    }
    /* Get the channels that are playing. */
    FMOD_System_GetChannelsPlaying(fmodSystem, &channelsplaying);
  }
}

/*-------------------------------------------------------------------------------------------------
END BACK-END FMOD STUFF
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
TEST FUNCTIONS
-------------------------------------------------------------------------------------------------*/

// Testing code
void TestAudioINIT(void)
{
  //Initialize the sound system
  Audio_Initialize_FMOD(32);

  Audio_Library_Init(&sampleLibrary, sampleLibSize);
  Audio_Library_Init(&musicLibrary, musicLibSize);

  // Test loading of one sample, one stream
  Audio_AddSoundSample("sample1.ogg");
  Audio_AddMusic("music_sample1.ogg");

  // Test loading of multiple samples, multiple streams
  Audio_AddSoundSample("sample2.ogg");
  Audio_AddSoundSample("sample3.ogg");
  Audio_AddMusic("music_sample2.ogg");

  // Test playback of one sample, one stream
  //Audio_PlaySoundSample("sample1.ogg", false);
  //Audio_PlaySound("music_sample1.ogg", false);

  // Test playback of multiple samples, multiple streams (will be cacophonous)
  //Audio_PlaySoundSample("sample2.ogg", false);
  //Audio_PlaySoundSample("sample3.ogg", false);
  //Audio_PlayMusicStream("music_sample2.ogg", false);

}

/*-------------------------------------------------------------------------------------------------
END TEST FUNCTIONS
-------------------------------------------------------------------------------------------------*/

