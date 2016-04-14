/*!
Project (working title): Epoch
\file   ControlScreen.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
This file contains the function implementation to set up the controls screen
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "ControlScreen.h"

//test stats



void ControlScreenInit()
{

  TextInit();
  //TextString *text = TextCreateString("Menu", -200, 0);
  //TextStringSetTint(text, GTint(1, 1, 1, 1));
  //TextCreateString("PRESS SPACE TO RETURN", -360, -50);
  //TextCreateString("TO MAIN MENU", -300, -100);


  
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
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
  AEGfxTexture* button_texture = GCreateTexture("animations/buttons/button_texture.png");
  Animation* anim_button = GCreateAnimation(1,
    button_texture,   //was null
    button_mesh,
    1);

  Sprite *button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
  //button1_sprite->offset.y = 10000;
  //PhysicsObject *button1_physics = PhysicsCreateObject(Vec2(button1x,button1y),1);

  GameObject* button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  //end button level 1 ------------------------------

  text_offset = 550;

  main_menu_text = TextCreateHUDString("WASD to Move", buttonx - text_offset, buttony -75);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));

  main_menu_text = TextCreateHUDString("Left click to attack, mouse to aim", buttonx - text_offset, buttony - 150);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));

  main_menu_text = TextCreateHUDString("Spacebar or right click to dodge", buttonx - text_offset, buttony - 225);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));

  main_menu_text = TextCreateHUDString("Attack during a dodge for a damage boost", buttonx - text_offset, buttony - 300);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));

  main_menu_text = TextCreateHUDString("Dodge-attacks require careful timing... ", buttonx - text_offset, buttony - 375);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
}

void ControlScreenRun()
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