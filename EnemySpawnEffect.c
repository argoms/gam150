/*!
Project (working title): Epoch
\file   EnemySpawnEffect.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Particle effect preset for when enemies spawn.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemySpawnEffect.h"

/*!
\brief Creates a puff of smoke at enemy spawn point.
\param position Position to spawn effect at.
\param size Number of particles to spawn (intensity of effect)
*/
void CreateEnemySpawnEffect(Vector2D position, int size)
{
  Vector2D puffPos = IsoWorldToScreen(&position); //convert to screen pos because that's what particles work in

  SetParticleAnim(GetAsset_Animation(asset_smokeParticle));

  GameObject* particleEffect = EffectCreate(Vec2(-5.f, -3.f), Vec2(10, 6), puffPos,
    size, -1.0f, Vec2(0, 4), 0.9f, 0.5f, 26, Vec2(10, 15), 0, GTint(1, 1, 1, 1));
  ParticleApplyBehavior(particleBehavior_linearAlpha, particleEffect);
  ParticleSetLifetime(particleEffect, 0.5f);
}