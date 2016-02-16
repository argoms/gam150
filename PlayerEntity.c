#include "PlayerEntity.h"
#include "GameObject.h"
#include "Physics.h"
#include "Entity.h"
#include "Isometric.h"
#include "GameLevel.h"
#include "AEEngine.h"

extern double frameTime;

static GameObject* player; /**< pointer to player object*/
static double attackCooldown; /**< timer before player can attack again*/

static double attackCooldownLength; /**< defined minimum time between attacks (attackCooldown is the timer)*/

/*!
\brief Call at the start of a level to initialize player values.
*/
void PlayerInit()
{
  player = GetPlayerObject();
  attackCooldown = 0;
  attackCooldownLength = 0.75;
}

/*!
\brief Called by player object every frame.
*/
void PlayerSimulate()
{
  attackCooldown -= frameTime;
  //printf("%f \n", frameTime);

  PlayerInput();
}

/*!
\brief Handles player input
*/
void PlayerInput()
{
  Animation* anim = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  //attacking:
  if (attackCooldown < 0 && AEInputCheckCurr(1))
  {
    attackCooldown = attackCooldownLength;
    GameObject* tracer = GameObjectCreate(
      PhysicsCreateObject(Vec2(player->physics->position.x - 1, player->physics->position.y), 1),
      GCreateSprite(0, 40, anim, 1),
      0,
      entity_friendlyProjectile);
    tracer->syncSpritePhysics = 1;
    tracer->simulate = &TracerSimulate;
    tracer->physics->onCollision = &TracerFriendlyProjectileCollision;

    printf("M1\n");
  }

  //player movement:

  //get movement vector:
  Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
    AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53));

  //transform vector to use world (iso) coordinates while visually appearing to be straight orthogonal
  if (input.x != 0 || input.y != 0)
  {
    Vector2DNormalize(&input, &input);
    Vector2DScale(&input, &input, 10);
  }

  //update positions
  player->physics->velocity = IsoScreenToWorld(&input);
  GSortSprite(player->sprite, player->physics->velocity.y);



}

/*!
\brief simulates attack tracers
*/
void TracerSimulate(GameObject* _self)
{
  //printf("%p || %p \n", &_self, &player);
  _self->simulate = 0;
  
}

void TracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  printf("%i", _otherObject->type);

  if (_otherObject->type == entity_enemy)
  {
    printf("AAA");
    EntityTakeDamage(&_otherObject->entity, 1);
    
  }
}