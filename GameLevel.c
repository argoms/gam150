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
#include "GameObject.h"
#include "LevelManager.h"
#include "Door.h"


extern int nextLevel;/**< Level to switch to (if not equal to current level) (uses enum)*/
static GameObject* player; /**< pointer to player object*/

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


  //load tile placeholder sprite:
  Animation* anim = GCreateAnimation(1, 
                  GCreateTexture("isotilePlaceholder1.png"),
                  GCreateMesh(128.f, 64.f, 1, 1),
                  1);


  //set up player object:
  player = GameObjectCreate(PhysicsCreateObject(Vec2(2, 2), 1), GCreateSprite(0, 40, anim, 1), 0, entity_player);
  player->simulate = &InputHandle;

  //create door object:
  GameObject* door = GameObjectCreate(PhysicsCreateObject(Vec2(5, 4), 1), GCreateSprite(0, 40, anim, 1), 0, entity_door);
  door->simulate = &OnTouchDoor;
  door->physics->onCollision = &DoorDefaultOnCollision;

  //PhysicsRemoveObject(&a);
}

/*!
\brief Game loop- run every frame

  Currently just updates player position based on input.
*/
void GameLevelRun()
{

  //Vector2D a = Vec2(2, 2);
  
  //printf("a %f, %f \n", IsoWorldToScreen(&playerObject->position).x, IsoWorldToScreen(&playerObject->position).y);

  //InputHandle();
  GameObjectSimulate();
  PhysicsSimulate();
  //
  AEGfxSetCamPosition(player->sprite->x, player->sprite->y);
  
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
    Vector2DScale(&input, &input, 10);
  }
  player->physics->velocity = IsoScreenToWorld(&input);
  GSortSprite(player->sprite, player->physics->velocity.y);
}

/*
\brief placeholder function for when you hit the door
*/
void OnTouchDoor()
{
  //nextLevel = level_mainMenu;
}