#include "Dodge.h"
// CALL THIS EVERY FRAME

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
  if (ent->health == NULL || ent->maxHealth == NULL || ent->health <= 0)
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
      printf("recovering\n");
    }
    else if (ent->invincibilityRecoveryTime <= 0) //done recovering
    {
      printf("DONE RECOVERING\n");
      //do stuff, all cooldowns are done
      //play a sound or something to indicate that it is done
    }    
  }         //CANNOT BE DAMAGED
  else if (ent->canBeDamaged == 0)  //cant be damaged, invincible
  {
    Vector2D *obj_vel = &(obj->physics->velocity);          /* get the obj velocity             */
    //remember to set the invincibility time to whatever i defined it to be
    if (ent->invincibilityTime > 0) //still invincibility
    {
      ent->invincibilityTime--;                               /* decrement invincibility duration */
      //Vector2DScale(obj_vel, obj_vel, SPEED_BONUS_MODIFIER);  /* scale the velocity of the object */ //ask if there are any other movement modifications we can do  
      printf("invincible\n");
    }
    else if (ent->invincibilityTime <= 0) // no longer invincible
    {
      ent->canBeDamaged = 1;                                   /* make the entity damagable          */
      ent->invincibilityRecoveryTime = PLAYER_IFRAME_RECOVORY; /* set the entity to start recovering */
      //Vector2DZero(obj_vel);                                   /* set the velocity to zero           */
      printf("NO LONGER INVINCIBLE\n");
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

  if(AEInputCheckReleased(input_key) && obj->entity->canBeDamaged == 1 && obj->entity->invincibilityRecoveryTime <= 0)
  {
    // do particle effects
    obj->entity->canBeDamaged = 0; /* activate invincibility */
    obj->entity->invincibilityTime = PLAYER_IFRAMES;
    printf("dodged, is invincibleLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
  }
}

