/**************************************************************************************************
Project       :  EPOCH
File          :  Hazard.c
Author        :  Matt Chasengnou
Created       :  2/19/16
Description   :  Provides functionality for working with audio
ChangeLog
-2/19/16      :  Initial Implementation.
-2/26/16      :  Moved hazard component into miscData

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
**************************************************************************************************/

#include "Hazard.h"

#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* strcat */
#include <stdio.h>    /* printf */

#include "GameObject.h"
#include "Physics.h"

#include <windows.h>  /* shows error messages */

/*-------------------------------------------------------------------------------------------------
DEFINES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
END DEFINES
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : ComponentAdd_Hazard
Description   : Adds and initializes a hazard component to the game object.
Input         : this is the object to attach the hazard to,
                damage is the damage the hazard causes,
                pushForce is the amount of pushback applied to player,
                slowForce is the amount of slowdown applied to player (0 is slower, 1 is normal speed),
                warpPosition is the position the hazard, if teleporting, warps the player,
                hazardTypes is the bitField representing hazard behaviors this hazard may take.
                Use
                HAZARD_DAMAGE             = 1,          Hazard will cause damage
                HAZARD_DAMAGE_CONSTANT    = 1 << 1,     Hazard does constant damage
                HAZARD_DAMAGE_IMPACT      = 1 << 2,     Hazard damages once
                HAZARD_PUSHBACK           = 1 << 3,     Hazard pushes player back
                HAZARD_SLOWDOWN           = 1 << 4,     Hazard slows player down
                HAZARD_WARP               = 1 << 5      Hazard warps player to other location
Output        : No output.
**************************************************************************************************/
void ComponentAdd_Hazard(GameObject *thisObject, int damage, float pushForce, float slowForce,
  Vector2D warpPosition, unsigned int hazardTypes)
{
  /* Add the hazard component. */
  thisObject->miscData = (Component_HAZARD *)malloc(sizeof(Component_HAZARD));

  Component_HAZARD *hazard = (Component_HAZARD *)thisObject->miscData;

  /* Set the hazard component members. */
  hazard->damage = damage;
  hazard->pushForce = pushForce;
  hazard->slowForce = slowForce;
  hazard->warpPosition.x = warpPosition.x;
  hazard->warpPosition.y = warpPosition.y;
  hazard->hazardTypes = hazardTypes;
}

/**************************************************************************************************
Function      : Hazard_OnCollision
Description   : Collision behavior for hazard objects.
Input         : thisObject is the hazard object,
                other is the object colliding with the hazard.
Output        : No output.
**************************************************************************************************/
void Hazard_OnCollision(GameObject *thisObject, GameObject *other)
{
  /* Get the hazard component */
  Component_HAZARD *hazard = (Component_HAZARD *)thisObject->miscData;

  /* If thisObject is a hazard and the other object is a player or enemy, do hazard stuff. */
  if (thisObject->type = entity_hazard && (other->type == entity_player || other->type == entity_enemy))
  {
    /* If thisObjectObject hazard does damage, then damage the other object. */
    if (hazard->hazardTypes & HAZARD_DAMAGE)
      EntityTakeDamage(&other->entity, hazard->damage);

    /* If thisObjectObject hazard does pushback, then push back the other object. */
    if (hazard->hazardTypes & HAZARD_PUSHBACK)
      PhysicsSetVelocity(other->physics, -1.f, -1.f);

    /* If thisObjectObject hazard does slowdown, then slow down the other object. */
    if (hazard->hazardTypes & HAZARD_SLOWDOWN)
    {
      other->physics->velocity.x *= hazard->slowForce;
      other->physics->velocity.y *= hazard->slowForce;
    }

    /* If thisObject hazard warps the player, then do warping. */
    if (hazard->hazardTypes & HAZARD_WARP)
    {
      other->physics->position.x = hazard->warpPosition.x;
      other->physics->position.y = hazard->warpPosition.y;
    }

    /* Destroy thisObject hazard if it is an impact type. */
    if (hazard->hazardTypes & HAZARD_DAMAGE_IMPACT)
      GameObjectDestroy(&thisObject);

    //switch (hazard->hazardTypes)
    //{
    //case HAZARD_DAMAGE:   /* If thisObjectObject hazard does damage, then damage the other object. */
    //  EntityTakeDamage(&other->entity, hazard->damage);

    //case HAZARD_PUSHBACK: /* If thisObjectObject hazard does pushback, then push back the other object. */
    //  PhysicsSetVelocity(other->physics, -1.f, -1.f);
    //  
    //case HAZARD_SLOWDOWN: /* If thisObjectObject hazard does slowdown, then slow down the other object. */
    //  other->physics->velocity.x *= hazard->slowForce;
    //  other->physics->velocity.y *= hazard->slowForce;

    //case HAZARD_WARP:     /* If thisObject hazard warps the player, then do warping. */
    //  other->physics->position.x = hazard->warpPosition.x;
    //  other->physics->position.y = hazard->warpPosition.y;

    //case HAZARD_DAMAGE_IMPACT:  /* Destroy thisObject hazard if it is an impact type. */
    //  GameObjectDestroy(&thisObject);
    //  return;

    //  /* Don't destroy if thisObject hazard is constantly damaging. */
    ////case HAZARD_DAMAGE_CONSTANT:

    //default:
    //  break;
    //}
  }
}