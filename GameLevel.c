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
#include "PlayerEntity.h"
#include "Hazard.h"
#include "Button.h"
#include "ImportData.h"

//dumb hard coding for alpha:
int level; 
//


//extern int nextLevel;/**< Level to switch to (if not equal to current level) (uses enum)*/
static GameObject* player; /**< pointer to player object*/

/*!
\brief Initialize game level
*/
void GameLevelInit(void)
{
  printf("game level init\n");
  PhysicsInit();
  IsoInit(64, 64);
  

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
  playerEntity->maxHealth = 30;
  EntityInit(&playerEntity);
  player = GameObjectCreate(PhysicsCreateObject(Vec2(2, 2), 1), GCreateSprite(0, 40, anim, 1), playerEntity, entity_player);
  player->simulate = &PlayerSimulate;
  player->entity->onEntityKilled = &OnPlayerKilled;
  
  PlayerInit();

  //create door object:
  GameObject* door = GameObjectCreate(PhysicsCreateObject(Vec2(6, 4), 1), GCreateSprite(0, 40, anim2, 1), 0, entity_door);
  door->physics->onCollision = &DoorDefaultOnCollision;
  door->simulate = NULL;

  /**********************
  HAZARDS
  ***************************/
  GameObject* hazard = GameObjectCreate(PhysicsCreateObject(Vec2(10, 5), 1), GCreateSprite(0, 40, anim2, 1), 0, entity_hazard);
  Vector2D pushForce = { 5.f, 8.f };
  unsigned int hType = 0;
  hType |= HAZARD_DAMAGE;
  hType |= HAZARD_DAMAGE_CONSTANT;
  hType |= HAZARD_WARP;
  ComponentAdd_Hazard(hazard, 4, 3, 1, pushForce, hType);
  hazard->physics->onCollision = &Hazard_OnCollision;
  hazard->simulate = NULL;
 
  //PhysicsRemoveObject(&a);

<<<<<<< HEAD
  ImportEnemyData(7, 8, "Level1EnemyMelee1.txt", player);
  ImportEnemyData(10, 10, "Level1EnemyMelee1.txt", player);

  ImportEnemyData(10, 8, "Level1EnemyRanged2.txt", player);
=======
  /*************************
  BUTTONS
  ************************/
  

  //GameObject* hazard = GameObjectCreate(, , 0, entity_hazard);
  //Magical function that populates the world
  ImportEnemyData("EnemiesLevel1.txt", player);
>>>>>>> refs/remotes/origin/master
}

//THE GAME LEVEL RUN FUNCTION WAS HERE AND MOVED TO LEVEL MANAGER BY TARRANT AND NOW IT IS BACK 
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
  GameObjectsPostStep();
  //
  AEGfxSetCamPosition(player->sprite->x, player->sprite->y);
  
}

/*!
\brief Internal- helper function called by GameLevelRun
*/
void InputHandle(void)
{
  
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
  LevelSetNext(level_deathScreen);
}