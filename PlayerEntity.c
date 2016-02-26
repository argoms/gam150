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

static Animation* animWalkDown;
static Animation* animWalkLeft;
static Animation* animWalkUp;
static Animation* animWalkRight;
static Animation* animWalkUpLeft;
static Animation* animWalkUpRight;
static Animation* animWalkDownRight;
static Animation* animWalkDownLeft;

static Sprite* playerSprite;

static enum directions 
{
  PLAYER_LEFT = 1,
  PLAYER_RIGHT = 2,
  PLAYER_UP = 4,
  PLAYER_DOWN = 8
};
static int playerDirection;

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

  
  //load animations:
  animWalkDown = GCreateAnimation(12,
    GCreateTexture("animations/player/walkDown.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkLeft = GCreateAnimation(12,
    GCreateTexture("animations/player/walkLeft.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkUp = GCreateAnimation(12,
    GCreateTexture("animations/player/walkUp.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkRight = GCreateAnimation(12,
    GCreateTexture("animations/player/walkRight.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkUpRight = GCreateAnimation(12,
    GCreateTexture("animations/player/walkUpRight.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkUpLeft = GCreateAnimation(12,
    GCreateTexture("animations/player/walkUpLeft.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkDownRight = GCreateAnimation(12,
    GCreateTexture("animations/player/walkDownRight.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  animWalkDownLeft = GCreateAnimation(12,
    GCreateTexture("animations/player/walkDownLeft.png"),
    GCreateMesh(192.f, 192.f, 12, 1),
    1);

  playerSprite = GCreateSprite(0, 0, animWalkLeft, 2);
  playerSprite->offset.x = 0;
  playerSprite->offset.y = 64;
}

/*!
\brief Called by player object every frame.
*/
void PlayerSimulate()
{
  attackCooldown -= frameTime;
  //printf("%f \n", frameTime);

  PlayerInput();
  PlayerAnimations();
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
    tracer->physics->onCollision = &TracerFriendlyProjectileCollision;

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
    playerDirection = 0;
    if (input.x > 0)
    {
      playerDirection += PLAYER_RIGHT;
    } 
    else if (input.x < 0)
    {
      playerDirection += PLAYER_LEFT;
    }

    if (input.y > 0)
    {
      playerDirection += PLAYER_UP;
    }
    else if (input.y < 0)
    {
      playerDirection += PLAYER_DOWN;
    }
  } 
  else
  {
    playerDirection = 0;
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

void TracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i", _otherObject->type);

  if (_otherObject && _otherObject->type == entity_enemy)
  {
    printf("YOU HIT ENEMY FOR %i DAMAGE\n", attackDamage);
    EntityTakeDamage(&_otherObject->entity, attackDamage);
    GameObjectDestroy(&_thisObject);
  }
}

void PlayerAnimations()
{
  //printf("%i", playerDirection);
  switch (playerDirection)
  {
  case PLAYER_DOWN:
    playerSprite->animation = animWalkDown;
    break;
  case PLAYER_DOWN + PLAYER_LEFT:
    playerSprite->animation = animWalkDownLeft;
    break;
  case PLAYER_LEFT:
    playerSprite->animation = animWalkLeft;
    break;
  case PLAYER_DOWN + PLAYER_RIGHT:
    playerSprite->animation = animWalkDownRight;
    break;
  case PLAYER_RIGHT:
    playerSprite->animation = animWalkRight;
    break;
  case PLAYER_UP:
    playerSprite->animation = animWalkUp;
    break;
  case PLAYER_UP + PLAYER_LEFT:
    playerSprite->animation = animWalkUpLeft;
    break;
  case PLAYER_UP + PLAYER_RIGHT:
    playerSprite->animation = animWalkUpRight;
    break;
  }
  playerSprite->x = player->sprite->x;
  playerSprite->y = player->sprite->y;
}

