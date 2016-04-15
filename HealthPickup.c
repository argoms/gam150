/*!
Project (working title): Epoch
\file   HealthPickup.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Health pickup powerup that restores health
*/
#include "Graphics.h"
#include "Physics.h"
#include "HealthPickup.h"
#include "Entity.h"
#include "EnvironmentAssets.h"
#include "EnvironmentalEffects.h"
#include "Isometric.h"
#include "Audio.h"

static int HealthBoost = 10;  // Set player's health to this value when picking up the powerup
static Animation* healthAnimation;

/*!
\brief
  Initializes animation for all future health pickups
*/
void HealthPickupInitialize()
{
  healthAnimation = GCreateAnimation(1, GCreateTexture("animations/heart.png"), GCreateMesh(50.f, 50.f, 1, 1), 1);
}

/*!
\brief
  Call this to spawn a health pickup
\param x
  X location of powerup
\param y
  Y location of powerup
*/
void HealthPickupSpawn(float x, float y)
{
  Vector2D position;
  position.x = x;
  position.y = y;

  GameObject* healthPowerup = GameObjectCreate(PhysicsCreateObject(position, 1), 
    GCreateSprite(0.f, 40.f,healthAnimation, 0), NULL, entity_healthpowerup);

  healthPowerup->physics->onCollision = &HealthPickupCollide;
  healthPowerup->type = entity_healthpowerup;

  SetParticleAnim(GetAsset_Animation(asset_particleHeart));

  GameObject* particleEffect = EffectCreate(Vec2(-5.f, -3.f), Vec2(10, 6), IsoWorldToScreen(&position),
    64, 0.1f, Vec2(5, 10), 0.9f, 0.4f, 0, Vec2(60, 60), 10, GTint(1, 1, 1, 1));
  ParticleApplyBehavior(particleBehavior_linearAlpha, particleEffect);

  healthPowerup->miscData = particleEffect;
  //EffectRemove((GameObject*)(healthPowerup->miscData));
}

/*!
\brief
  Handles actual logic when player collides with the powerup
  Plays a sound, resets player health, and destroys the powerup
\param thisObject
  The health pickup object
\param y
  Hopefully the player object
*/
void HealthPickupCollide(GameObject* thisObject, GameObject* otherObject)
{
  if (otherObject->type == entity_player && thisObject->type == entity_healthpowerup)
  {
    otherObject->entity->health = HealthBoost;
    GameObjectDestroy(&thisObject);
    EffectRemove((GameObject*)(thisObject->miscData));

    Audio_PlaySoundSample("coin.ogg", 0);
    SetParticleAnim(GetAsset_Animation(asset_particleHeart));
    GameObject* explosionEffect = EffectCreate(Vec2(-10.f, -10.f), Vec2(20, 20), IsoWorldToScreen(&thisObject->physics->position),
      16, -1.f, Vec2(5, 10), 0.9f, 0.4f, 0, Vec2(60, 60), 10, GTint(1, 1, 1, 0.75));
    ParticleApplyBehavior(particleBehavior_linearAlpha, explosionEffect);
    ParticleSetLifetime(explosionEffect, 0.05f);
  }
}
