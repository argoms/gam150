/*!
\file   GameObject.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions for in-world game objects.
*/

#include "GameObject.h"
#include "Graphics.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Isometric.h"
#include "Entity.h"
#include "Enemy.h"
#include <stdlib.h>
#include <stdio.h>

//#define MAX_GAME_OBJECTS 10 /**< maximum number of game objects that can be simulated*/
static GameObjectList gameObjectList; /**< contains data about currently active objects*/
static int objectDestroyedFlag; /**< flag triggered when a game object is destroyed, for internal usage*/

//static function prototypes:
static void GameObjectRemove(GameObject** _input); 

void GameObjectInitialize()
{
  gameObjectList.first = NULL;
  gameObjectList.last = NULL;
  objectDestroyedFlag = 0;
}
/*!
\brief creates a gameobject given relevant components

\param _physics physics component to add
\param _sprite sprite component to add
\param _entity entity component to add
\param _type type of the new object
\return Returns a pointer to the new gameobject.
*/
GameObject* GameObjectCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type)
{
  GameObject* newGameObject = (GameObject*) malloc(sizeof(GameObject));
  newGameObject->sprite = _sprite;
  
  newGameObject->physics = _physics;
  newGameObject->physics->owner = newGameObject;

  newGameObject->entity = _entity;

  if (_entity)
  {
    _entity->owner = newGameObject;
  }
  
  newGameObject->enemyAI = NULL;

  newGameObject->type = _type;
  newGameObject->destroyFlag = 0;
  
  newGameObject->syncSpritePhysics = 1;
  newGameObject->prev = NULL;
  newGameObject->next = NULL; 

  newGameObject->miscData = NULL; 
  newGameObject->simulate = NULL;
  //printf("%f", newGameObject->entity);

  //update list:
  if (!gameObjectList.first)
  {
    gameObjectList.first = newGameObject;
    gameObjectList.last = newGameObject;
  }
  else {
    newGameObject->prev = gameObjectList.last;
    gameObjectList.last->next = newGameObject;
    gameObjectList.last = newGameObject;
  }
  
  return newGameObject;
}

/*!
\brief Tells engine to free gameobject from memory, including removing component objects (eg. sprite) from their relevant parts.
Actually sets a flag, which is then managed to properly destroy later.
\param _input pointer to address of gameobject to destroy (&potato*, not potato*)
*/
void GameObjectDestroy(GameObject** _input)
{
  /*
  printf("%i ASADQWE", (*_input)->type);
  if ((*_input)->type == 2)
  {
    printf("EK");
  }*/
  (*_input)->destroyFlag = 1;
  
}

/*!
\brief actually frees gameobject from memory, done after game loop steps
\param _target pointer to address of gameobject to destroy (&potato*, not potato*)
*/
static void GameObjectRemove(GameObject** _input)
{
  if ((*_input)->sprite)
  {
    GRemoveSprite(&((*_input)->sprite));
  }
  if ((*_input)->physics)
  {
    PhysicsRemoveObject(&((*_input)->physics));
  }
  if ((*_input)->entity)
  {
    free((*_input)->entity);
  }
  if ((*_input)->enemyAI)
  {
    free((*_input)->enemyAI);
  }
  if ((*_input)->miscData && (*_input)->type == entity_enemy)
  {
    free((*_input)->miscData);
  }

  if ((*_input)->prev)
  {

    if ((*_input)->next)
    {
      (*_input)->next->prev = (*_input)->prev;
      (*_input)->prev->next = (*_input)->next;
    }
    else
    {
      (*_input)->prev->next = NULL;
      gameObjectList.last = (*_input)->prev;
    }

  }
  else
  {

    if ((*_input)->next)
    {
      (*_input)->next->prev = NULL;
      gameObjectList.first = (*_input)->next;
    }
    else
    {
      gameObjectList.first = NULL;
      gameObjectList.last = NULL;
    }
  }
  free(*_input);
  *_input = NULL;
}

/*!
\brief run at the end of a game loop to remove objects (prevents stuff from breaking by removing mid-simulation of a step)
*/
void GameObjectsPostStep()
{
  if (gameObjectList.first)
  {
    GameObject* instance = gameObjectList.first;
    while (instance)
    {
      GameObject* instanceNext = instance->next;
      if (instance->destroyFlag)
      {
        //printf("\nQQQ \nQQQ\n %i poststep\n", instance->next);
        GameObjectRemove(&instance);
      }
      instance = instanceNext;

    }
  }
}
/*!
\brief simulates gameobject behavior (calls the "simulate" function on every gameobject)
*/
void GameObjectSimulate()
{
  //printf("A");
  if (gameObjectList.first)
  {
    GameObject* instance = gameObjectList.first;
    GameObject* instanceNext;
    while (instance)
    {

      instanceNext = instance->next;
      if (instance->simulate)
      {
        objectDestroyedFlag = 0;
        instance->simulate(instance); //remember to set this to time later on
        if (objectDestroyedFlag)
        {
          instance = instanceNext;
          continue;
        }
      }

      if (instance->physics && instance->syncSpritePhysics)
      {
        
        instance->sprite->x = IsoWorldToScreen(&instance->physics->position).x;
        instance->sprite->y = IsoWorldToScreen(&instance->physics->position).y;

      }
      instance = instanceNext;

    }
  }
}

/*!
\brief frees allocated memory for ALL gameobjects
mem leak atm can't free sprite member?
*/
void GameObjectFree()
{
  GameObject* temp = gameObjectList.first; //index
  GameObject* tempPrevious; //index for previous value

  if (temp)
  {
    while (temp->next)
    {
      tempPrevious = temp;
      //printf("||| %p, %p ||", tempPrevious, gameObjectList.first);
        
      temp = temp->next;
      //GRemoveSprite(tempPrevious->sprite);
      free(tempPrevious);
      //GameObjectDestroy(tempPrevious);

    }
    //GameObjectDestroy(temp);
    //printf("||| %p ||", temp);
    free(temp);
  }

  gameObjectList.first = NULL;
  gameObjectList.last = NULL;
}