/*!
Project (working title): Epoch
\file   PlayerSmoke.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Wrapper around particles to specifically manage the smoke trailing effect that the player (sometimes) has.

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameLevel.h"
#include "PlayerSmoke.h"
#include "EnvironmentalEffects.h"
#include "EnvironmentAssets.h"
#include "GameObject.h"
#include "PlayerEntity.h"

static GameObject* smokeEffect;
/*!
\brief Instantiates the player smoke effect (and hides it). Call this on level load.
*/
void InitializePlayerSmoke()
{
  smokeEffect = CreatePlayerSmoke();

  SetSmoke(particle_inactive);
 
}

/*!
\brief Set smoke particle system state

\param input Use particle_active and particle_inactive to set this.

*/
void SetSmoke(int input)
{
  EffectSource* smokeComponent = (EffectSource*)(smokeEffect->miscData);

  smokeComponent->state = input;
  printf("poooop\n");
}

/*!
\brief Creates the player smoke effect and returns a pointer to the particle system created.
*/
GameObject* CreatePlayerSmoke()
{
  SetParticleAnim(GetAsset_Animation(asset_smokeParticle));
  
  GameObject* particleEffect =  EffectCreate(Vec2(-2.f, -1.f), Vec2(4, 2), Vec2(0,0),
    256, 0.0f, Vec2(0, 4), 0.99f, 0.4f, 26, Vec2(30, 45), 60, GTint(1, 1, 1, 1));
  ParticleApplyBehavior(particleBehavior_linearAlpha, particleEffect);
  return particleEffect;
}

/*!
\brief Given a world coordinate, translates the particle system to the appropriate screen coordinate.
*/
void UpdateSmokePosition(Vector2D worldPos)
{
  if (!smokeEffect)
  {
    
  }
  EffectSource* smokeComponent = (EffectSource*)(smokeEffect->miscData);
  smokeComponent->position = IsoWorldToScreen(&worldPos);
}