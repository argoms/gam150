/**************************************************************************************************
Author        :   Matt Chasengnou
File          :   Hazards.c
Created       :   2/19/16
Description   :   Provides functionality for working with audio
ChangeLog
-2/19/16      :   Initial Implementation.
**************************************************************************************************/

#include "Hazard.h"

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* strcat */
#include <stdio.h>    /* printf */

#include "GameObject.h"
#include <windows.h>  /* shows error messages */

/*-------------------------------------------------------------------------------------------------
DEFINES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
END DEFINES
-------------------------------------------------------------------------------------------------*/



/**************************************************************************************************
Function      : Hazard_OnCollision
Description   : Collision behavior for hazard objects.
Input         : this is the hazard object,
                other is the object colliding with the hazard.
Output        : No output.
**************************************************************************************************/
void Hazard_OnCollision(GameObject *this, GameObject *other)
{
  /* If this is a hazard and the other object is a player or enemy, do hazard stuff. */
  if (this->type = entity_hazard && (other->type == entity_player || other->type == entity_enemy))
  {
    switch (this->hazardComponent->hazardTypes)
    {
    case HAZARD_DAMAGE:   /* If this hazard does damage, then damage the other object. */
      EntityTakeDamage(&other->entity, this->hazardComponent->damage);
    case HAZARD_PUSHBACK: /* If this hazard does pushback, then push back the other object. */
      // PLACEHOLDER CODE
      printf_s("Hazard_Collision PUSH-BACK");
    case HAZARD_SLOWDOWN: /* If this hazard does slowdown, then slow down the other object. */
      other->physics->velocity.x *= this->hazardComponent->slowForce;
      other->physics->velocity.y *= this->hazardComponent->slowForce;
    default:
      break;
    }
  }
}