/*!
\file   LevelManager.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic level/gamestate manager implementation.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include "GameLevel.h"
#include "TownScreen.h"
#include "Button.h"
#include "DeathScreen.h"
#include "SplashScreen.h"
#include "WinScreen.h"
#include "Audio.h"
//EXAMPLE VARIABLES, NOT STRICTLY NEEDED
static AEGfxVertexList*	pMesh2;				/**< EXAMPLE VAR*/
static AEGfxTexture *pTex1;/**< EXAMPLE VAR*/
static AEGfxTexture *pTex2;/**< EXAMPLE VAR*/

static Sprite* sprite; /**< EXAMPLE VAR*/ //only one for debug purposes
static Animation* animtest;/**< EXAMPLE VAR*/
static Animation* animtest2;/**< EXAMPLE VAR*/

static float textY;/**< EXAMPLE VAR*/
static TextString* textString;/**< EXAMPLE VAR*/
//EXAMPLE CODE ENDS HERE


static int currentLevel;/**< Current level (uses enum)*/
static int nextLevel;/**derp< Level to switch to (if not equal to current level) (uses enum)*/

extern int gGameRunning; /**< used to interface with main file*/

double frameTime;

/*
\brief loads given level
\param _level level to be loaded
*/
void LevelLoad(int _level)
{
  GInitialize();

  switch (_level)
  {
  case level_level1:
    Level1Init();
    break;
  case level_mainMenu:
    MainMenuInit();
    break;
  case level_town:
    TownScreenInit();
    break;
  case level_deathScreen:
    DeathScreenInit();
    break;
  case level_splashScreen:
    SplashScreenInit();
    break;
  case level_winScreen:
    WinScreenInit();
    break;
  }

}

/*
\brief level game loop
*/
void LevelRun()
{
  frameTime = AEFrameRateControllerGetFrameTime();
  switch (currentLevel)
  {
    
  case level_level1:
    GameLevelRun();
    break;
  case level_mainMenu:
    MainMenuRun();
    break;
  case level_town:
    TownScreenRun();
    break;
  case level_deathScreen:
    DeathScreenRun();
    break;
  case level_splashScreen:
    SplashScreenRun();
    break;
  case level_winScreen:
    WinScreenRun();
    break;
  }

  if (currentLevel != nextLevel)
  {
    switch (nextLevel)
    {
    case level_exit:
      gGameRunning = 0;
      break;
    default:
      Audio_PlaySoundSample("ButtonClick2.ogg", 0);
      LevelUnload();
      currentLevel = nextLevel;
      LevelLoad(nextLevel);
      break;
    }
  }
}

/*
\brief unloads the current level
*/
void LevelUnload()
{
  AEGfxSetCamPosition(0, 0);
  GameObjectFree();
  GFree();
}


/*
\brief loading for main menu
*/
void MainMenuInit()
{
  AEGfxSetCamPosition(0, 0);
  static AEGfxVertexList*	newmesh;				// Pointer to Mesh (Model)
  newmesh = GCreateMesh(16, 24, 16, 16);

  TextInit();
  printf("you're in the main menu, woo\n");
  //pretty much all example stuff
  printf("loading menu\n");
  pMesh2 = GCreateMesh(128.f, 128.f, 16, 1);
  

  // Texture 1: From file
  pTex1 = GCreateTexture("spiderwolfbrighter.png");
  pTex2 = GCreateTexture("dffont.png");

  AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  //EXAMPLE ENDS HERE

  
  textString = TextCreateString("PLACEHOLDER MAIN MENU", -360, 100);
  textString = TextCreateString("PRESS SPACE FOR LEVEL 1", -360, 0);

  //BUTTONS------------------------------------------------------
  int button_type = LEVEL_ONE_BUTTON;             /* type of button  */
  float button1x = -200;                          /* x position      */
  float button1y = -100;                          /* y position      */
  float mesh1x = 128.0f;                          /* mesh x          */
  float mesh1y = 64.0f;                           /* mesh y          */
  float button1size = 1.0f;                       /* size            */
  static AEGfxVertexList*	button_mesh;				    /* mesh ptr        */
  button_mesh = GCreateMesh(mesh1x, mesh1y, 1, 1);/* create the mesh */

  TextString *main_menu_text;
  main_menu_text = TextCreateHUDString("Level 1", button1x, button1y);
  //GCreateTexture("isotilePlaceholder1.png")
  //load button:
  Animation* anim_button1 = GCreateAnimation(1,
    NULL,
    button_mesh,
    1);

  Sprite *button1_sprite = GCreateSprite(button1x, button1y, anim_button1, 1);
  button1_sprite->offset.y = 10000;
  //PhysicsObject *button1_physics = PhysicsCreateObject(Vec2(button1x,button1y),1);

  GameObject* button = CreateButton(0, button1_sprite, NULL, button_type, button1size, mesh1x, mesh1y);

  //END BUTTONS-------------------------------------------------------------


  //EXAMPLE CODE, REMOVE OUT WHEN USING
  {
    animtest2 = GCreateAnimation(16, pTex1, pMesh2, 1);
    //sprite = GCreateSprite(0, 30, animtest, 4);
    //sprite = GCreateSprite(0, 20, animtest, 4);
    //sprite = GCreateSprite(0, -30, animtest, 4);
    sprite = GCreateSprite(100, -100, animtest2, 4);
    //sprite = GCreateHudSprite(0, 0, animtest2, 1);
  }
  //EXAMPLE CODE ENDS HERE


}

/*
\brief loading for level 1
*/
void Level1Init()
{
  GameLevelInit();
}

/*
\brief sets next level for gsm

\param _input level to set nextLevel var to
*/
void LevelSetNext(int _input)
{
  nextLevel = _input;
}

void MainMenuRun()
{
  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
  //debug
  if (AEInputCheckReleased(VK_SPACE))
  {
    
    switch (currentLevel)
    {
    case level_level1:
      nextLevel = level_exit;
      break;
    case level_mainMenu:
      nextLevel = level_level1;
      break;
    }
  }
  //
}

