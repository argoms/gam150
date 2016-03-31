/*!
Project (working title): Epoch
\file   SplashScreen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions handling the starting splash screen

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "SplashScreen.h"
#include "Audio.h"

static float time = 2;
static Sprite* splashScreen;
void SplashScreenInit()
{
  TextInit();
  splashScreen = GCreateSprite(0, 0, GCreateAnimation(1, GCreateTexture("splash.png"), GCreateMesh(800, 600, 1, 1), 1), 1);
  TextCreateString("", -300, -50);
  //printf("AAA");
  printf("AAA");
  
}

void SplashScreenRun()
{
 
  time -= AEFrameRateControllerGetFrameTime();

  if (time < 1.5f)
  {
    splashScreen->tint.blue = time / 1.5f;
    splashScreen->tint.red = time / 1.5f;
    splashScreen->tint.green = time / 1.5f;
    if (time < 0)
    {
      LevelSetNext(level_mainMenu);
    }
  }
  //printf("AAA");
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_mainMenu);
    //Audio_PlaySoundSample("SwordSwing1.ogg", 0);
  }
}