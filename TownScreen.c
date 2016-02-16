/*!
\file   TownScreen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions handling the town menu/screen portion of the game where the player restocks, changes equipment etc.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>

void TownScreenInit()
{
  TextInit();
  printf("\nWELCOME TO TOWN");
  TextCreateString("PLACEHOLDER TOWN", -200, 0);
  TextCreateString("PRESS SPACE TO LEAVE", -300, -50);
}

void TownScreenRun()
{
  if (AEInputCheckTriggered(VK_SPACE))
  {
    LevelSetNext(level_level1);
  }
}