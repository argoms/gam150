/*!
Project (working title): Epoch
\file   DeathScreen.c
\author Tarrant Mangasarian, James Do
\par    email: j.do\@digipen.edu
\par    email: tarrant.mangasarian\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Functions handling the death screen
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "DeathScreen.h"
#include "stats.h"
#include "PlayerEntity.h"

void DeathScreenInit()
{
  
  TextInit();
  TextCreateString("YOU DIED", -300, 0);
  //TextCreateString("PRESS SPACE TO RETURN", -360, -50);
  //TextCreateString("TO MAIN MENU", -300, -100);
  
  //PrintStats(GetPlayerStats()); // print the player stats
  //TARRANT CODE START
  //-----------------Tarant text------------------------------
  //text string
  
  

  // window size vars
  float winMaxX;
  float winMaxY;
  float winMinX;
  float winMinY;

  // get the window values
  winMaxX = AEGfxGetWinMaxX();
  winMaxY = AEGfxGetWinMaxY();
  winMinX = AEGfxGetWinMinX();
  winMinY = AEGfxGetWinMinY();

  // for my sanity
  float screenWidth = winMaxX - winMinX;
  float screenHeight = winMaxY - winMinY;

  // string positions
  float string_xpos = 0.0f; // default x
  float string_ypos = 0.0f; // default y

  string_xpos += 0;
  string_ypos += 0.25f * screenHeight;

  PrintStats(GetPlayerStats());

  //TextString* textString;
  //textString = TextCreateString("EPOCH", string_xpos, string_ypos); // title

 //BUTTONS------------------------------------------------------
 // BUTTON 1

  int button_type = MAIN_MENU_BUTTON;             /* type of button  */
  float buttonx = -200;                           /* x position      */
  float buttony = 100;                           /* y position      */
  float meshx = 256.0f;                           /* mesh x          */
  float meshy = 64.0f;                            /* mesh y          */
  float buttonsize = 1.0f;                        /* size            */
  static AEGfxVertexList*	button_mesh;				    /* mesh ptr        */
  button_mesh = GCreateMesh(meshx, meshy, 1, 1);  /* create the mesh */

  float text_offset = 90.0f;
  TextString *main_menu_text;
  main_menu_text = TextCreateHUDString(" Menu", buttonx - text_offset, buttony);
  //TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
  AEGfxTexture* button_texture = GCreateTexture("animations/buttons/button_texture2.png");
  Animation* anim_button = GCreateAnimation(1,
    button_texture,   //was null
    button_mesh,
    1);

  Sprite *button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
  //button1_sprite->offset.y = 10000;
  //PhysicsObject *button1_physics = PhysicsCreateObject(Vec2(button1x,button1y),1);

  GameObject* button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  //end button level 1 ------------------------------
}

void DeathScreenRun()
{
  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
  /*
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_mainMenu);
  }
  */
}