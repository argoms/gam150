#include "Dodge.h"
// CALL THIS EVERY FRAME
void UpdateEntity(GameObject *obj)
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
  if (ent->health || ent->maxHealth == NULL || ent->health <= 0)
  {
    return;
  }

  /*
  * if the entity has its vulnerabilty flag set
  * decrement the Iframe recovery so the entity can have the option to dodge
  * if it is set, decrement the i frame
  * while canBeDamaged is 0, it is invincible and health cannot be reduced
  * the entity gets a speed bouns while invincible
  */
  if (ent->canBeDamaged) //if can be damaged
  {
    if (ent->invincibilityRecoveryTime > 0)  //if recovering
    {
      ent->invincibilityRecoveryTime--; /* decrement recovery time */
    }
    else if (ent->invincibilityRecoveryTime <= 0) //done recovering
    {
      //do stuff, all cooldowns are done
      //play a sound or something to indicate that it is done
    }    
  }
  else if (ent->canBeDamaged == 0)  //cant be damaged, invincible
  {
    Vector2D *obj_vel = &(obj->physics->velocity);          /* get the obj velocity             */
    //remember to set the invincibility time to whatever i defined it to be
    if (ent->invincibilityTime > 0) //still invincibility
    {
      ent->invincibilityTime--;                               /* decrement invincibility duration */
      Vector2DScale(obj_vel, obj_vel, SPEED_BONUS_MODIFIER);  /* scale the velocity of the object */ //ask if there are any other movement modifications we can do      
    }
    else if (ent->invincibilityTime <= 0) // no longer invincible
    {
      ent->canBeDamaged = 1;                                   /* make the entity damagable          */
      ent->invincibilityRecoveryTime = PLAYER_IFRAME_RECOVORY; /* set the entity to start recovering */
      Vector2DZero(obj_vel);                                   /* set the velocity to zero           */
    }        
  }
}

//CALL THIS FOR PLAYER INPUT
void Dodge(int input_key)
{

}

