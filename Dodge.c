#include "Dodge.h"
#include "ParticleSystems(Redo).h"
// CALL THIS EVERY FRAME

static int color_changed = 0;

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
      //printf("recovering\n");
    }
    else if (ent->invincibilityRecoveryTime <= 0) //done recovering
    {
      //printf("DONE RECOVERING\n");
      //do stuff, all cooldowns are done
      //play a sound or something to indicate that it is done
    }    
  }         //CANNOT BE DAMAGED
  else if (ent->canBeDamaged == 0)  //cant be damaged, invincible
  {
    //remember to set the invincibility time to whatever i defined it to be
    if (ent->invincibilityTime > 0) //still invincibility
    {
      ent->invincibilityTime--;                               /* decrement invincibility duration */


      if (!color_changed)
      {
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
		int i;

      ent->canBeDamaged = 1;                                   /* make the entity damagable          */
      ent->invincibilityRecoveryTime = PLAYER_IFRAME_RECOVORY; /* set the entity to start recovering */
      //Vector2DZero(obj_vel);                                   /* set the velocity to zero           */

	  for (i = 0; i < 4; i++)
	  {
		  if (pDodgeSmoke[i]->PS_Burst->ShutDown)
		  {
			  pDodgeSmoke[i]->PS_Burst->StartPosX = obj->sprite->x;
			  pDodgeSmoke[i]->PS_Burst->StartPosY = obj->sprite->y;
			  Start_PS(pDodgeSmoke[i]);
			  break;
		  }
	  }

      //reset color
      ResetColor(obj);
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
	int i;

  //if the key is pressed and is NOT CURRENTLY INVINCIBLE, IT CAN BE DAMAGED
  if (AEInputCheckReleased(input_key))
  {
   int poop = 1 + 1;
  }

  if(AEInputCheckReleased(input_key) && obj->entity->canBeDamaged == 1 && obj->entity->invincibilityRecoveryTime <= 0)
  {
    Vector2DScale(&(obj->physics->velocity), &(obj->physics->velocity), 8);

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
    printf("dodged, is invincibleLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");

	for (i = 0; i < 4; i++)
	{
		if (pDodgeSmoke[i]->PS_Burst->ShutDown)
		{
			pDodgeSmoke[i]->PS_Burst->StartPosX = obj->sprite->x;
			pDodgeSmoke[i]->PS_Burst->StartPosY = obj->sprite->y;
			Start_PS(pDodgeSmoke[i]);
			break;
		}
	}
  }
}

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

  obj_sprite->tint.red   = obj_sprite->tint.red   / DODGE_RED_MODIFIER;
  obj_sprite->tint.green = obj_sprite->tint.green / DODGE_GREEN_MODIFIER;
  obj_sprite->tint.blue = obj_sprite->tint.blue   / DODGE_BLUE_MODIFIER;
  obj_sprite->tint.alpha = obj_sprite->tint.alpha / DODGE_ALPHA_MODIFIER;
  color_changed = 0;
}

