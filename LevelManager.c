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
#include "CreditsScreen.h"
#include "FancyBackground.h"
#include "ColorFilter.h"
#include "ControlScreen.h"


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

// 1 show, 0 hide or not create, level 1 button will always be there
// flags to turn off or turn on tarrants buttons to be creATED
int button_flag_level1 = 0;
int button_flag_level2 = 0;
int button_flag_level3 = 0;
int button_flag_level4 = 0;
int button_flag_win = 1;
int button_flag_lose = 1;
int buttin_flag_credits = 1;
int button_flag_control = 1;
int button_flag_play = 1;
int button_flag_exit = 1;

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
    //ColorFilter_Set(1.f, 0.7f, 0.5f);
    //ColorFilter_Fluctuate(1, 0.6f, 0.3f, 0.2f);
    //ColorFilter_Set(1.f, 1.f, 1.f);
    if (!(currentLevel == level_town))
    {
      Audio_PlayMusicStream("music_sample4.ogg", 1);
      Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    }
    if (level == 4)
    {
      Audio_PlayMusicStream("EPOCH_theme_final_level.ogg", 1);
      Audio_PauseMusicStream("music_sample4.ogg");
    }
    break;
  case level_mainMenu:
    MainMenuInit();
    Background_Init(BACKGROUND_BH_SWIRLY, BACKGROUND_MD_BLOCKS);
    ColorFilter_Set(1.f, 1.f, 1.f);
    Audio_PauseMusicStream("music_sample4.ogg");
    Audio_PauseMusicStream("EPOCH_theme_final_level.ogg");
    Audio_PauseMusicStream("death_tunes.ogg");
    Audio_PauseMusicStream("EPOCH_theme_end.ogg");
    if (!(currentLevel == level_splashScreen || currentLevel == level_creditScreen || currentLevel == level_controlScreen))
      Audio_PlayMusicStream("EPOCH_main_theme.ogg", 1);
    //Audio_PauseMusicStream("music_sample3.ogg");
    break;
  case level_town:
    TownScreenInit();
    Background_Init(BACKGROUND_BH_HORIZONTAL, BACKGROUND_MD_BLOCKS);
    ColorFilter_Init();
    ColorFilter_Fluctuate(1, 0.5f, 0.2f, 0.2f);
    ColorFilter_Set(0.5f, 0.8f, 0.8f);
    break;
  case level_deathScreen:
    DeathScreenInit();
    Background_Init(BACKGROUND_BH_HORIZONTAL, BACKGROUND_MD_ENERGY);
    ColorFilter_Init();
    ColorFilter_Fluctuate(1, 0.3f, 0.0f, 0.0f);
    ColorFilter_Set(1.7f, 0.1f, 0.1f);
    level = 0;
    //Audio_PauseMusicStream("music_sample2A.ogg");
    Audio_PauseMusicStream("music_sample4.ogg");
    Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    Audio_PauseMusicStream("EPOCH_theme_final_level.ogg");
    //Audio_PauseMusicStream("EPOCH_theme_funky.ogg");
    Audio_PlayMusicStream("death_tunes.ogg", 1);
    Audio_PlaySoundSample("Creepy_Voice.ogg", 0);
    ColorFilter_Set(1.f, 1.f, 1.f);
    break;
  case level_splashScreen:
    //Audio_PlayMusicStream("music_sample3.ogg", 0);
    Audio_PlayMusicStream("EPOCH_main_theme.ogg", 1);
    SplashScreenInit();
    ColorFilter_Set(1.f, 1.f, 1.f);
    break;
  case level_winScreen:
    WinScreenInit();
    Background_Init(BACKGROUND_BH_SWIRLY, BACKGROUND_MD_ENERGY);
    level = 0;
    Audio_PlayMusicStream("EPOCH_theme_end.ogg", 0);
    Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    Audio_PauseMusicStream("music_sample4.ogg");
    Audio_PauseMusicStream("EPOCH_theme_final_level.ogg");
    //Audio_PauseMusicStream("EPOCH_theme_funky.ogg");
    ColorFilter_Set(1.f, 1.f, 1.f);
    break;
  case level_creditScreen:
    CreditsScreenInit();
    Background_Init(BACKGROUND_BH_HORIZONTAL, BACKGROUND_MD_BLOCKS);
    break;
  case level_controlScreen:
    ControlScreenInit();
    Background_Init(BACKGROUND_BH_SWIRLY, BACKGROUND_MD_ENERGY);
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
    if (AEInputCheckTriggered(VK_SPACE))
    {
      // SpawnDodgeSmokePS(4.0f, 4.0f);
    }
    if (AEInputCheckTriggered('T'))
    {
      SpawnHitSplashPS(4.0f, 4.0f, 1, 0);
    }
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
  case level_creditScreen:
    CreditsScreenRun();
    break;
  case level_controlScreen:
    ControlScreenRun();
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
      //Audio_PlaySoundSample("ButtonClick2.ogg", 0);
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

  Background_Unload();
  ColorFilter_Unload();
  Compass_Unload();
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

  //TARRANT CODE START
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
  TextStringSetTint(textString, GTint(1, 1, 1, 1));

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
  if (button_flag_level1 == 0)
  {
    DisableAndHideButton(button); //hide this button
    TextStringSetTint(main_menu_text, GTint(0.0, 0.0, 0.0, 0.0));

  }

  //end button level 1 ------------------------------

  //BUTTON TWO for level 2
  if (button_flag_level2)
  {
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

  }
  // --------------------end button level 2-----------------------
  //BUTTON three for level 3
  if (button_flag_level3)
  {
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
  }
  //---------------------------end button level 3-------------------------
  //BUTTON four for level 4
  if (button_flag_level4)
  {
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
  }
  //end button 4------------------------------------------------------------------
  // scrub button (win screen)
  if (button_flag_win)
  {
    button_type = WIN_SCREEN;               /* type of button  */
    buttonx = -200;                          /* x position      */
    buttony = -200;                          /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Scrub", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  }
  //end scrubbutton------------------------------------------------------------------
  // Lose screen
  if (button_flag_lose)
  {
    button_type = DEATH_SCREEN_BUTTON;       /* type of button  */
    buttonx = 75;                          /* x position      */
    buttony = -200;                          /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Die", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  }
  //end lose screen ------------------------------------------------------------------
  //credits button----------------------------------------------------------------
  if (buttin_flag_credits)
  {
    button_type = CREDIT_SCREEN;          /* type of button  */
    buttonx = 200;                          /* x position      */
    buttony = 25;                            /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Credits", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  }


  //END credit BUTTONS-------------------------------------------------------------
  //play level 1 button------------------------------------------------------
  if (button_flag_play)
  {
    button_type = LEVEL_ONE_BUTTON;          /* type of button  */
    buttonx = 200;                          /* x position      */
    buttony = 100;                            /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Play", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  }
 //END play BUTTON-------------------------------------------------------------
  //start control button--------------------------------------------------------
  if (button_flag_control)
  {
    button_type = CONTROL_SCREEN;          /* type of button  */
    buttonx = 200;                          /* x position      */
    buttony = -50;                            /* y position      */
    meshx = 256.0f;                          /* mesh x          */
    meshy = 64.0f;                           /* mesh y          */
    buttonsize = 1.0f;                       /* size            */
    button_mesh = GCreateMesh(meshx, meshy, 1, 1);/* create the mesh */
    text_offset = 90.0f;
    main_menu_text = TextCreateHUDString("Controls", buttonx - text_offset, buttony);
    TextStringSetTint(main_menu_text, GTint(1, 1, 1, 1));
    button_texture = GCreateTexture("animations/buttons/button_texture.png");
    anim_button = GCreateAnimation(1,
      button_texture,
      button_mesh,
      1);

    button_sprite = GCreateSprite(buttonx, buttony, anim_button, 1);
    button = CreateButton(0, button_sprite, NULL, button_type, buttonsize, meshx, meshy);
  }
  //end control button ----------------------------------------------------------

  //END BUTTONS-------------------------------------------------------------
  //END TARRANT CODE

  //EXAMPLE CODE, REMOVE OUT WHEN USING
  {
    //animtest2 = GCreateAnimation(16, pTex1, pMesh2, 1);
    //sprite = GCreateSprite(0, 30, animtest, 4);
    //sprite = GCreateSprite(0, 20, animtest, 4);
    //sprite = GCreateSprite(0, -30, animtest, 4);
    //sprite = GCreateSprite(100, -100, animtest2, 4);
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

  Background_Update();

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
      //TARRANT CODE------------
      // map any cheat keys here
      //TARRANT CODE END-------
    }
    //Audio_PlayMusicStream("music_sample4.ogg", 1);
    //Audio_PauseMusicStream("music_sample3.ogg");
    //Audio_PauseMusicStream("EPOCH_main_theme.ogg");
    //Audio_PlayMusicStream("EPOCH_theme_funky.ogg", 1);
  }
  //
}

