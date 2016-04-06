#pragma once
#include "GameObject.h"
#include "Vector2D.h"

enum convertType
{
  transform_none,
  transform_worldToScreen,
  transform_screenToWorld
};

//the number of parameters in this function is becoming ludicrous, if only we had these things called function overloading and default parameters.
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density,
  float emitDelay, Vector2D zVelocityVariance, float damping, float particleLifeTime,
  float zPosition, Vector2D positionVariance,
  float zPositionVariance, Tint particleTint);
void SetParticleAnim(Animation* input); //temporary debug for now;