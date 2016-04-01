/*!
Project (working title): Epoch
\file   PlayerEntity.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Handles player-related functionality:
  -controls (movement/attacking)
  -animation
  -health display

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
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
#include "Dodge.h"
#include "EntityAnimation.h"
#include "PlayerAnimations.h"

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

static Sprite* playerSprite;
static float stepSoundTimer;

static int dodge_key = VK_SPACE;

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
Bit 5 is active if the player is idle
Bit 6 is active if the player is moving
Bit 7 is active if the player is attacking
*/

static TextString* healthText;
/*!
\brief Call at the start of a level to initialize player values.
*/

static AnimationSet* playerWalkAnims;
static AnimationSet* playerIdleAnims;
static AnimationSet* playerSwordAnims;

static Vector2D playerDirection;
void PlayerInit()
{
  


  player = GetPlayerObject();

  //set up player health:
  player->entity->health = 30;
  //

  attackCooldown = 0;
  attackCooldownLength = 0.5;
  attackDamage = 10;
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

  
  playerWalkAnims = PlayerAnimationCreateWalk();
  playerIdleAnims = PlayerAnimationCreateIdle();
  playerSwordAnims = PlayerAnimationCreateSword();

  playerSprite = player->sprite;
  //playerSprite = GCreateSprite(0, 0, animWalkLeft, 2); //re-add this code instead of directly pointing to player sprite if you want collision shape under player
  playerSprite->frameDelay = 3;
  playerSprite->offset.x = 0;
  playerSprite->offset.y = 80;

  playerAction = PLAYER_IDLE;

  //shitty alpha fast coding:
  TextInit();
  
  char hpstring[20] = "Health:            ";
  int tempHP = player->entity->health;
  int count = 0;
  while (tempHP > 0 && count < 10)
  {
    //printf("a");
    count++;
    hpstring[6 + count] = 3;
    tempHP -= 10;
  }
  healthText =  TextCreateHUDString(hpstring, -300, -200);
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
  
  //update the i frames
  UpdateEntityIFs(player);

  //-------Tarrants code for dodge no touchie----------------------
  //check key
  //int key = VK_SPACE;  

  
  //int key;
  //key = RIGHT_CLICK;

  int dodge_keys[] = { RIGHT_CLICK, VK_SPACE };
  int key_list_size = 2;
  /*
  if (AEInputCheckTriggered('Z'))
  {
    printf("%d\n", dodge_key);
    if (dodge_key == RIGHT_CLICK)
    {
      dodge_key = VK_SPACE;
    }
    else if (dodge_key == VK_SPACE)
    {
      dodge_key = RIGHT_CLICK;
    }
  }
  */

  //Dodge(dodge_key, player); //check if the key is pressed if so then dodge
  Dodge2(dodge_keys, player, key_list_size);

  /*
  int inc_drag_key = 'H'; //set this equal to whatever key   
  if (AEInputCheckTriggered(inc_drag_key))
  {
    playerDrag = 0.5f;
    //printf("%f dragggg", playerDrag);
  }

  int dec_drag_key = 'G';
  if (AEInputCheckTriggered(dec_drag_key))
  {
    playerDrag = 0.9f;
  }

  int reset_drag_key = 'F';
  if (AEInputCheckTriggered(reset_drag_key))
  {
    playerDrag = 0.7f;
  }
  */
  //-------Tarrants code no touchie----------------------


  if (AEInputCheckTriggered('1'))
  {
    player->entity->health = 100;
  }
  //alpha dumb hardcoding
  {
    char hpstring[20] = "Health:            ";
    int tempHP = player->entity->health;
    int count = 0;
    while (tempHP > 0 && count < 10)
    {
      count++;
      hpstring[6 + count] = 3;
      tempHP -= 10;
    }
    TextRemoveString(healthText);
    healthText = TextCreateHUDString(hpstring, -300, -200);
    //TextHUDStringSet(&healthText, "aaa");
  }
}

/*!
\brief Handles player input
*/
void PlayerInput()
{
  //printf("frame %i \n", playerSprite->frame);
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
    

    AEInputGetCursorPosition(&mouseX, &mouseY);

    
    if (input.x != 0 || input.y != 0)
    {
      stepSoundTimer -= frameTime;
      if(stepSoundTimer < 0)
      {
       Audio_PlaySoundSample("FootstepPlayer1.ogg", 0);
       stepSoundTimer = 0.3;
      }
      Vector2DNormalize(&input, &input);
      Vector2DScale(&input, &input, 10);

      //set player animation flags:
      playerAction = PLAYER_WALK;
    }
    else
    {

      stepSoundTimer = 0.1;
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
    Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
      ((float)(AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53)) / 2));
    Vector2DScale(&input, &input, 5);
    player->physics->velocity.x += IsoScreenToWorld(&input).x * playerAccel; 
    player->physics->velocity.y += IsoScreenToWorld(&input).y * playerAccel;


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
    //playerAction += PLAYER_RIGHT;
    //printf("ri");
  }
  if (_input->x < -EPSILON)
  {
    //playerAction += PLAYER_LEFT;
   // printf("li");
  }
  if (_input->y > EPSILON)
  {
   // playerAction += PLAYER_UP;
    //printf("ui");
  }
  if (_input->y < -EPSILON)
  {
   // playerAction += PLAYER_DOWN;
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
  //randomly pick sword sound
  if (AERandFloat() < 0.5)
  {
    Audio_PlaySoundSample("SwordSwing1.ogg", 0);
  }
  else
  {
    Audio_PlaySoundSample("SwordSwing2.ogg", 0);
  }
  //set up animation:
  //playerAction = playerAction & (PLAYER_DOWN + PLAYER_UP + PLAYER_LEFT + PLAYER_RIGHT); //strip direction from current flags
  playerAction = PLAYER_SWORD;
  playerSprite->frame = 0;

  


  AEInputGetCursorPosition(&mouseX, &mouseY);

  //HARD CODING MAGIC NUMBERS SPOOOOOKY
  //these are half screen height/widths
  /*
  mouseX += -400;
  mouseY += -300;
  */
  mouseX += (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) / -2;
  mouseY += (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) / -2;
  //printf("%f", (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) / -2);
  /***/

  //printf("%i, %i|||", mouseX, mouseY);
  //AEGfxConvertScreenCoordinatesToWorld(mouseX, mouseY, &(mousePos.x), &(mousePos.y));
  Vector2D mousePos = Vec2(mouseX, mouseY * -1);
  //SnapVector(&mousePos);

  mousePos = IsoScreenToWorld(&mousePos);
  Vector2DNormalize(&mousePos, &mousePos);
  playerDirection = mousePos;

  
  
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

  Vector2DScale(&mousePos, &mousePos, 0.25);
  EntityApplyKnockback(player->entity, &mousePos);
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
    Audio_PlaySoundSample("SwordClash1.ogg", 0);
    printf("YOU HIT ENEMY FOR %i DAMAGE\n", attackDamage);
    EntityTakeDamage(&_otherObject->entity, attackDamage);
  }

  if (_otherObject->entity && _otherObject->entity->health <= 0)
  {
    GameObjectDestroy(&_thisObject);
  }
}

/*!
\brief Handles player animation setting based on playerAction flags.
*/
void PlayerAnimations()
{
  switch (playerAction)
  {
    //PLAYER WALKING:
  case PLAYER_WALK:
    playerDirection = (player->physics->velocity);
    playerSprite->animation = AnimationPlay(playerWalkAnims, &(player->physics->velocity));
    break;
  case PLAYER_IDLE:
    playerSprite->animation = AnimationPlay(playerIdleAnims, &playerDirection);
    break;
    //PLAYER SWORD:

  case PLAYER_SWORD:
    playerSprite->animation = AnimationPlay(playerSwordAnims, &playerDirection);
    break;
  }
  playerSprite->x = player->sprite->x;
  playerSprite->y = player->sprite->y;
}

void IncrementPlayerDrag()
{
  playerDrag++;
}

void DecrementPlayerDrag()
{
  if (playerDrag <= 1.0)
  {
    return;//cap it
  }
  playerDrag--;
}

void SetPlayerDrag(float drag)
{
  playerDrag = drag;
}

float  GetPlayerDrag()
{
  return playerDrag;
}