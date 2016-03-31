/*!
Project (working title): Epoch
\file   TownScreen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions handling the intermission portion of the game between levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"

extern int level;
void TownScreenInit()
{
  TextInit();
  printf("\nWELCOME TO TOWN");
  char levelIndicator[3];
  itoa(level, levelIndicator, 10);
  TextCreateString(levelIndicator, -350, 50);
  TextCreateString("PLACEHOLDER INTERMISSION", -350, 0);
  TextCreateString("PRESS SPACE TO LEAVE", -300, -50);


  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  /*
  PhysicsObject *button_physics = PhysicsCreateObject(Vec2(25, 25), 1);
  Sprite *sprite_object_button = GCreateSprite(0, 40, anim2, 1);
  //int entity_butt = entity_button;
  int button_type = MAIN_MENU_BUTTON;
  GameObject* button = CreateButton(button_physics, sprite_object_button, NULL, button_type);
  */
}

void TownScreenRun()
{
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_level1);
  }
}