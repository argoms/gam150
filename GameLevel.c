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
#include "Enemy.h"


//extern int nextLevel;/**< Level to switch to (if not equal to current level) (uses enum)*/
static GameObject* player; /**< pointer to player object*/

/*!
\brief Initialize game level
*/
void GameLevelInit(void)
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

  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);


  //set up player object:
  Entity* playerEntity = malloc(sizeof(Entity));
  playerEntity->maxHealth = 100;
  EntityInit(&playerEntity);
  player = GameObjectCreate(PhysicsCreateObject(Vec2(2, 2), 1), GCreateSprite(0, 40, anim, 1), playerEntity, entity_player);
  player->simulate = &InputHandle;
  player->entity->onEntityKilled = &OnPlayerKilled;

  //create door object:
  GameObject* door = GameObjectCreate(PhysicsCreateObject(Vec2(5, 4), 1), GCreateSprite(0, 40, anim2, 1), 0, entity_door);
  door->physics->onCollision = &DoorDefaultOnCollision;
  door->simulate = NULL;

  //create enemy object:
  Entity* enemyEntity = malloc(sizeof(Entity));
  enemyEntity->maxHealth = 100;
  EntityInit(&enemyEntity);
  GameObject* enemy = GameObjectCreate(PhysicsCreateObject(Vec2(4, 5), 1), GCreateSprite(0, 40, anim, 1), 0, entity_enemy);
  enemy->physics->onCollision = EnemyOnCollision; //ENEMY COLLISON BEHAVIOR GO HERE
  enemy->simulate = &EnemySimulate; //ENEMY CALLS THIS EVERY FRAMEA


 
  //PhysicsRemoveObject(&a);
}

/*!
\brief Game loop- run every frame

  Currently just updates player position based on input.
*/
void GameLevelRun(void)
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
void InputHandle(void)
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
  //printf("PLAYER HP: %i\n", player->entity->health);
}

/*
\brief returns player object
*/
GameObject* GetPlayerObject(void)
{
  return player;
}

/*!
\brief called when player dies
*/
void OnPlayerKilled(void)
{
  printf("\n***\n***\nYOU DIED SO NOW YOU'RE IN MAIN MENU WOOO\n***\n***\n");
  LevelSetNext(level_mainMenu);
}