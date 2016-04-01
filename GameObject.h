/*!
Project (working title): Epoch
\file   GameObject.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Declarations for in-world game objects. Objects contain components such as:
-physics (movement/collision)
-graphics (sprite)
-entity (health)
-misc

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "Entity.h"
#include "Graphics.h"
#include "Physics.h"

#define AISTACKSIZE 3

typedef struct GameObject GameObject;

typedef struct GameObjectList GameObjectList;
typedef struct EnemyAI EnemyAI;
typedef struct EnemyContainer EnemyContainer;

enum GameObjectNames {
  entity_player, /**< player*/
  entity_door, /**< inter-level door*/
  entity_enemy, /**< generic enemy*/
  entity_friendlyProjectile, /**<used for player projectiles/attack tracers etc.*/
  entity_enemyProjectile,
  entity_button, /**< generic button */
  entity_hazard,   /* environmental hazards */
  //entity_health_bar /**< health bar */
  entity_particle, /**< particle that doesn't affect other game objects*/
  entity_damageText, /**< visual indicator for damage dealt*/
  entity_gate, /**< opens after clearing a room*/
  entity_room /**< used for entire rooms*/
};

/**************************************************************************************************
    
    ENEMY SPECIFIC STUFF STARTS HERE
    Mostly Noah's stuff
      
**************************************************************************************************/

// SET THE NUMBERS IN COMMENTS FOR THE ENEMIES TEXT FILE
enum EnemyType {
  ENEMY_TYPE_MELEE,         // 1
  ENEMY_TYPE_MELEE_BIG,     // 2
  ENEMY_TYPE_MELEE_CHARGE,  // 3
  ENEMY_TYPE_RANGED,        // 4
  ENEMY_TYPE_RANGED_ARC,    // 5
  ENEMY_TYPE_RANGED_HOMING, // 6

  /* Special enemies */
  ENEMY_TYPE_HEALER, // 7
  ENEMY_TYPE_SLIME,  // 8
  ENEMY_TYPE_SHIELD, // 9
};

struct EnemyAI
{
  int currentEnemyState;       /* Current state */
  int newEnemyState;    /* Internal usage, no touch */
  void(*EnemyStateStart)();
  void(*EnemyStateUpdate)();
  void(*EnemyStateExit)();
};

/*
\brief Container for enemy specific stuff
*/
struct EnemyContainer
{
  unsigned int enemyAnimationState;

  int enemyType;

  int health;

  float chaseSpeed;
  float patrolSpeed;

  float detectRange;
  float knockback;
  
  float animationCooldown; /*Placeholder until better solution*/
  float attackCooldown; /**< timer before player can attack again*/
  float attackCooldownLength; /**< defined minimum time between attacks (attackCooldown is the timer)*/
  float attackWindup;  /* Wind up timer */
  float attackWindupLength; /* Defined minimum time between windups (attackWindup is the actual timer) */
  float attackRange;
  int attackDamage;
  float attackKnockbackForce;

  float projectileSpeed;
};

/**************************************************************************************************

ENEMY SPECIFIC STUFF ENDS HERE

**************************************************************************************************/

/*!
\struct GameObject
\brief Game objects (mobs, pressure plates etc.)
*/
struct GameObject
{
  Sprite* sprite; /**< graphical component*/
  PhysicsObject* physics; /**< physics component*/
  Entity* entity; /**< entity component*/
  void(*simulate)(); /**< function to run every frame*/
  void(*initialize)(); /* Initialize values*/
  int syncSpritePhysics; /**< whether or not to sync the graphical component with the world position of the gameobject (leave it at 1 unless you're doing something weird)*/
  int type; /**< type of entity that the gameobject is (refer to enum list)*/

  EnemyAI* enemyAI;
  GameObject* target;

  int destroyFlag; /**< internal, used for removing game objects*/
  GameObject* next; /**< pointer to previous object in list*/
  GameObject* prev; /**< pointer to next object in list*/
  void* miscData;   /**< void pointer to whatever we want*/
  float projectileLifeTime;

  GameObject* parent;
};



/*!
\struct GameObjectList
\brief contains simple info about the active list of GameObjects
*/
struct GameObjectList
{
  GameObject* first; /**< first object in GameObject linked list*/
  GameObject* last; /**< last object in GameObject linked list*/
};

void GameObjectSimulate();
void GameObjectDestroy(GameObject** _input);
GameObject* GameObjectCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type);
void GameObjectInitialize();
void GameObjectFree();
void GameObjectsPostStep();
void DisplayHealth(GameObject *obj);