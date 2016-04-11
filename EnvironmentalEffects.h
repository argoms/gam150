/*!
Project (working title): Epoch
\file   EnvironmentalEffects.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Particle system declarations

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "GameObject.h"
#include "Vector2D.h"


#define MAX_PARTICLES_PER_EFFECT 64 //maximum number of particles that can be instantiated (increasing this increases memory footprint of each particle effect slightly)

typedef struct EffectSource EffectSource;
typedef struct ParticleComponent ParticleComponent;


//pretty self-explanatory, used to see whether a given particle-related system is alive
enum ParticleStates
{
  particle_active,
  particle_inactive
};

/*

*/
enum ParticleBehaviors
{
  particleBehavior_linearAlpha, //decreases alpha of particle by constant
  particleBehavior_fadeIn //the weird pseudo-fireish behavior of door particles
};

struct EffectSource
{
  GameObject* particles[MAX_PARTICLES_PER_EFFECT];

  Vector2D minVelocity; /**< base value of velocity*/
  Vector2D maxVelocity; /**< a random scalar between 0-100% of these are added to velocity*/
  Vector2D position; /**< origin point of particles*/
  Vector2D pLifeTime; /**< duration of each individual particle (in seconds*/
  Vector2D zVelocityVariance; /**< x value represents lowest z-velocity, y value represents an additional value that will be scaled 0-100% and then added to the z-velocity*/
  Vector2D positionVariance; /**< a random scalar between -50% and 50% of these are added to x/y positions (centered around position)*/
  float zPositionVariance; /**< a random scalar between -50% and 50% of this is added to z position*/
  float zPosition;  /**< base z-position (y position without affecting layer*/

  float emitDelayCounter; /**< internal*/
  float emitDelay; /**< time between particle emissions (set 0/negative for burst*/
  float damping; /**< scalar applied to velocity every game loop*/
  int density; /**< number of particles to instantiate (fewer is better for performance)*/
  int state; /**< whether active or inactive (see ParticleStates enum*/

  float lifetime; /**< how much longer the particle will be alive (0 defaults to infinite)*/

  Tint tint; /**< tint value to apply to particles*/
};

//the number of parameters in this function is becoming ludicrous, if only we had these things called function overloading and default parameters.
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density,
  float emitDelay, Vector2D zVelocityVariance, float damping, float particleLifeTime,
  float zPosition, Vector2D positionVariance,
  float zPositionVariance, Tint particleTint);
void SetParticleAnim(Animation* input); //a bit hacky at a glance, but makes managing multiple different particle effects somewhat easier
void EffectRemove(GameObject* inst);
void ParticleSetLifetime(GameObject* inst, float life);
void ParticleApplyBehavior(int behavior, GameObject* inst);