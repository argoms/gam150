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
#include <Windows.h>

#define SCREEN_TIME 6.f

static float time = SCREEN_TIME;
static Sprite* splashScreen;
void SplashScreenInit()
{
  TextInit();
  splashScreen = GCreateSprite(0, 0, GCreateAnimation(1, GCreateTexture("splash.png"), GCreateMesh(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1, 1), 1), 1);
  //TextCreateString("", -300, -50);
  //printf("AAA");
  printf("AAA");
  
  /* Matt's fade-in code. */
  {
    splashScreen->tint.blue = 0;
    splashScreen->tint.red = 0;
    splashScreen->tint.green = 0;
  }

}

void SplashScreenRun()
{
 
  time -= AEFrameRateControllerGetFrameTime();

  /* Matt's fade-in code */
  {
    if (time > SCREEN_TIME * 0.75)
    {
      float deltaTime = (float)AEFrameRateControllerGetFrameTime();
      splashScreen->tint.blue += deltaTime * 4.f / SCREEN_TIME;// / 3.f;
      splashScreen->tint.red += deltaTime * 4.f / SCREEN_TIME;// / 3.f;
      splashScreen->tint.green += deltaTime * 4.f / SCREEN_TIME;// / 3.f;

      if (splashScreen->tint.blue >= 1 || time - deltaTime <= SCREEN_TIME * 0.75)
      {
        splashScreen->tint.blue = 1;
        splashScreen->tint.red = 1;
        splashScreen->tint.green = 1;
      }
    }
  }

  if (time < SCREEN_TIME * 0.25f)
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