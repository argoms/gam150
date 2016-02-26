/*!
\file   GameObject.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions for game objects.
*/
#pragma once
#include "Entity.h"
#include "Graphics.h"
#include "Physics.h"
#include "Hazard.h"

typedef struct GameObject GameObject;

typedef struct GameObjectList GameObjectList;
typedef struct EnemyAI EnemyAI;

typedef struct Component_HAZARD Component_HAZARD;

enum GameObjectNames {
  entity_player, /**< player*/
  entity_door, /**< inter-level door*/
  entity_enemy, /**< generic enemy*/
  entity_friendlyProjectile, /**<used for player projectiles/attack tracers etc.*/
  entity_button, /**< generic button */
  entity_hazard   /* environmental hazards */
};


struct EnemyAI
{
  int enemyState;
};

/*!
\struct GameObject
\brief Game objects (mobs, pressure plates etc.)
*/
struct GameObject
{
  Sprite* sprite; /**< graphical component*/
  PhysicsObject* physics; /**< physics component*/
  Entity* entity; /**< entity component*/
  Component_HAZARD *hazardComponent;  /* Hazard component */

  void(*simulate)(); /**< function to run every frame*/
  int syncSpritePhysics; /**< whether or not to sync the graphical component with the world position of the gameobject (leave it at 1 unless you're doing something weird)*/
  int type; /**< type of entity that the gameobject is (refer to enum list)*/
  
  EnemyAI* enemyAI;
  GameObject* target;

  int destroyFlag; /**< internal, used for removing game objects*/
  GameObject* next; /**< pointer to previous object in list*/
  GameObject* prev; /**< pointer to next object in list*/
  void* miscData;   /**< void pointer to whatever we want*/
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