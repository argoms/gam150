/**************************************************************************************************
Author        :   Matt Chasengnou
File          :   Hazards.c
Created       :   2/19/16
Description   :   Provides functionality for working with audio
ChangeLog
-2/19/16      :   Initial Implementation.
**************************************************************************************************/
#ifndef HAZARD_H
#define HAZARD_H


#include "Vector2D.h"   /* Vector2D */
#include "GameObject.h" /* GameObject */

/*-----------------
enums and structs for hazard component
-------------------*/

/* Hazard types for the hazard implementation. */
enum HAZARD_TYPES
{
  HAZARD_DAMAGE             = 1,         /* Hazard will cause damage */
  HAZARD_DAMAGE_CONSTANT    = 1 << 1,    /* Hazard does constant damage */
  HAZARD_DAMAGE_IMPACT      = 1 << 2,    /* Hazard damages once */
  HAZARD_PUSHBACK           = 1 << 3,    /* Hazard pushes player back */
  HAZARD_SLOWDOWN           = 1 << 4,    /* Hazard slows player down */
  HAZARD_WARP               = 1 << 5     /* Hazard warps player to other location */
};

typedef struct Component_HAZARD Component_HAZARD;

/* The component information for hazard functionality. Required for hazards. */
struct Component_HAZARD {
  unsigned int hazardTypes;       /* Hazard types */

  int damage;                     /* Damage dealt by the hazard */
  float pushForce;                /* Amount of pushback applied to player */
  float slowForce;                /* How much slowdown is applied to player. 0 is slower, 1 is normal speed */
  Vector2D warpPosition;          /* Where to warp the player */
};

/**************************************************************************************************
Function      : ComponentAdd_Hazard
Description   : Adds and initializes a hazard component to the game object.
Input         : thisObject is the object to attach the hazard to,
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
  Vector2D warpPosition, unsigned int hazardTypes);

/**************************************************************************************************
Function      : Hazard_OnCollision
Description   : Collision behavior for hazard objects.
Input         : this is the hazard object,
other is the object colliding with the hazard.
Output        : No output.
**************************************************************************************************/
void Hazard_OnCollision(GameObject *thisObject, GameObject *other);

#endif // !HAZARD_H

