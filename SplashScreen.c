/*!
\file   DeathScreen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions handling the splash screen
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "SplashScreen.h"
#include "Audio.h"


void SplashScreenInit()
{
  TextInit();
  GCreateSprite(0, 0, GCreateAnimation(1, GCreateTexture("splash.png"), GCreateMesh(800, 600, 1, 1), 1), 1);
  TextCreateString("", -300, -50);
  //printf("AAA");
  printf("AAA");
  Audio_AddSoundSample("SwordSwing1.ogg");
  Audio_AddSoundSample("SwordClash1.ogg");
  Audio_AddSoundSample("SwordSwing2.ogg");
  Audio_AddSoundSample("ButtonClick.ogg");
  Audio_AddSoundSample("ButtonClick2.ogg");
  Audio_AddSoundSample("music_sample1.ogg");
  Audio_PlayMusicStream("music_sample1.ogg", 0);
}

void SplashScreenRun()
{
  
  //printf("AAA");
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_mainMenu);
    //Audio_PlaySoundSample("SwordSwing1.ogg", 0);
  }
}