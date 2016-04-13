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
#include "EnvironmentAssets.h"
#include "Audio.h"
#include <stdio.h>
#include "ScreenShake.h"

static float GATE_FADE_DIST = 5.f;


/*!
\brief Creates a gate object in the world
*/
GameObject* CreateWorldGate(Vector2D position, int orientation)
{

  Entity* gateEntity = malloc(sizeof(Entity));
  gateEntity->health = 5;
  
  /*
  switch (orientation)
  {
  case gate_vertical:
    break;
  case gate_horizontal:
    break;
  }*/

  //grab an animation based on the orientation of the gate
  Animation* anim2 = GetGateAnimation(orientation);

  //create the game object
  GameObject* newGate = GameObjectCreate(PhysicsCreateObject(Vec2(position.x, position.y), 1), GCreateSprite(position.x, position.y, anim2, 1), gateEntity, entity_gate);
  
  newGate->simulate = &GateSimulate;
  newGate->entity->onEntityKilled = &GateOpened;

  //set up gate component
  newGate->miscData = (WorldGate*)malloc(sizeof(WorldGate));
  WorldGate* gateData = GetWorldGate(newGate);
  gateData->positionX = (int)position.x;
  gateData->positionY = (int)position.y;
  gateData->orientation = orientation;
  gateData->status = gate_open;

  //create an array of pointers to particle effects, with one particle effect per tile length of the gate
  for (int i = 0; i < GATE_LENGTH; i++)
  {
    gateData->particleSystems[i] = NULL;
  }
  newGate->sprite->tint.alpha = 0.0f;

  //printf("created a gate. %f, %f\n", position.x, position.y);
  return newGate;
}



/*!
\brief Killed the gate! D: Called when a room is completed and the gates open again.
*/
void GateOpened(GameObject* DeadGate)
{
  //create a gate explosion particle
  SetParticleAnim(GetAsset_Animation(asset_particleGate));
  Vector2D particleEffectRadius = Vec2(64, 64);
  GameObject* particleEffect = EffectCreate(Vec2(-10.f, -5.f), Vec2(20, 10), IsoWorldToScreen(&DeadGate->physics->position),
    16, -1.0f, Vec2(-4, 8), 0.9f, 0.5f, 32, particleEffectRadius, 0, GTint(1, 1, 1, 1.f));
  ParticleSetLifetime(particleEffect, 0.1f);
  ParticleApplyBehavior(particleBehavior_linearAlpha, particleEffect);

  //play gate open sound
  Audio_PlaySoundSample("RoomComplete.ogg", 0);

  //remove particle effects attached to the gate
  WorldGate* gateComponent = GetWorldGate(DeadGate);
  for (int i = 0; i < GATE_LENGTH; i++)
  {
    if (gateComponent->particleSystems[i])
    {
      //printf("ONEWASFOUND\n");
      EffectRemove(gateComponent->particleSystems[i]);
    }
  }

  //set the collision flags of the gate-occupied areas back to 0 (no collision)
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
\brief Called when the gate closes (player enters a room). This makes the room impossible to exit until the enemies are clearead.
*/
void GateClosed(GameObject* inst)
{
  AddScreenShake(0.12, 100);
  Audio_PlaySoundSample("RoomClosed.ogg", 0);


  WorldGate* gateComponent = GetWorldGate(inst);
  gateComponent->status = gate_closed;

  
  
  //set up particle systems corresponding to the gate's occupied tiles: (this is hard coded for 3-tile wide paths at the moment)
  Animation* particle = GetAsset_Animation(asset_particleGate);
  SetParticleAnim(particle);

  Vector2D particleEffectRadius = Vec2(64, 64);

  //the origin point particle system occurs on both horizontal and vertical gates
  gateComponent->particleSystems[0] = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&inst->physics->position), 32, 0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
    particleEffectRadius, 0, GTint(1, 1, 1, 0.1f));

  //other than that, create particle systems according to gate orientation
  if (gateComponent->orientation == gate_horizontal)
  {
    Vector2D offsetPos = inst->physics->position;
    ++offsetPos.x;
    gateComponent->particleSystems[1] = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&offsetPos), 32, 0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
      particleEffectRadius, 0, GTint(1, 1, 1, 0.1f));
    offsetPos.x -= 2;

    gateComponent->particleSystems[2] = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&offsetPos), 32, 0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
      particleEffectRadius, 0, GTint(1, 1, 1, 0.1f));
  }

  else if (gateComponent->orientation == gate_vertical)
  {
    Vector2D offsetPos = inst->physics->position;
    ++offsetPos.y;
    gateComponent->particleSystems[1] = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&offsetPos), 32, 0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
      particleEffectRadius, 0, GTint(1, 1, 1, 0.1f));
    offsetPos.y -= 2;

    gateComponent->particleSystems[2] = EffectCreate(Vec2(-2.f, -2.f), Vec2(4, 4), IsoWorldToScreen(&offsetPos), 32, 0.0f, Vec2(4, 3), 0.99f, 0.5f, 0,
      particleEffectRadius, 0, GTint(1, 1, 1, 0.1f));
  }
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
  if (GetPlayerObject()->sprite)
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
}

/*!
\brief Getter for WorldGate misc data
*/
 WorldGate* GetWorldGate(GameObject* input)
{
  return ((WorldGate*)(input->miscData));
}

/*!
\brief Makes gates fade in more when player gets closer
*/
void GateSimulate(GameObject* inst)
{
  if (GetWorldGate(inst)->status == gate_closed)
  {
    float dist = PhysicsDistSQ(GetPlayerObject()->physics->position, inst->physics->position);
    if (dist > GATE_FADE_DIST)
    {
      dist = GATE_FADE_DIST;
    }

    inst->sprite->tint.alpha = 0.2f + ((GATE_FADE_DIST - dist) / GATE_FADE_DIST) * 0.5f;
  }
}