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
static int attackDamage;
static Animation* tracerAnimation;

signed long mouseX;
signed long mouseY;

static float playerMaxSpeed;
static float playerAccel;
static float playerDrag;

/*!
\brief Call at the start of a level to initialize player values.
*/
void PlayerInit()
{
  player = GetPlayerObject();
  attackCooldown = 0;
  attackCooldownLength = 0.75;
  attackDamage = 100;
  tracerAnimation = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  //set up player movement vars:
  playerMaxSpeed = 1;
  playerAccel = 0.2;
  playerDrag = 0.7;
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
  

  //attacking:
  if (attackCooldown < 0 && AEInputCheckCurr(1))
  {
    

    AEInputGetCursorPosition(&mouseX, &mouseY);

    //HARD CODING MAGIC NUMBERS SPOOOOOKY
    mouseX += -400;
    mouseY += -300;
    /***/

    //printf("%i, %i|||", mouseX, mouseY);
    //AEGfxConvertScreenCoordinatesToWorld(mouseX, mouseY, &(mousePos.x), &(mousePos.y));
    Vector2D mousePos = Vec2(mouseX, mouseY * -1);
    
    mousePos = IsoScreenToWorld(&mousePos);
    Vector2DNormalize(&mousePos, &mousePos);

    attackCooldown = attackCooldownLength;
    GameObject* tracer = GameObjectCreate(
      PhysicsCreateObject(Vec2(player->physics->position.x + mousePos.x, player->physics->position.y + mousePos.y), 1),
      GCreateSprite(0, 40, tracerAnimation, 1),
      0,
      entity_friendlyProjectile);
    tracer->syncSpritePhysics = 1;
    tracer->simulate = &TracerSimulate;
    tracer->physics->onCollision = &TracerEnemyProjectileCollision;

    printf("M1\n");
  }

  //player movement:

  //get movement vector:
  Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
    ((float)(AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53)) / 2));

  //transform vector to use world (iso) coordinates while visually appearing to be straight orthogonal
  if (input.x != 0 || input.y != 0)
  {
    Vector2DNormalize(&input, &input);
    Vector2DScale(&input, &input, 10);
  }

  //update positions
  player->physics->velocity.x += IsoScreenToWorld(&input).x * playerAccel; //= IsoScreenToWorld(&input);
  player->physics->velocity.y += IsoScreenToWorld(&input).y * playerAccel;
  Vector2DScale(&(player->physics->velocity), &(player->physics->velocity), playerDrag);
  
  GSortSprite(player->sprite, player->physics->velocity.y);



}

/*!
\brief simulates attack tracers, at the moment they just die immediately
*/
void TracerSimulate(GameObject* _self)
{
  //printf("%p || %p \n", &_self, &player);
  //GameObjectDestroy(&_self);
}

void TracerEnemyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i", _otherObject->type);

  if (_otherObject && _otherObject->type == entity_enemy)
  {
    printf("YOU HIT ENEMY FOR %i DAMAGE\n", attackDamage);
    EntityTakeDamage(&_otherObject->entity, attackDamage);
    GameObjectDestroy(&_thisObject);
    
    Vector2D knockbackVector; //first, create the vectWor
    Vector2DSub(&knockbackVector, &_thisObject->physics->position, &_otherObject->physics->position); //based on difference between characters

                                                                                                      //scale vector according to ENEMY_KNOCKBACK_FORCE:
    Vector2DNormalize(&knockbackVector, &knockbackVector);
    Vector2DScale(&knockbackVector, &knockbackVector, -PLAYERS_KNOCKBACK_FORCE);

    //actually apply knockback:
    EntityApplyKnockback(_otherObject->entity, &knockbackVector);
    
  }
}

