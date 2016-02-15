/******************************************************************
Author        :  Matt Chasengnou
File          :  Audio.h
Created       :  1/25/16
Description   :  Contains forward declarations used by Audio.c
ChangeLog
  -2/3/16     : Added LoadSoundSample, LoadSoundStream, PlaySound,
                and StopSound functions.
  -
******************************************************************/
#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>

/* Initializes FMod for audio playback. */
void InitializeAudio(void);

/*
Loads a sound as a sample from a file.
Include extensions in name. ex: "drums.wav"
pSound is pointer to the sound object you will store the sound in.
*/
void LoadSoundSample(const char *pathName, FMOD_SOUND **pSound);

/*
Loads a sound as a stream.
Include extensions in name. ex: "drums.wav"
pSound is pointer to the sound object you will store the sound in.
*/
void LoadMusic(const char *pathName, FMOD_SOUND **pSound);

/*
Plays a sound.
pSound is pointer to sound you want to play.
*/
void PlayAudio(FMOD_SOUND *pSound);

/*
Stops playback of a sound.
*/
void StopSound(FMOD_SOUND *pSound);
#endif // !AUDIO_H

void FreeSound();
void TestAudioINIT(void);
void UpdateAudio();