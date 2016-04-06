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
#include <stdlib.h>
#include "EnvironmentalEffects.h"


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
  Animation* anim2 = GetGateAnimation(orientation);
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
\brief Killed the gate! D: Called when a room is completed and the gates open again.
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

/*!
\brief Called when the gate closes (player enters a room)
*/
void GateClosed(GameObject* inst)
{
  WorldGate* gateComponent = GetWorldGate(inst);

 // printf("Gatedim: %f, %f", gateDimensions.x, gateDimensions.y);

  Animation* particle = GCreateAnimation(1,
    GCreateTexture("animations/world/cloudTemplate.png"),
    GCreateMesh(24.f, 16.f, 1, 1),
    1);
  SetParticleAnim(particle);
  EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&inst->physics->position), 32, 0.0f, Vec2(4,3), 0.99f, 0.5f, 0,
    Vec2(1, 1), 0, GTint(1, 1, 1, 0.1f));
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