#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "ControlScreen.h"


void ControlScreenInit()
{
  TextInit();
  TextCreateString("!", -200, 0);
}

void ControlScreenRun()
{
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_mainMenu);
  }
}