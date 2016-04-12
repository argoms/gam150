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

// button pointers
static GameObject* level1_button;
static GameObject* level2_button;
static GameObject* level3_button;
static GameObject* level4_button;
static GameObject* level5_button;// or main menu button

 // button visibility flags
static int button_flag_level1 = 0;
static int button_flag_level2 = 0;
static int button_flag_level3 = 0;
static int button_flag_level4 = 0;


void TownScreenInit()
{
  TextInit();
  //printf("\nWELCOME TO TOWN");
  //char levelIndicator[3];
  //itoa(level, levelIndicator, 10);
  //TextCreateString(levelIndicator, -350, 50);
  TextString* cleared = TextCreateString("Level Cleared!", 50, 0);
  TextStringSetTint(cleared, GTint(1, 1, 1, 1));
  //TextCreateString("PRESS SPACE TO LEAVE", -300, -50);


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



  //BUTTONS------------------------------------------------------
  // BUTTON LEVEL 1
  //if (button_flag_level1)
  //{
    int button_type = LEVEL_ONE_BUTTON;             /* type of button  */
    float buttonx = -200;                           /* x position      */
    float buttony = 100;                           /* y position      */
    float meshx = 256.0f;                           /* mesh x          */
    float meshy = 64.0f;                            /* mesh y          */
    float buttonsize = 1.0f;                        /* size            */
    static AEGfxVertexList*	button_mesh;				    /* mesh ptr        */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);  /* create the mesh */

    float text_offset = 90.0f;
    TextString *main_menu_text;
    main_menu_text = TextCreateHUDString("Level 1", buttonx - text_offset, buttony);
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
    level1_button = button;
  //}
  //end button level 1 ------------------------------

  //BUTTON TWO for level 2
  //if (button_flag_level2)
  //{
    button_type = LEVEL_TWO_BUTTON;          /* type of button  */
    buttonx = -200;                          /* x position      */
    buttony = 25;                            /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Level 2", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
    level2_button = button;
  //}
  // --------------------end button level 2-----------------------
  //BUTTON three for level 3
  //if (button_flag_level3)
  //{
    button_type = LEVEL_THREE_BUTTON;          /* type of button  */
    buttonx = -200;                          /* x position      */
    buttony = -50;                          /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Level 3", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
    level3_button = button;
  //}
  //---------------------------end button level 3-------------------------
  //BUTTON four for level 4
  //if (button_flag_level4)
  //{
    button_type = LEVEL_FOUR_BUTTON;         /* type of button  */
    buttonx = -200;                          /* x position      */
    buttony = -125;                          /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Level 4", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
    level4_button = button;
  //}
  //end button 4------------------------------------------------------------------


  //BUTTON  for main menu

  button_type = MAIN_MENU_BUTTON;         /* type of button  */
  buttonx = -200;                          /* x position      */
  buttony = -200;                          /* y position      */
  meshx = 256.0f;                          /* mesh x          */
  meshy = 64.0f;                           /* mesh y          */
  buttonsize = 1.0f;                       /* size            */
  button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
  text_offset = 90.0f;
  main_menu_text = TextCreateHUDString("Menu", buttonx - text_offset, buttony);
  TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
  button_texture = GCreateTexture("animations/buttons/button_texture.png");
  anim_button = GCreateAnimation(1,
    button_texture,
    button_mesh,
    1);

  button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
  button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  level5_button = button;
  //end button 4------------------------------------------------------------------

  //we are going to level 2
  if (level == 2)
  {
    button_flag_level1 = 0;
    button_flag_level2 = 1;
    button_flag_level3 = 0;
    button_flag_level4 = 0;

    ReactivateAndDisplayButton(level1_button);
    ReactivateAndDisplayButton(level2_button);
    FadeAndDisableButton(level3_button);
    FadeAndDisableButton(level4_button);
  }
  // we are going to level 3
  else if (level == 3)
  {
    button_flag_level1 = 0;
    button_flag_level2 = 0;
    button_flag_level3 = 1;
    button_flag_level4 = 0;

    ReactivateAndDisplayButton(level1_button);
    ReactivateAndDisplayButton(level2_button);
    ReactivateAndDisplayButton(level3_button);
    FadeAndDisableButton(level4_button);
  }
  // we are going to level 4
  else if (level == 4)
  {
    button_flag_level1 = 0;
    button_flag_level2 = 0;
    button_flag_level3 = 0;
    button_flag_level4 = 0;

    ReactivateAndDisplayButton(level1_button);
    ReactivateAndDisplayButton(level2_button);
    ReactivateAndDisplayButton(level3_button);
    ReactivateAndDisplayButton(level4_button);
  }
  // we are going to level 5 or the winscreen
  else if (level == 5)
  {
    button_flag_level1 = 0;
    button_flag_level2 = 0;
    button_flag_level3 = 0;
    button_flag_level4 = 1;

    FadeAndDisableButton(level1_button);
    //ReactivateAndDisplayButton();
    FadeAndDisableButton(level2_button);
    FadeAndDisableButton(level3_button);
    FadeAndDisableButton(level4_button);
  }



}


void TownScreenRun()
{
  /*
  if (AEInputCheckReleased(VK_SPACE))
  {
    LevelSetNext(level_level1);
  }
  */
  //TARRANT CODE START




  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
  
  //-----------------Tarant text------------------------------
  /*
  TextInit();
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

  TextString* textString;
  //textString = TextCreateString("EPOCH", string_xpos, string_ypos); // title
  //TextStringSetTint(textString, GTint(1, 1, 1, 1));



  int button_type;
  float buttonx;
  float buttony;
  float meshx;
  float meshy;
  float buttonsize;
  static AEGfxVertexList*	button_mesh;

  float text_offset;
  TextString *main_menu_text;
  AEGfxTexture* button_texture;
  Animation* anim_button;
  Sprite *button_sprite;
  //button1_sprite->offset.y = 10000;
  //PhysicsObject *button1_physics = PhysicsCreateObject(Vec2(button1x,button1y),1);

  GameObject* button;

  */





}
