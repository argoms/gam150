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
#include "EnvironmentalEffects.h"
#include "EnvironmentAssets.h"
#include <stdio.h>


extern int level;

typedef struct DoorComponent DoorComponent;

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
    
    printf("DOOR ME \n LEVEL %i \n", level);

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
  //load door animation:
  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  //create door object:
  GameObject* door = GameObjectCreate(PhysicsCreateObject(Vec2(position.x, position.y), 1), GCreateSprite(0, 40, anim2, 1), 0, entity_door);
  door->physics->onCollision = &DoorDefaultOnCollision;
  door->simulate = NULL;

  //make door invisible:
  door->sprite->tint.alpha = 0.0f;

  //create door particle effect:
  SetParticleAnim(GetAsset_Animation(asset_particleDoor));
  Vector2D particleEffectRadius = Vec2(64, 64);
  GameObject* doorParticles = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&door->physics->position), 64, -0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
    particleEffectRadius, 0, GTint(1, 1, 1, 0.f));

  ParticleApplyBehavior(particleBehavior_fadeIn, doorParticles);

  GSortSprite(door->sprite, 0);

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


/*!
\brief Called on initializing door. 
*/
void DoorInit(GameObject* inst)
{
  
}

/*!
\brief Run by door every frame (for graphical effects)
*/
void DoorSimulate(GameObject* instance)
{
}