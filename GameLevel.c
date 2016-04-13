/*!
Project (working title): Epoch
\file   GameLevel.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
  Contains functions for 'in-dungeon' game levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
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
#include "EnemyAnimations.h"

//#include "ParticleSystems(Redo).h"
#include "MyRandom.h"
#include "Compass.h"
#include "PlayerDeathTimer.h"
#include "EnemyAnimations.h"

#define MAP_WIDTH 64
#define MAP_HEIGHT 64

//dumb hard coding for alpha:
// for game play levels not win/lose screens 
int level = 1; 
//

//test particle effect istances
//PS_Instance *pPS_C;
//PS_Instance *pPS_B;

//extern int nextLevel;/**< Level to switch to (if not equal to current level) (uses enum)*/
static GameObject* player; /**< pointer to player object*/

/*!
\brief Initialize game level
*/
void GameLevelInit(void)
{
  EnemyImportInfo(ENEMY_TYPE_MELEE, "EnemyMelee.txt");
  EnemyImportInfo(ENEMY_TYPE_MELEE_BIG, "EnemyMeleeBig.txt");
  EnemyImportInfo(ENEMY_TYPE_MELEE_CHARGE, "EnemyMeleeCharger.txt");
  EnemyAnimationInitialize();
  //EnemyImportInfo(ENEMY_TYPE_RANGED, "EnemyRanged.txt");
  Entity* playerEntity;

  /*

	pPS_C = Create_PS_Continuous(2.0f, 5.0f, -1);

	pPS_C->PS_Continuous->StartPosX = 2;
	pPS_C->PS_Continuous->StartPosY = 2;

	pPS_B = Create_PS_Burst(2.0f, 20);

	//pPS_B->PS_Burst->StartPosX = 2;
	//pPS_B->PS_Burst->StartPosY = 2;

	for (i = 0; i < 4; i++)
	{
		pDodgeSmoke[i] = Create_PS_Burst(2.0f, 10);
		pDodgeSmoke[i]->PS_Burst->vpParticle_Create = Particle_Create_DodgeSmoke;
	}

	for (i = 0; i < 4; i++)
	{
		pHitSplash[i] = Create_PS_Burst(0.5f, 10);
		pHitSplash[i]->PS_Burst->vpParticle_Create = Particle_Create_HitSplash;
	}

	
	for (i = 0; i < 2; i++)
	{
		pFireHazard[i] = Create_PS_Continuous(1.0f, 10, -1);
		pFireHazard[i]->PS_Continuous->vpParticle_Create = Particle_Create_FireHazard;
	}
	
	
	pFireHazard[0]->PS_Continuous->StartPosX = 5;
	pFireHazard[0]->PS_Continuous->StartPosY = 3;
	Start_PS(pFireHazard[0]);

	pFireHazard[1]->PS_Continuous->StartPosX = 3;
	pFireHazard[1]->PS_Continuous->StartPosY = 5;
	Start_PS(pFireHazard[1]);
	*/

  printf("game level init\n");
  PhysicsInit();
  
  

  AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  //EXAMPLE ENDS HERE


  //load tile placeholder sprite:
  Animation* anim = GCreateAnimation(1, 
                  GCreateTexture("animations/isotilePlaceholder1.png"),
                  GCreateMesh(128.f, 64.f, 1, 1),
                  1);

  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("animations/isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  Animation* anim3 = GCreateAnimation(1,
    GCreateTexture("animations/isoTileRed.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  
  


  //set up player object:

  

  playerEntity = malloc(sizeof(Entity));
  playerEntity->maxHealth = 60;
  EntityInit(&playerEntity);
  player = GameObjectCreate(PhysicsCreateObject(Vec2(4, 4), 1), GCreateSprite(0, 40, anim, 1), playerEntity, entity_player);
  player->simulate = &PlayerSimulate;
  player->entity->onEntityKilled = &OnPlayerKilled;
  
  PlayerInit();

  //create compass
  Compass_Load();
  Compass_Init();
  

  //map generation actually happens at the end because it needs the player and shit.
  IsoInit(MAP_WIDTH, MAP_HEIGHT);

  /*
  int x, y;
  do
  {
    x = RandIntRange(1, 8);
    y = RandIntRange(1, 8);
  } while (IsoTileGet(x, y));

  //create door object:
  do
  {
    x = RandIntRange(15, 20);
    y = RandIntRange(15, 20);
  } while (IsoTileGet(x, y) || (player->physics->position.x == x && player->physics->position.y == y));*/

  

  /**********************
  HAZARDS
  ***************************/
  /*
  do
  {
    x = RandIntRange(9, 14);
    y = RandIntRange(9, 14);
  } while (IsoTileGet(x, y) || (player->physics->position.x == x && player->physics->position.y == y)
    || (door->physics->position.x == x && door->physics->position.y == y));*/
  /*
  GameObject* hazard = GameObjectCreate(PhysicsCreateObject(Vec2(x, y), 1), GCreateSprite(0, 40, anim3, 1), 0, entity_hazard);
  Vector2D pushForce = { 5.f, 8.f };
  unsigned int hType = 0;
  hType |= HAZARD_DAMAGE;
  hType |= HAZARD_DAMAGE_CONSTANT;
  hType |= HAZARD_WARP;
  ComponentAdd_Hazard(hazard, 4, 3, 1, pushForce, hType);
  hazard->physics->onCollision = &Hazard_OnCollision;
  hazard->simulate = NULL;*/
 
  //PhysicsRemoveObject(&a);

  /*
  int numEnemies;
  int enemiesToCreate = 6;

  for (numEnemies = 0; numEnemies < enemiesToCreate; numEnemies += 2)
  {
    int x, y;
    do
    {
      x = RandIntRange(6, MAP_WIDTH / 4);
      y = RandIntRange(6, MAP_HEIGHT / 4);
    } while (IsoTileGet(x, y) || (player->physics->position.x == x && player->physics->position.y == y)
      || (door->physics->position.x == x && door->physics->position.y == y));
//      || (hazard->physics->position.x == x && hazard->physics->position.y == y));
    
    ImportEnemyData(x, y, "Level1EnemyMelee1.txt", player);

    do
    {
      x = RandIntRange(6, MAP_WIDTH / 4);
      y = RandIntRange(6, MAP_HEIGHT / 4);
    } while (IsoTileGet(x, y) || (player->physics->position.x == x && player->physics->position.y == y)
      || (door->physics->position.x == x && door->physics->position.y == y));
//      || (hazard->physics->position.x == x && hazard->physics->position.y == y));

    ImportEnemyData(x, y, "Level1EnemyRanged2.txt", player);
  }

  //ImportEnemyData(7, 8, "Level1EnemyMelee1.txt", player);
  //ImportEnemyData(10, 10, "Level1EnemyMelee1.txt", player);

  //ImportEnemyData(10, 8, "Level1EnemyRanged2.txt", player);
  */
  /*************************
  BUTTONS
  ************************/
  

  //GameObject* hazard = GameObjectCreate(, , 0, entity_hazard);
  //Magical function that populates the world

  
}

//THE GAME LEVEL RUN FUNCTION WAS HERE AND MOVED TO LEVEL MANAGER BY TARRANT AND NOW IT IS BACK 
/*!
\brief Game loop- run every frame

  Currently just updates player position based on input.
*/

void GameLevelRun(void)
{
  /*
	if (AEInputCheckTriggered('Y'))
	{
		Start_PS(pPS_C);
	}
	if (AEInputCheckTriggered('H'))
	{
		Stop_PS(pPS_C);
	}
  */

  //Vector2D a = Vec2(2, 2);
  
  //printf("a %f, %f \n", IsoWorldToScreen(&playerObject->position).x, IsoWorldToScreen(&playerObject->position).y);
  //InputHandle();
  GameObjectSimulate();
  PhysicsSimulate();
  GameObjectsPostStep();
  //
  
  if (player->sprite)
  {
    Compass_Update(&player->physics->position, &(DoorGetDoor()->physics->position));
  }

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

