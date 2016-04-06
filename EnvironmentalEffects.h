#pragma once
#include "GameObject.h"
#include "Vector2D.h"

enum convertType
{
  transform_none,
  transform_worldToScreen,
  transform_screenToWorld
};
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position,
  int density, float emitDelay, int convertType, float damping, float particleLifeTime);
void SetParticleAnim(Animation* input); //temporary debug for now;