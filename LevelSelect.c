#include "LevelSelect.h"
#include "Graphics.h"
#include "AEEngine.h"
//#include "LevelManager.h"
#include "Text.h"
//#include <stdio.h>
#include "Button.h"
extern int level;
void LevelSelectScreenInit()
{
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

  TextString* title;
  title = TextCreateHUDString("Level Select",0,0.25 * screenHeight );
  TextStringSetTint(title, GTint(1, 1, 1, 1));


  //BUTTONS------------------------------------------------------
  // BUTTON LEVEL 1

  int button_type = LEVEL_ONE_BUTTON;             /* type of button  */
  float buttonx = -200;                           /* x position      */
  float buttony = 100;                            /* y position      */
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

  
  

  //end button level 1 ------------------------------

  //BUTTON TWO for level 2

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

  
  // --------------------end button level 2-----------------------
  //BUTTON three for level 3

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
  
  //---------------------------end button level 3-------------------------
  //BUTTON four for level 4

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
  
  //end button 4------------------------------------------------------------------
  //main menu button------------------------------------------------------------

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

  // end main menu  ------------------------------------------------------------
    //end buttons
}

void LevelSelectScreenRun()
{
  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
}