/*!
Project (working title): Epoch
\file   PlayerEntity.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Handles player-related functionality:
  -controls (movement/attacking)
  -animation

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
#include "PlayerHUD.h"
#include "LevelManager.h"
#include "PlayerSmoke.h"
#include "EnvironmentalEffects.h"
#include "Audio.h"

extern double frameTime;


static int attackDamage = 1;

static GameObject* player; /**< pointer to player object*/
static double attackCooldown; /**< timer before player can attack again*/

static double attackCooldownLength; /**< defined minimum time between attacks (attackCooldown is the timer)*/

//static Animation* tracerAnimation;

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

static int isDead;

//the following enums are used for the player action bit field:
 enum directions 
{
  PLAYER_LEFT = 1,
  PLAYER_RIGHT = 2,
  PLAYER_UP = 4,
  PLAYER_DOWN = 8
};

 enum actions
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


/*!
\brief Call at the start of a level to initialize player values.
*/

static AnimationSet* playerWalkAnims;
static AnimationSet* playerIdleAnims;
static AnimationSet* playerSwordAnims;

static Vector2D playerDirection;

static int PlayerHealth = 10;

void PlayerInit()
{
  
  isDead = 0;

  player = GetPlayerObject();

  //set up player health:
  player->entity->health = PlayerHealth;
  //

  attackCooldown = 0;
  attackCooldownLength = 0.5f;
  attackDamage = 1;
  /*
  tracerAnimation = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);*/

  //set up player movement vars:
  playerMaxSpeed = 1;
  playerAccel = 0.2f;
  playerDrag = 0.7f;

  
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
  
  
}

/*!
\brief Called by player object every frame.
*/
void PlayerSimulate()
{

  if (!player)
  {
    return;
  }

  //move camera to player if player not dead
  if (!isDead)
  {
    
    AEGfxSetCamPosition(player->sprite->x, player->sprite->y);
  }

  
  //printf("%f \n", frameTime);

  if (isDead)
  {
    return;
  }

  //update player stuff:
  UpdatePlayerHealthHUD();
  UpdateSmokePosition(player->physics->position);
  attackCooldown -= frameTime;
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
  //-------Tarrants code ends here no touchie----------------------


  //cheat codes:
  //press 1 for 100 health
  if (AEInputCheckTriggered('1'))
  {
    player->entity->health = 100;
  }

  //press 2 for super speed +damage
  if (AEInputCheckTriggered('2'))
  {
    if (playerAccel < 0.3)
    {
      playerAccel = 0.5f;
    }
    else
    {
      playerAccel = 0.2f;
    }
  }
  
  if (AEInputCheckTriggered('2'))
  {
    if (attackDamage < 15)
    {
      attackDamage = 9999;
    }
    else
    {
      attackDamage = 1;
    }
  }
  
 
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
    //set player's animation speed:
    playerSprite->frameDelay = 3;

    //get movement vector:
    Vector2D input = Vec2((float)(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41)),
      ((float)(AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53)) / 2));
    

    AEInputGetCursorPosition(&mouseX, &mouseY);

    
    //play footstep sounds if velocity isn't 0:
    if (input.x != 0 || input.y != 0)
    {
      stepSoundTimer -= (float)frameTime;
      if(stepSoundTimer < 0)
      {
        Audio_PlaySoundSample("FootstepPlayer1.ogg", 0);
        stepSoundTimer = 0.3f;
        
       
      }
      Vector2DNormalize(&input, &input);
      Vector2DScale(&input, &input, 10);

      //set player animation flags:
      playerAction = PLAYER_WALK;
    }
    else
    {

      stepSoundTimer = 0.1f;
      //if idle, set idle flag and remove walk flag
      if (!(playerAction & PLAYER_IDLE)) //called on the frame where player goes from walk to idle
      {
        playerAction -= PLAYER_WALK;
        playerAction += PLAYER_IDLE;
      }
    }

    //update positions
    //transform vector to use world (iso) coordinates while visually appearing to be straight orthogonal
    player->physics->velocity.x += IsoScreenToWorld(&input).x * playerAccel; 
    player->physics->velocity.y += IsoScreenToWorld(&input).y * playerAccel;
  }
  else if(playerAction & PLAYER_SWORD) //if currently attacking, movespeed is reduced
  {
    Vector2D input = Vec2((float)(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41)),
      ((float)(AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53)) / 2));
    Vector2DScale(&input, &input, 5);
    player->physics->velocity.x += IsoScreenToWorld(&input).x * playerAccel; 
    player->physics->velocity.y += IsoScreenToWorld(&input).y * playerAccel;


    playerSprite->frameDelay = 1;

    //stop animation after it's done and set state back to idle
    if (playerSprite->frame == 12)
    {
      playerAction -= PLAYER_SWORD;
      playerAction += PLAYER_IDLE;
    }
  }

  //apply drag to player movement
  Vector2DScale(&(player->physics->velocity), &(player->physics->velocity), playerDrag);

}

/*!
\brief DEPRECATED internal, used to snap player to direction given by input vector

\param _input Vector between player (center of screen) and mouse
*/
static void SnapVector(Vector2D* _input)
{
  //get input dir
  float directionAngle = atan2f(_input->y, _input->x);
  float fortyFiveDegrees = 0.785398f;
  directionAngle = lroundf(directionAngle / fortyFiveDegrees) * fortyFiveDegrees;
  
  //convert mouse angle into world angle (iso projection means x is twice as much as y)
  _input->x *= 2;
  Vector2DNormalize(_input, _input);
  _input->x = cosf(directionAngle);
  _input->y = sinf(directionAngle);

  //change player sprite direction:
  playerAction = playerAction & (~(PLAYER_LEFT + PLAYER_RIGHT + PLAYER_UP + PLAYER_DOWN));

 // printf("playeraction: %i", playerAction);
}

/*!
\brief Contains behavior for when the player attacks
*/
static void PlayerAttack()
{
  //randomly pick sword swing sound
  if (AERandFloat() < 0.5)
  {
    Audio_PlaySoundSample("SwordSwing1.ogg", 0);
  }
  else
  {
    Audio_PlaySoundSample("SwordSwing2.ogg", 0);
  }

  //set up animation:
  playerAction = PLAYER_SWORD;
  playerSprite->frame = 0;

  

  //set player direction to look at mouse
  AEInputGetCursorPosition(&mouseX, &mouseY);
  mouseX += (long)((AEGfxGetWinMaxX() - AEGfxGetWinMinX()) / -2);
  mouseY += (long)((AEGfxGetWinMaxY() - AEGfxGetWinMinY()) / -2);
  Vector2D mousePos = Vec2((float)mouseX, (float)mouseY * -1);
  mousePos = IsoScreenToWorld(&mousePos);
  Vector2DNormalize(&mousePos, &mousePos);
  playerDirection = mousePos;

  
  
  //EntityApplyKnockback(player->entity, &mousePos);

  attackCooldown = attackCooldownLength;

  //create attack collider (tracer) object:
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(player->physics->position.x + mousePos.x, player->physics->position.y + mousePos.y), 1),
    GCreateSprite(0, 40, 0, 1),
    0,
    entity_friendlyProjectile);
  tracer->syncSpritePhysics = 1;
  tracer->simulate = &TracerSimulate;
  tracer->physics->onCollision = &TracerFriendlyProjectileCollision;

  //push player in the direction that they attacked:
  Vector2DScale(&mousePos, &mousePos, 0.25);
  EntityApplyKnockback(player->entity, &mousePos);
}

/*!
\brief Removes attack colliders after 1 game loop
*/
void TracerSimulate(GameObject* _self)
{
  GameObjectDestroy(&_self);
}

/*!
\brief Collision handler for player's attacks
*/
void TracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  
  //if other object is ane nemy
  if (_otherObject && _otherObject->type == entity_enemy)
  {
    //randomly play one of two sword clash sounds:
    if (AERandFloat() > 0.5f)
    {
      Audio_PlaySoundSample("SwordClash1.ogg", 0);
    }
    else
    {
      Audio_PlaySoundSample("SwordClash2.ogg", 0);
    }

    //printf("YOU HIT ENEMY FOR %i DAMAGE\n", attackDamage);


    //actually doing damage:
    if (Vector2DSquareLength(&player->physics->velocity) > 0.065f)
    {
      //more damage if dashing while attacking
      EntityTakeDamage(&_otherObject->entity, attackDamage * 3);
    }
    else
    {
      EntityTakeDamage(&_otherObject->entity, attackDamage);
    }
  }

  //destroy own collider after hitting something:
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
  case PLAYER_WALK:
    playerDirection = (player->physics->velocity); //running also updates player's current direction
    playerSprite->animation = AnimationPlay(playerWalkAnims, &(player->physics->velocity));
    break;


  case PLAYER_IDLE:
    playerSprite->animation = AnimationPlay(playerIdleAnims, &playerDirection);
    break;

  case PLAYER_SWORD:
    playerSprite->animation = AnimationPlay(playerSwordAnims, &playerDirection);
    break;
  }
  playerSprite->x = player->sprite->x;
  playerSprite->y = player->sprite->y;
}

/*!
\brief called when player dies
*/
void OnPlayerKilled(void)
{
  //do nothing if already dead
  if (isDead)
  {
    return;
  }

  //turn off player smoke effect
  SetSmoke(particle_inactive);


  printf("\n***\n***\nYOU DIED SO NOW YOU'RE IN MAIN MENU WOOO\n***\n***\n");
  player->sprite->tint.alpha = 0;
  Audio_PlaySoundSample("death.ogg", 0);
  //PhysicsRemoveObject(&player->physics);
  DeathTimerStart(player->physics->position);
  isDead = 1;
  //LevelSetNext(level_deathScreen);
}

//TARRANT'S CODE PAST THIS POINT

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

// tarrants cheat to restore hp
// im gonna map m to this
void RestoreHealth(GameObject* obj)
{
  if (obj == NULL)
  {
    return;
  }

  if (obj->entity == NULL)
  {
    return;
  }

  // who hardcoded ten, now i gotta do it
  if (obj->entity->health < 10)
  {
    obj->entity->health = 10;
  }
}

