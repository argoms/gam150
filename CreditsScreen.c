
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include <stdio.h>
#include "Button.h"
#include "WinScreen.h"


void CreditsScreenInit()
{
  TextInit();

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

  /* Matt - handler to created text. */
  TextString *text;

  // text for the creidts
  text = TextCreateString("Credits", winMinX + screenWidth / 2, winMinY + 9 * screenHeight / 10);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

  text = TextCreateString("Producer:", winMinX + screenWidth / 5, winMinY + screenHeight / 2 + 170);
  TextStringSetTint(text, GTint(1, 1, 1, 1));
  text = TextCreateString("Noah Park", winMinX + screenWidth / 5, winMinY + screenHeight / 2 + 130);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

  text = TextCreateString("Tech Director:", winMinX + screenWidth / 5, winMinY + screenHeight / 2 + 80);
  TextStringSetTint(text, GTint(1, 1, 1, 1));
  text = TextCreateString("James Do", winMinX + screenWidth / 5, winMinY + screenHeight / 2 + 30);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

  text = TextCreateString("Product Manager:", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 20);
  TextStringSetTint(text, GTint(1, 1, 1, 1));
  text = TextCreateString("Matt Chasengnou", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 60);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

  text = TextCreateString("Design Director:", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 110);
  TextStringSetTint(text, GTint(1, 1, 1, 1));
  text = TextCreateString("Jack Ile", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 150);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

  text = TextCreateString("Test Manager:", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 200);
  TextStringSetTint(text, GTint(1, 1, 1, 1));
  text = TextCreateString("Tarrant Mangasarian", winMinX + screenWidth / 5, winMinY + screenHeight / 2 - 240);
  TextStringSetTint(text, GTint(1, 1, 1, 1));

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
  float buttony = 225;                           /* y position      */
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
}

void CreditsScreenRun()
{
  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
}