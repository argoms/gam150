#include "PlayerEntity.h"
#include "GameObject.h"
#include "Physics.h"
#include "Entity.h"
#include "Isometric.h"
#include "GameLevel.h"
#include "AEEngine.h"
#include <math.h>
#include "Text.h"
#include <string.h>

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

//prototypes for static funcs:
static void PlayerAttack();
static void SnapVector(Vector2D* _input);

//animations:
static Animation* animWalkDown;
static Animation* animWalkLeft;
static Animation* animWalkUp;
static Animation* animWalkRight;
static Animation* animWalkUpLeft;
static Animation* animWalkUpRight;
static Animation* animWalkDownRight;
static Animation* animWalkDownLeft;

static Animation* animIdleDown;
static Animation* animIdleLeft;
static Animation* animIdleRight;
static Animation* animIdleUp;
static Animation* animIdleDownLeft;
static Animation* animIdleDownRight;
static Animation* animIdleUpLeft;
static Animation* animIdleUpRight;

static Animation* animSwordDown;
static Animation* animSwordLeft;
static Animation* animSwordRight;
static Animation* animSwordUp;
static Animation* animSwordDownLeft;
static Animation* animSwordDownRight;
static Animation* animSwordUpLeft;
static Animation* animSwordUpRight;

static Sprite* playerSprite;

//the following enums are used for the player action bit field:
static enum directions 
{
  PLAYER_LEFT = 1,
  PLAYER_RIGHT = 2,
  PLAYER_UP = 4,
  PLAYER_DOWN = 8
};

static enum actions
{
  PLAYER_IDLE = 16,
  PLAYER_WALK = 32,
  PLAYER_SWORD = 64
};

static unsigned int playerAction; //bit field for player action flags:
/*!
Bits 1-4 are for direction (going left/right/up/down or some combination of those)
Bit 5 is active if the player is idle
Bit 6 is active if the player is moving
Bit 7 is active if the player is attacking
*/

static TextString* healthText;
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
  AEGfxVertexList* walkMesh = GCreateMesh(256.f, 256.f, 12, 1); //mesh for walking (12 frames)
  AEGfxVertexList* swordMesh = GCreateMesh(256.f, 256.f, 1, 13); //mesh for sword (13 frames)
  AEGfxVertexList* idleMesh = GCreateMesh(256.f, 256.f, 1, 1); //mesh for idle (1 frames)
  int walkFrames = 12; //number of frames in walk animation
  int idleFrames = 1; //number of frames in idle animation
  int swordFrames = 13;

  //load walking:
  animWalkDown = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkDown.png"),
    walkMesh,
    1);

  animWalkLeft = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkLeft.png"),
    walkMesh,
    1);

  animWalkUp = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkUp.png"),
    walkMesh,
    1);

  animWalkRight = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkRight.png"),
    walkMesh,
    1);

  animWalkUpRight = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkUpRight.png"),
    walkMesh,
    1);

  animWalkUpLeft = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkUpLeft.png"),
    walkMesh,
    1);

  animWalkDownRight = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkDownRight.png"),
    walkMesh,
    1);

  animWalkDownLeft = GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/walkDownLeft.png"),
    walkMesh,
    1);

  //idle:

  animIdleDown = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleDown.png"),
    idleMesh,
    1);

  animIdleLeft = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleLeft.png"),
    idleMesh,
    1);

  animIdleUp = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleUp.png"),
    idleMesh,
    1);

  animIdleRight = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleRight.png"),
    idleMesh,
    1);

  animIdleUpRight = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleUpRight.png"),
    idleMesh,
    1);

  animIdleUpLeft = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleUpLeft.png"),
    idleMesh,
    1);

  animIdleDownRight = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleDownRight.png"),
    idleMesh,
    1);

  animIdleDownLeft = GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/idleDownLeft.png"),
    idleMesh,
    1);

  //sword:
  animSwordDown = GCreateAnimation(1,
    GCreateTexture("animations/player/swordDown.png"),
    swordMesh,
    swordFrames);

  animSwordLeft = GCreateAnimation(1,
    GCreateTexture("animations/player/swordLeft.png"),
    swordMesh,
    swordFrames);

  animSwordUp = GCreateAnimation(1,
    GCreateTexture("animations/player/swordUp.png"),
    swordMesh,
    swordFrames);

  animSwordRight = GCreateAnimation(1,
    GCreateTexture("animations/player/swordRight.png"),
    swordMesh,
    swordFrames);

  animSwordUpRight = GCreateAnimation(1,
    GCreateTexture("animations/player/swordUpRight.png"),
    swordMesh,
    swordFrames);

  animSwordUpLeft = GCreateAnimation(1,
    GCreateTexture("animations/player/swordUpLeft.png"),
    swordMesh,
    swordFrames);

  animSwordDownRight = GCreateAnimation(1,
    GCreateTexture("animations/player/swordDownRight.png"),
    swordMesh,
    swordFrames);

  animSwordDownLeft = GCreateAnimation(1,
    GCreateTexture("animations/player/swordDownLeft.png"),
    swordMesh,
    swordFrames);

  playerSprite = player->sprite;
  //playerSprite = GCreateSprite(0, 0, animWalkLeft, 2); //re-add this code instead of directly pointing to player sprite if you want collision shape under player
  playerSprite->frameDelay = 3;
  playerSprite->offset.x = 0;
  playerSprite->offset.y = 80;

  playerAction = PLAYER_IDLE + PLAYER_DOWN;

  TextInit();
  healthText =  TextCreateHUDString("Health: ", -300, -200);
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
  //8
  
}

/*!
\brief Handles player input
*/
void PlayerInput()
{
  

  //attacking:
  if (attackCooldown < 0 && AEInputCheckCurr(1))
  {
    PlayerAttack();
  }

  //player movement:
  if (!(playerAction & PLAYER_SWORD))
  {
    playerSprite->frameDelay = 3;
    //get movement vector:
    Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
      ((float)(AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53)) / 2));
    

<<<<<<< HEAD
    AEInputGetCursorPosition(&mouseX, &mouseY);
    
    //HARD CODING MAGIC NUMBERS SPOOOOOKY
    mouseX += -400;
    mouseY += -300;
    /***/
=======

    if (input.x != 0 || input.y != 0)
    {
      Vector2DNormalize(&input, &input);
      Vector2DScale(&input, &input, 10);
>>>>>>> refs/remotes/origin/master

      //set player animation flags:
      playerAction = PLAYER_WALK;

      //player directions:
      if (input.x > 0)
      {
        playerAction += PLAYER_RIGHT;
      }
      else if (input.x < 0)
      {
        playerAction += PLAYER_LEFT;
      }

      if (input.y > 0)
      {
        playerAction += PLAYER_UP;
      }
      else if (input.y < 0)
      {
        playerAction += PLAYER_DOWN;
      }
    }
    else
    {

      //if idle, set idle flag and remove walk flag
      if (!(playerAction & PLAYER_IDLE)) //called on the frame where player goes from walk to idle
      {
        playerAction -= PLAYER_WALK;
        playerAction += PLAYER_IDLE;
      }
    }

    //update positions
    //transform vector to use world (iso) coordinates while visually appearing to be straight orthogonal
    player->physics->velocity.x += IsoScreenToWorld(&input).x * playerAccel; //= IsoScreenToWorld(&input);
    player->physics->velocity.y += IsoScreenToWorld(&input).y * playerAccel;
  }
  else if(playerAction & PLAYER_SWORD)
  {
    playerSprite->frameDelay = 1;
    //printf("AAA");
    //stop animation after it's done
    if (playerSprite->frame == 12)
    {
      playerAction -= PLAYER_SWORD;
      playerAction += PLAYER_IDLE;
    }
  }

  
  Vector2DScale(&(player->physics->velocity), &(player->physics->velocity), playerDrag);
  
  GSortSprite(playerSprite, player->physics->velocity.y); //NOTE: having to do this hints at some bugginess somewhere



}

/*!
\brief internal, used to snap player to direction given by input vector

\param _input Vector between player (center of screen) and mouse
*/
static void SnapVector(Vector2D* _input)
{
  //get input dir
  float directionAngle = atan2f(_input->y, _input->x);
  float fortyFiveDegrees = 0.785398;
  directionAngle = lroundf(directionAngle / fortyFiveDegrees) * fortyFiveDegrees;
  
  //note: probably more efficient to switch
  //convert mouse angle into world angle (iso projection means x is twice as much as y)
  _input->x *= 2;
  Vector2DNormalize(_input, _input);
  _input->x = cosf(directionAngle);
  _input->y = sinf(directionAngle);

  //change player sprite direction:
  playerAction = playerAction & (~(PLAYER_LEFT + PLAYER_RIGHT + PLAYER_UP + PLAYER_DOWN));
  printf("playeraction: %i", playerAction);
  if (_input->x > EPSILON)
  {
    playerAction += PLAYER_RIGHT;
    //printf("ri");
  }
  if (_input->x < -EPSILON)
  {
    playerAction += PLAYER_LEFT;
   // printf("li");
  }
  if (_input->y > EPSILON)
  {
    playerAction += PLAYER_UP;
    //printf("ui");
  }
  if (_input->y < -EPSILON)
  {
    playerAction += PLAYER_DOWN;
    //printf("di");
  }
  
  //printf("x: %f, y: %f |", _input->x, _input->y);
  //printf("%f, AAA", directionAngle * (180 / 3.14159265358979));
}

/*!
\brief Contains behavior for when the player attacks
*/
static void PlayerAttack()
{
  //set up animation:
  playerAction = playerAction & (PLAYER_DOWN + PLAYER_UP + PLAYER_LEFT + PLAYER_RIGHT); //strip direction from current flags
  playerAction += PLAYER_SWORD;
  playerSprite->frame = 0;

  


  AEInputGetCursorPosition(&mouseX, &mouseY);

  //HARD CODING MAGIC NUMBERS SPOOOOOKY
  //these are half screen height/widths
  mouseX += -400;
  mouseY += -300;
  /***/

  //printf("%i, %i|||", mouseX, mouseY);
  //AEGfxConvertScreenCoordinatesToWorld(mouseX, mouseY, &(mousePos.x), &(mousePos.y));
  Vector2D mousePos = Vec2(mouseX, mouseY * -1);
  SnapVector(&mousePos);

  mousePos = IsoScreenToWorld(&mousePos);
  Vector2DNormalize(&mousePos, &mousePos);

  
  //EntityApplyKnockback(player->entity, &mousePos);

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
/*!
\brief simulates attack tracers, at the moment they just die immediately
*/
void TracerSimulate(GameObject* _self)
{
  //printf("%p || %p \n", &_self, &player);
  GameObjectDestroy(&_self);
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

/*!
\brief Handles player animation setting based on playerAction flags.
*/
void PlayerAnimations()
{
  //printf("%i", playerAction);
  switch (playerAction)
  {
  //PLAYER WALKING:
  case PLAYER_WALK + PLAYER_DOWN:
    playerSprite->animation = animWalkDown;
    break;
  case PLAYER_WALK + PLAYER_DOWN + PLAYER_LEFT:
    playerSprite->animation = animWalkDownLeft;
    break;
  case PLAYER_WALK + PLAYER_LEFT:
    playerSprite->animation = animWalkLeft;
    break;
  case PLAYER_WALK + PLAYER_DOWN + PLAYER_RIGHT:
    playerSprite->animation = animWalkDownRight;
    break;
  case PLAYER_WALK + PLAYER_RIGHT:
    playerSprite->animation = animWalkRight;
    break;
  case PLAYER_WALK + PLAYER_UP:
    playerSprite->animation = animWalkUp;
    break;
  case PLAYER_WALK + PLAYER_UP + PLAYER_LEFT:
    playerSprite->animation = animWalkUpLeft;
    break;
  case PLAYER_WALK + PLAYER_UP + PLAYER_RIGHT:
    playerSprite->animation = animWalkUpRight;
    break;


  //PLAYER IDLE:
    
  case PLAYER_IDLE + PLAYER_DOWN:
    playerSprite->animation = animIdleDown;
    break;
  case PLAYER_IDLE + PLAYER_DOWN + PLAYER_LEFT:
    playerSprite->animation = animIdleDownLeft;
    break;
  case PLAYER_IDLE + PLAYER_LEFT:
    playerSprite->animation = animIdleLeft;
    break;
  case PLAYER_IDLE + PLAYER_DOWN + PLAYER_RIGHT:
    playerSprite->animation = animIdleDownRight;
    break;
  case PLAYER_IDLE + PLAYER_RIGHT:
    playerSprite->animation = animIdleRight;
    break;
  case PLAYER_IDLE + PLAYER_UP:
    playerSprite->animation = animIdleUp;
    break;
  case PLAYER_IDLE + PLAYER_UP + PLAYER_LEFT:
    playerSprite->animation = animIdleUpLeft;
    break;
  case PLAYER_IDLE + PLAYER_UP + PLAYER_RIGHT:
    playerSprite->animation = animIdleUpRight;
    break;

    //PLAYER SWORD:

  case PLAYER_SWORD + PLAYER_DOWN:
    playerSprite->animation = animSwordDown;
    break;
  case PLAYER_SWORD + PLAYER_DOWN + PLAYER_LEFT:
    playerSprite->animation = animSwordDownLeft;
    break;
  case PLAYER_SWORD + PLAYER_LEFT:
    playerSprite->animation = animSwordLeft;
    break;
  case PLAYER_SWORD + PLAYER_DOWN + PLAYER_RIGHT:
    playerSprite->animation = animSwordDownRight;
    break;
  case PLAYER_SWORD + PLAYER_RIGHT:
    playerSprite->animation = animSwordRight;
    break;
  case PLAYER_SWORD + PLAYER_UP:
    playerSprite->animation = animSwordUp;
    break;
  case PLAYER_SWORD + PLAYER_UP + PLAYER_LEFT:
    playerSprite->animation = animSwordUpLeft;
    break;
  case PLAYER_SWORD + PLAYER_UP + PLAYER_RIGHT:
    playerSprite->animation = animSwordUpRight;
    break;
  }
  playerSprite->x = player->sprite->x;
  playerSprite->y = player->sprite->y;
}

