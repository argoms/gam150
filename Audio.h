/*!
Project (working title): Epoch
\file   Audio.h
\author Matt Chasengnou
\par    email: chasengnou.m@digipen.edu
\par    Course: GAM150
\par    Created: 1/25/16
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Contains forward declarations used by Audio.c

\ChangeLog
-2/3/16     : Added LoadSoundSample, LoadSoundStream, PlaySound,
and StopSound functions.
-2/16/16    : Cleaned up file.
Implemented black-box approach to accessing audio
*/

#ifndef AUDIO_H
#define AUDIO_H

#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

typedef int bool;

/**************************************************************************************************
Function      : Audio_Initialize
Description   : Initializes the audio engine. Call during system initialization.
Input         : channels is the number of audio channels to use. Maximum is 4093.
Output        : No output.
**************************************************************************************************/
void Audio_Initialize(int channels);

/**************************************************************************************************
Function      : Audio_FreeSystem
Description   : Frees the audio engine from memory. Call this during system free.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Audio_FreeSystem(void);

/**************************************************************************************************
Function      : Audio_PlaySoundSample
Description   : Plays a sound sample. Use for sound effects.
Input         : name is the name of the sound to play. Example: "sample.wav"
                loop is whether or not to loop the sound.
Output        : No output.
**************************************************************************************************/
void Audio_PlaySoundSample(char *name, bool loop);

/**************************************************************************************************
Function      : 

PlayMusicStream
Description   : Plays a sound stream. Use for music.
Input         : name is the name of the sound stream to play. Example: "sample.wav"
                loop is whether or not to loop the stream.
Output        : No output.
**************************************************************************************************/
void Audio_PlayMusicStream(char *name, bool loop);

/**************************************************************************************************
Function      : Audio_PauseSound
Description   : Pauses playback of a sound.
Input         : name is the name of the sound to pause. Example: "sample.wav"
Output        : No output.
**************************************************************************************************/
void Audio_PauseSoundSample(char *name);

/**************************************************************************************************
Function      : Audio_PauseMusicStream
Description   : Pauses playback of a stream.
Input         : name is the name of the stream to pause. Example: "sample.wav"
Output        : No output.
**************************************************************************************************/
void Audio_PauseMusicStream(char *name);

/**************************************************************************************************
Function      : Audio_UpdatePlayback
Description   : Updates the audio system for playback. Call in the game update loop after
                doing draw calls.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Audio_UpdatePlayback(void);

// Testing functions
void TestAudioINIT(void);

#endif // !AUDIO_H

