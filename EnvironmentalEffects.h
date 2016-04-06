#pragma once
#include "GameObject.h"
#include "Vector2D.h"


#define MAX_PARTICLES_PER_EFFECT 64

typedef struct EffectSource EffectSource;
typedef struct ParticleComponent ParticleComponent;
enum convertType
{
  transform_none,
  transform_worldToScreen,
  transform_screenToWorld
};


enum ParticleStates
{
  particle_active,
  particle_inactive
};

struct EffectSource
{
  GameObject* particles[MAX_PARTICLES_PER_EFFECT];

  Vector2D minVelocity;
  Vector2D maxVelocity;
  Vector2D position;
  Vector2D pLifeTime;
  Vector2D zVelocityVariance;
  Vector2D positionVariance;
  float zPositionVariance;
  float zPosition;

  float emitDelayCounter;
  float emitDelay;
  float damping;
  int density;
  int state;

  Tint tint;
};

//the number of parameters in this function is becoming ludicrous, if only we had these things called function overloading and default parameters.
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density,
  float emitDelay, Vector2D zVelocityVariance, float damping, float particleLifeTime,
  float zPosition, Vector2D positionVariance,
  float zPositionVariance, Tint particleTint);
void SetParticleAnim(Animation* input); //temporary debug for now;
void EffectRemove(GameObject* inst);