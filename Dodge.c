/*!
Project (working title): Epoch
\file   Dodge.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
This file contains function implementations for player dodging/invulnerabilty on hit behavior
*/
#include "Dodge.h"
#include "EnvironmentalEffects.h"
#include "PlayerSmoke.h"

//#include <windows>
// CALL THIS EVERY FRAME

static int color_changed = 0;
static float test_player_drag_low = 0.8f;
static float test_player_drag_reset = 0.7f;
static float test_player_drag_high = 0.6f;
/*************************************************************************/
/*!
\par   Function: UpdateEntityIFs

\par Description: Update the entity vulerabuility status every frame

\param obj pointer to the button game object

*/
/*************************************************************************/
void UpdateEntityIFs(GameObject *obj)
{
  //check if it is null
  if (obj == NULL)
  {

    return;
  }

  //check if the entity is null
  if (obj->entity == NULL)
  {
    return;
  }

  Entity *ent = obj->entity; /* reference the entity */

                             //it should have been destroyed
  if (ent->health == 0 || ent->maxHealth == 0 || ent->health <= 0)
  {
    return;
  }
  CheckDrag();


  /*
  * if the entity has its vulnerabilty flag set
  * decrement the Iframe recovery so the entity can have the option to dodge
  * if it is set, decrement the i frame
  * while canBeDamaged is 0, it is invincible and health cannot be reduced
  * the entity gets a speed bouns while invincible
  */
  //VULNERABLE
  if (ent->canBeDamaged) //if can be damaged
  {
    CheckDrag();
    if (ent->invincibilityRecoveryTime > 0)  //if recovering
    {


      CheckDrag();
      ent->invincibilityRecoveryTime--; /* decrement recovery time */
      Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), DODGE_VELOCITY_SCALE); //slow it down
                                                                                                 //printf("recovering\n");
                                                                                                 //SetPlayerDrag(0.7f);
    }
    else if (ent->invincibilityRecoveryTime <= 0) //done recovering
    {
      // if it is exactly 0 and not moving
      if (ent->invincibilityRecoveryTime == 0)//&& obj->physics->velocity.x == 0.0f && obj->physics->velocity.y == 0.0f)
      {
        //Vector2D *obj_vel = &(obj->physics->velocity);           /* get the velocity                   */
        //Vector2DZero(obj_vel);                                   /* set the velocity to zero           */
      }
      //CheckDrag();
      //printf("DONE RECOVERING\n");
      //do stuff, all cooldowns are done
      //play a sound or something to indicate that it is done
      //SetPlayerDrag(0.7f);//reset the drag

    }
  }         //CANNOT BE DAMAGED
  else if (ent->canBeDamaged == 0)  //cant be damaged, invincible
  {
    CheckDrag();
    //remember to set the invincibility time to whatever i defined it to be
    if (ent->invincibilityTime > 0) //still INVINCIBLE
    {
      // create particles in during the i frames but make sure to do it once every so often 
      //FIX THIS LATER TARRANT OR I WILL KILL U WRITTEN BY TARRANT
      /*
      if (obj->type == entity_player && obj->entity->invincibilityTime % FRAME_DELAY_DODGE_PARTICLES == 0)
      {
        float player_x = obj->physics->position.x; // x position
        float player_y = obj->physics->position.y; // y position
        SpawnDodgeSmokePS(player_x, player_y);         // spawn particles where the player is
      }
      */

      CheckDrag();
      //SetPlayerDrag(0.9);
      ent->invincibilityTime--;                               /* decrement invincibility duration */
      Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), DODGE_VELOCITY_SCALE); //slow it down

      if (!color_changed)
      {
        CheckDrag();
        obj->sprite->tint.alpha = obj->sprite->tint.alpha * DODGE_ALPHA_MODIFIER;//change alpha color
        obj->sprite->tint.red = obj->sprite->tint.red * DODGE_RED_MODIFIER;      //change red color
        obj->sprite->tint.blue = obj->sprite->tint.blue * DODGE_BLUE_MODIFIER;   //change blue color
        obj->sprite->tint.green = obj->sprite->tint.green * DODGE_GREEN_MODIFIER;//change green color

                                                                                 //update flag
        color_changed = 1;
      }
      //Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), SPEED_BONUS_MODIFIER);  /* scale the velocity of the object */ //ask if there are any other movement modifications we can do  
      //printf("invincible\n");
    }
    else if (ent->invincibilityTime <= 0) // no longer invincible
    {
      //SetPlayerDrag(0.3f);
      ent->canBeDamaged = 1;                                   /* make the entity damagable          */
      ent->invincibilityRecoveryTime = PLAYER_IFRAME_RECOVORY; /* set the entity to start recovering */
      SetSmoke(particle_inactive);// turn off particles
                                                               //reset color
      ResetColor(obj);

      // spawn dodge particles if player
      if (obj->type == entity_player)
      {
        float player_x = obj->physics->position.x; // x position
        float player_y = obj->physics->position.y; // y position
        //SpawnDodgeSmokePS(player_x, player_y);         // spawn particles where the player is
      }
      else
      {
        return;
      }
      //obj->sprite->tint.alpha = player_alpha;//change alpha color
      //obj->sprite->tint.red = player_red;    //change red color
      //obj->sprite->tint.blue = player_blue;  //change blue color
      //obj->sprite->tint.green = player_green;//change green color

      //obj->sprite->tint = Gtint(player_red, player_green, player_blue, player_alpha);
      //printf("NO LONGER INVINCIBLE\n");
    }
  }
}

//CALL THIS FOR PLAYER INPUT

/*************************************************************************/
/*!
\par   Function: Dodge

\par Description: start the dodge

\param input_key key that triggers the invincibility

\param obj key that triggers the invincibility

*/
/*************************************************************************/
void Dodge(int input_key, GameObject *obj)
{
  //if the key is pressed and is NOT CURRENTLY INVINCIBLE, IT CAN BE DAMAGED
  if (AEInputCheckReleased(input_key))
  {
    int poop = 1 + 1;
  }

  // If value is between -EPSILON and +EPSILON, make it 0.0
  if (obj->physics->velocity.x > -EPSILON && obj->physics->velocity.x < EPSILON)
    obj->physics->velocity.x = 0.0f;

  // If value is between -EPSILON and +EPSILON, make it 0.0
  if (obj->physics->velocity.y > -EPSILON && obj->physics->velocity.y < EPSILON)
    obj->physics->velocity.y = 0.0f;

  // if the player is no moving 
  if (obj->physics->velocity.x == 0.0f && obj->physics->velocity.y == 0.0f)
  {
    return;
    //IdleDodge(obj); // call idle dodge
  }



  if (AEInputCheckReleased(input_key) && obj->entity->canBeDamaged == 1 && obj->entity->invincibilityRecoveryTime <= 0)
  {

    Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), DODGE_FORCE); // apply force to the player

    //SpawnDodgeSmokePS(float StartPosX, float StartPosY);

    float player_x = obj->physics->position.x; // x position
    float player_y = obj->physics->position.y; // y position
    //SpawnDodgeSmokePS(player_x, player_y);     // spawn particles where the player is
    //SPAWN PARTICLES HERE

    //save original values
    float player_red = obj->sprite->tint.red;
    float player_blue = obj->sprite->tint.blue;
    float player_green = obj->sprite->tint.green;
    float player_alpha = obj->sprite->tint.alpha;

    //obj->physics->velocity.x = obj->physics->velocity.x * SPEED_BONUS_MODIFIER;
    //obj->physics->velocity.y = obj->physics->velocity.y * SPEED_BONUS_MODIFIER;
    // do particle effects
    obj->entity->canBeDamaged = 0; /* activate invincibility */
    obj->entity->invincibilityTime = PLAYER_IFRAMES;
    SetSmoke(particle_active); // turn on particles
    printf("dodged, is zzz\n");
  }
}

/*************************************************************************/
/*!
\par    Function: Dodge2

\par Description: second dodge function b/c c function overloading

\param  keys an array of input keys for the dodge

\param obj - pointer to an object

\param list_size - size of the list

*/
/*************************************************************************/
void Dodge2(int keys[], GameObject *obj, int list_size)
{
  int input_key = 0;
  int i;
  for (i = 0; i < list_size; i++)
  {
    if (AEInputCheckReleased(keys[i]))
    {
      input_key = keys[i];
    }
  }


  // If value is between -EPSILON and +EPSILON, make it 0.0
  if (obj->physics->velocity.x > -EPSILON && obj->physics->velocity.x < EPSILON)
    obj->physics->velocity.x = 0.0f;

  // If value is between -EPSILON and +EPSILON, make it 0.0
  if (obj->physics->velocity.y > -EPSILON && obj->physics->velocity.y < EPSILON)
    obj->physics->velocity.y = 0.0f;

  // if the player is not moving 
  if (obj->physics->velocity.x == 0.0f && obj->physics->velocity.y == 0.0f)
  {
    return;
    //IdleDodge(obj); // call idle dodge
  }



  if (AEInputCheckReleased(input_key) && obj->entity->canBeDamaged == 1 && obj->entity->invincibilityRecoveryTime <= 0)
  {

    Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), DODGE_FORCE);

    float player_x = obj->physics->position.x; // x position
    float player_y = obj->physics->position.y; // y position
    //SpawnDodgeSmokePS(player_x, player_y);     // spawn particles where the player is
    //SPAWN PRTICLES HERE

    //save original values
    float player_red = obj->sprite->tint.red;
    float player_blue = obj->sprite->tint.blue;
    float player_green = obj->sprite->tint.green;
    float player_alpha = obj->sprite->tint.alpha;

    //obj->physics->velocity.x = obj->physics->velocity.x * SPEED_BONUS_MODIFIER;
    //obj->physics->velocity.y = obj->physics->velocity.y * SPEED_BONUS_MODIFIER;
    // do particle effects
    obj->entity->canBeDamaged = 0; /* activate invincibility */
    obj->entity->invincibilityTime = PLAYER_IFRAMES;
    SetSmoke(particle_active);
    printf("dodged, is invincibleLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
  }
}

/*************************************************************************/
/*!
\par    Function: ResetColor

\par Description: resets the colorr

\param  GameObj - a game object

*/
/*************************************************************************/
void ResetColor(GameObject *GameObj)
{
  if (GameObj == NULL)
  {
    return;
  }

  if (GameObj->entity == NULL)
  {
    return;
  }


  if (GameObj->sprite == NULL)
  {
    return;
  }

  Sprite *obj_sprite = GameObj->sprite;

  // undo the value modifications
  obj_sprite->tint.red = obj_sprite->tint.red / DODGE_RED_MODIFIER;
  obj_sprite->tint.green = obj_sprite->tint.green / DODGE_GREEN_MODIFIER;
  obj_sprite->tint.blue = obj_sprite->tint.blue / DODGE_BLUE_MODIFIER;
  obj_sprite->tint.alpha = obj_sprite->tint.alpha / DODGE_ALPHA_MODIFIER;
  color_changed = 0;
  



}

/*************************************************************************/
/*!
\par    Function: BriefInvulnerability

\par Description: grants brief invulnerability

\param  GameObj - a game object

\param PlayerOnly - flag if only the player

*/
/*************************************************************************/
void BriefInvulnerability(GameObject *GameObj, int PlayerOnly)
{
  if (GameObj == NULL)
  {
    return;
  }

  if (GameObj->sprite == NULL)
  {
    return;
  }

  if (GameObj->entity == NULL)
  {
    return;
  }

  //do it only if we have it set for the player only
  if (!(PlayerOnly && GameObj->type == entity_player))
  {
    return;
  }


  if (GameObj->entity->health == 0)
  {
    return;
  }

  //already invincible
  //if (GameObj->entity->invincibilityTime > 0)
  //{
  // return;
  //}

  Entity *ent = GameObj->entity;
  ent->canBeDamaged = 0;  //make it invulnerable
  ent->invincibilityTime = BRIEF_IFRAMES;
  printf("AAAAA");
  SetSmoke(particle_active); // turn on particles
  //ent->wasDamaged = 0;
}

/*************************************************************************/
/*!
\par    Function: IdleDodge

\par Description: dodge whill not moving

\param  GameObj - a game object

*/
/*************************************************************************/
void IdleDodge(GameObject *GameObj)
{
  // do nothing on idle dodge

  
}


/*************************************************************************/
/*!
\par    Function: CheckDrag

\par Description: checks player drag

*/
/*************************************************************************/
void CheckDrag()
{
  //high drag
  int inc_drag_key = 'H'; //set this equal to whatever key   
  int dec_drag_key = 'G';//low drag  
  int reset_drag_key = 'F';
  if (AEInputCheckTriggered(inc_drag_key))
  {
    SetPlayerDrag(test_player_drag_high);
    printf("%f using high drag \n", GetPlayerDrag());
  }
  else
    if (AEInputCheckTriggered(dec_drag_key))
    {
      SetPlayerDrag(test_player_drag_low);
      printf("%f using low drag \n", GetPlayerDrag());
    }
    else
      if (AEInputCheckTriggered(reset_drag_key))
      {
        SetPlayerDrag(test_player_drag_reset);
        printf("%f using reset ", GetPlayerDrag());
      }
}


