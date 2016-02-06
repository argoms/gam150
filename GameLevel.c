/*!
\file   GameLevel.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
  Contains functions for 'in-dungeon' game levels.
*/
#include "GameLevel.h"
#include "Physics.h"
#include "Graphics.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Isometric.h"


static PhysicsObject* playerObject; /**< pointer to player physics object*/
static Sprite* playerSprite; /**<pointer to player sprite object*/

/*!
\brief Initialize game level
*/
void GameLevelInit()
{
  printf("game level init\n");
  PhysicsInit();
  IsoInit(16, 16);
  

  AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  //EXAMPLE ENDS HERE



  Animation* anim = GCreateAnimation(1, 
                  GCreateTexture("isotileGreen.png"),
                  GCreateMesh(128.f, 64.f, 1, 1),
                  1);

  playerSprite = GCreateSprite(0, 40, anim, 1);
  playerObject = PhysicsCreateObject(Vec2(2, 2), Vec2(1, 1));
}

/*!
\brief Game loop- run every frame

  Currently just updates player position based on input.
*/
void GameLevelRun()
{

  //Vector2D a = Vec2(2, 2);
  playerSprite->x = IsoWorldToScreen(&playerObject->position).x;
  playerSprite->y = IsoWorldToScreen(&playerObject->position).y;
  //printf("a %f, %f \n", IsoWorldToScreen(&playerObject->position).x, IsoWorldToScreen(&playerObject->position).y);

  InputHandle();
  PhysicsSimulate();
  AEGfxSetCamPosition(playerSprite->x, playerSprite->y);

  
}

/*!
\brief Internal- helper function called by GameLevelRun
*/
void InputHandle()
{

  //if (AEInputCheckTriggered(VK_SPACE))
  Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
                        AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53));
  if (input.x != 0 || input.y != 0)
  {
    Vector2DNormalize(&input, &input);
    Vector2DScale(&input, &input, 5);
  }
  playerObject->velocity = IsoScreenToWorld(&input);
  GSortSprite(playerSprite, playerObject->velocity.y);
  
  
}