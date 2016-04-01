/*!
Project (working title): Epoch
\file   LevelManager.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic level/gamestate manager implementation.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
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
#include "ParticleSystems(Redo).h"
#include "Compass.h"


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

extern int level;

/*
\brief loads given level
\param _level level to be loaded
*/
void LevelLoad(int _level)
{
	LoadAll_PS();

  GInitialize();

  switch (_level)
  {
  case level_level1:
    Level1Init();
    if (!(currentLevel == level_town))
    {
      Audio_PlayMusicStream("music_sample4.ogg", 1);
      Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    }
    break;
  case level_mainMenu:
    MainMenuInit();
    Audio_PlayMusicStream("EPOCH_main_theme.ogg", 1);
    Audio_PauseMusicStream("music_sample3.ogg");
    break;
  case level_town:
    TownScreenInit();
    break;
  case level_deathScreen:
    DeathScreenInit();
    level = 0;
    //Audio_PauseMusicStream("music_sample2A.ogg");
    Audio_PauseMusicStream("music_sample4.ogg");
    //Audio_PauseMusicStream("EPOCH_theme_funky.ogg");
    break;
  case level_splashScreen:
    Audio_PlayMusicStream("music_sample3.ogg", 0);
    SplashScreenInit();
    break;
  case level_winScreen:
    WinScreenInit();
    level = 0;
    Audio_PauseMusicStream("music_sample4.ogg");
    //Audio_PauseMusicStream("EPOCH_theme_funky.ogg");
    break;
  }

  if (_level != level_level1)
  {
    //Matt's compass code
    Compass_Free();
    Compass_Unload();

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
      Compass_Unload();
      break;
    default:
      Audio_PlaySoundSample("ButtonClick2.ogg", 0);
      LevelUnload();
      LevelLoad(nextLevel);
      currentLevel = nextLevel;
      break;
    }
  }

  if (frameTime > 0.5)
  {
	  frameTime = 0.016;
  }
  UpdateAllPS_Inst((float)frameTime);
}

/*
\brief unloads the current level
*/
void LevelUnload()
{
  AEGfxSetCamPosition(0, 0);
  GameObjectFree();
  UnloadAll_PS();
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

  
  //textString = TextCreateString("PLACEHOLDER MAIN MENU", -360, 100);
  //textString = TextCreateString("PRESS SPACE FOR LEVEL 1", -360, 0);

  //-----------------Tarant text------------------------------

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

  textString = TextCreateString("EPOCH", string_xpos, string_ypos); // title

  //BUTTONS------------------------------------------------------
  // BUTTON LEVEL 1

  int button_type = LEVEL_ONE_BUTTON;             /* type of button  */
  float button1x = -200;                          /* x position      */
  float button1y = -100;                          /* y position      */
  float mesh1x = 256.0f;                          /* mesh x          */
  float mesh1y = 64.0f;                           /* mesh y          */
  float button1size = 1.0f;                       /* size            */
  static AEGfxVertexList*	button_mesh;				    /* mesh ptr        */
  button_mesh = GCreateMesh(mesh1x, mesh1y, 1, 1);/* create the mesh */

  float text_offset = 90.0f;
  TextString *main_menu_text;
  main_menu_text = TextCreateHUDString("Level 1", button1x - text_offset, button1y);
  AEGfxTexture* button_texture = GCreateTexture("isocircleGreen.png");
  Animation* anim_button1 = GCreateAnimation(1,
    button_texture,   //was null
    button_mesh,
    1);

  Sprite *button1_sprite = GCreateSprite(button1x, button1y, anim_button1, 1);
  //button1_sprite->offset.y = 10000;
  //PhysicsObject *button1_physics = PhysicsCreateObject(Vec2(button1x,button1y),1);

  GameObject* button = CreateButton(0, button1_sprite, NULL, button_type, button1size, mesh1x, mesh1y);
  //end button level 1 ------------------------------
  


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
    //Audio_PlayMusicStream("music_sample4.ogg", 1);
    //Audio_PauseMusicStream("music_sample3.ogg");
    //Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    //Audio_PlayMusicStream("EPOCH_theme_funky.ogg", 1);
  }
  //
}

