/*!
\file   WinScreen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions handling the death screen
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "WinScreen.h"


void WinScreenInit()
{
  TextInit();
  TextCreateString("YOU WON!", -200, 0);
  TextCreateString("PRESS SPACE TO RETURN", -360, -50);
  TextCreateString("TO MAIN MENU", -300, -100);
}

void WinScreenRun()
{
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_mainMenu);
  }
}