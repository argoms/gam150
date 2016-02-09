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

#define MAX_GAME_OBJECTS 10 /**< maximum number of game objects that can be simulated*/
static GameObjectList gameObjectList;
void GameObjectInitialize()
{
  gameObjectList.first = NULL;
  gameObjectList.last = NULL;
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
  newGameObject->type = _type;
  
  newGameObject->syncSpritePhysics = 1;
  newGameObject->prev = NULL;
  newGameObject->next = NULL;
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
}

/*!
\brief Frees gameobject from memory, including removing component objects (eg. sprite) from their relevant parts.
\param _target pointer to address of gameobject to destroy (&potato*, not potato*)
*/
void GameObjectDestroy(GameObject** _input)
{
  if ((*_input)->sprite)
  {
    GRemoveSprite(&(*_input)->sprite);
  }
  if ((*_input)->physics)
  {
    PhysicsRemoveObject(&((*_input)->physics));
  }
  if ((*_input)->entity)
  {
    free((*_input)->entity);
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
\brief simulates gameobject behavior (calls the "simulate" function on every gameobject)
*/
void GameObjectSimulate()
{
  //printf("A");
  if (gameObjectList.first)
  {
    GameObject* instance = gameObjectList.first;
    while (instance)
    {

      if (instance->simulate)
      {
        instance->simulate(); //remember to set this to time later on
      }
      if (instance->syncSpritePhysics)
      {
        
        instance->sprite->x = IsoWorldToScreen(&instance->physics->position).x;
        instance->sprite->y = IsoWorldToScreen(&instance->physics->position).y;
        
      }
      instance = instance->next;

    }
  }
}

/*!
\brief frees allocated memory for gameobjects

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
      free(tempPrevious);

    }
    //printf("||| %p ||", temp);
    free(temp);
  }

  gameObjectList.first = NULL;
  gameObjectList.last = NULL;
}