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
#include "Button.h"


void TownScreenInit()
{
  TextInit();
  printf("\nWELCOME TO TOWN");
  TextCreateString("PLACEHOLDER TOWN", -200, 0);
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
  if (AEInputCheckTriggered(VK_SPACE))
  {
    LevelSetNext(level_level1);
  }
}