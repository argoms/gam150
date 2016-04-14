#include "Graphics.h"
#include "Physics.h"
#include "HealthPickup.h"
#include "Entity.h"

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
}

void HealthPickupCollide(GameObject* thisObject, GameObject* otherObject)
{
  if (otherObject->type == entity_player && thisObject->type == entity_healthpowerup)
  {
    otherObject->entity->health = HealthBoost;
    GameObjectDestroy(&thisObject);
  }
}
