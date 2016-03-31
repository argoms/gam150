/*!
Project (working title): Epoch
\file   Gate.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for gates that close when the player enters a room.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Gate.h"
#include "GameObject.h"
#include "Isometric.h"
#include "GameLevel.h"
#include "Physics.h"
#include "MapGen.h"
#include "Text.h"



/*!
\brief Creates a gate object in the world
*/
GameObject* CreateWorldGate(Vector2D position, int orientation)
{

  Entity* gateEntity = malloc(sizeof(Entity));
  gateEntity->health = 5;

  switch (orientation)
  {
  case gate_vertical:
    break;
  case gate_horizontal:
    break;
  }
  Animation* anim2 = GetGateAnimation();
  GameObject* newGate = GameObjectCreate(PhysicsCreateObject(Vec2(position.x, position.y), 1), GCreateSprite(position.x, position.y, anim2, 1), gateEntity, entity_gate);
  
  newGate->simulate = NULL;
  newGate->entity->onEntityKilled = &GateOpened;

  newGate->miscData = (WorldGate*)malloc(sizeof(WorldGate));
  
  WorldGate* gateData = GetWorldGate(newGate);
  gateData->positionX = position.x;
  gateData->positionY = position.y;
  gateData->orientation = orientation;
  newGate->sprite->tint.alpha = 0.1f;

  //printf("created a gate. %f, %f\n", position.x, position.y);
  return newGate;
  //IsoTileSet(position.x, position.y, 1);
}



/*!
\brief Killed the gate! D:
*/
void GateOpened(GameObject* DeadGate)
{
  IsoTileSet(GetWorldGate(DeadGate)->positionX, GetWorldGate(DeadGate)->positionY, tile_floor);
  switch (GetWorldGate(DeadGate)->orientation)
  {
  case gate_horizontal:
    IsoTileSet(GetWorldGate(DeadGate)->positionX + 1, GetWorldGate(DeadGate)->positionY, tile_floor);
    IsoTileSet(GetWorldGate(DeadGate)->positionX - 1, GetWorldGate(DeadGate)->positionY, tile_floor);
    break;
  case gate_vertical:
    IsoTileSet(GetWorldGate(DeadGate)->positionX, GetWorldGate(DeadGate)->positionY + 1, tile_floor);
    IsoTileSet(GetWorldGate(DeadGate)->positionX, GetWorldGate(DeadGate)->positionY - 1, tile_floor);
    break;
  }
  GameObjectDestroy(&DeadGate);
}

///*!
//\brief Called by dying enemies
//*/
//void GateRemoveEnemy(GameObject* Target)
//{
//  Target->entity->health--;
//}
//
///*!
//\brief Called on creating enemies
//*/
//void GateAddEnemy(GameObject* Target)
//{
//  Target->entity->health++;
//}


/*!
\brief Simulate function for rooms that contain gates.
*/
void GateRoomSimulate(GameObject* instance)
{
  if (instance->type != entity_room)
  {
    printf("You've assigned gateroomsimulate badly. Oops.");
    abort();
  }

  int roomRadius = (GetRoomSize(instance) - 2) / 2;
  if (PhysicsDistSQ(GetPlayerObject()->physics->position, instance->physics->position) < roomRadius * roomRadius)
  {
    //clear the sim function on gameobject after one execution:
    instance->simulate = NULL;
    CloseRoom(instance);
    //OpenRoom(instance);
    printf("\n \n *** \n room closed \n \n *** \n");
  }


}

/*!
\brief Getter for WorldGate misc data
*/
 WorldGate* GetWorldGate(GameObject* input)
{
  return ((WorldGate*)(input->miscData));
}