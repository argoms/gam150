#include "Graphics.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Text.h"
#include "GameLevel.h"

//EXAMPLE VARIABLES, NOT STRICTLY NEEDED
static AEGfxVertexList*	pMesh2;				// Pointer to Mesh (Model)
static AEGfxTexture *pTex1;					// Pointer to Texture (Image)
static AEGfxTexture *pTex2;					// Pointer to Texture (Image)

static Sprite* sprite; //only one for debug purposes
static Animation* animtest;
static Animation* animtest2;

static float textY;
static TextString* textString;
//EXAMPLE CODE ENDS HERE


static int currentLevel;
static int nextLevel;

extern int gGameRunning; //assume that for now

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
  }

}

/*
\brief level game loop
*/
void LevelRun()
{
  //debug
  if (AEInputCheckTriggered(VK_SPACE))
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

  switch (currentLevel)
  {
  case level_level1:
    GameLevelRun();
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
  GFree();
}

/*
\brief loading for main menu
*/
void MainMenuInit()
{
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

  
  textString = TextCreateString("PLACEHOLDER MAIN MENU PRESS SPACE TO ENTER GAME", -360, 50);
  


  //EXAMPLE CODE, REMOVE OUT WHEN USING
  {
    animtest2 = GCreateAnimation(16, pTex1, pMesh2, 1);
    //sprite = GCreateSprite(0, 30, animtest, 4);
    //sprite = GCreateSprite(0, 20, animtest, 4);
    //sprite = GCreateSprite(0, -30, animtest, 4);
    sprite = GCreateSprite(100, 40, animtest2, 4);
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