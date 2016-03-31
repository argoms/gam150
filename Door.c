/*!
Project (working title): Epoch
\file   Door.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for doors to move between levels on contact.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Door.h"



#include <stdio.h>


extern int level;

static GameObject* levelDoor;
/*!
\brief Functionality for doors switching levels upon contact.

good as an example/default collision event function to look at when in doubt

\param _thisObject pointer to gameobject of obect 1
\param _otherObject pointer to gameobject of object 2
*/
void DoorDefaultOnCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i||", _otherObject->type);
  if (_thisObject->type == entity_door && _otherObject->type == entity_player)
  {
    //GameObjectDestroy(&_thisObject);
    if (level < 3)
    {
      LevelSetNext(level_town);
    }
    else
    {
      LevelSetNext(level_winScreen);
    }
    
    printf("DOOR ME");

    //alpha hardcoding:
    level++;

  }
}

/*!
\brief Creates a door object at the given position, updating door static var.

\param position Position to spawn door at.
\return Returns a pointer to the door object.
*/
GameObject* DoorCreateDoorAt(Vector2D position)
{
  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  GameObject* door = GameObjectCreate(PhysicsCreateObject(Vec2(position.x, position.y), 1), GCreateSprite(0, 40, anim2, 1), 0, entity_door);
  door->physics->onCollision = &DoorDefaultOnCollision;
  door->simulate = NULL;

  levelDoor = door;
  return door;
}

/*!
\brief Getter function for the current level's door

\return Returns a pointer to the current level's door.
*/
GameObject* DoorGetDoor()
{

  if (levelDoor) //not sure if this check works as intended atm
  {
    return levelDoor;
  } 
  else
  {
    return NULL;
  }

}

