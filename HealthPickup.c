#include "Graphics.h"
#include "Physics.h"
#include "HealthPickup.h"
#include "Entity.h"
#include "EnvironmentAssets.h"
#include "EnvironmentalEffects.h"
#include "Isometric.h"

static int HealthBoost = 10;
static Animation* healthAnimation;

void HealthPickupInitialize()
{
  healthAnimation = GCreateAnimation(1, GCreateTexture("animations/heart.png"), GCreateMesh(50.f, 50.f, 1, 1), 1);
}

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

void HealthPickupCollide(GameObject* thisObject, GameObject* otherObject)
{
  if (otherObject->type == entity_player && thisObject->type == entity_healthpowerup)
  {
    otherObject->entity->health = HealthBoost;
    GameObjectDestroy(&thisObject);
    EffectRemove((GameObject*)(thisObject->miscData));
  }
}
