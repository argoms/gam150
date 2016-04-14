/*!
Project (working title): Epoch
\file   Entity.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Contains prototypes/declarations for entities- objects that can take/receive damage.
*/
#pragma once
#include "Vector2D.h"

#define ONLY_PLAYER_BREIF_INVINCIBILITY_AFTER_DAMAGED 1
typedef struct Entity Entity;
typedef struct GameObject GameObject;
/*!
\struct entity
\brief Character objects (enemies, players etc.)
*/
struct Entity
{
  GameObject* owner; /**< pointer to owner gameobject of this entity instance*/

  int health;                   /**< number of hitpoints remaining       */
  int maxHealth;                /**< maximum number of hitpoints         */
  int invincibilityTime;        /**< frames for invincibility            */
  int canBeDamaged;             /**< flag if we can damage the entity    */
  int wasDamaged;               /**< flag if we were damaged by an enemy */
  int invincibilityRecoveryTime;/**< frames for recovering               */  
  void(*onEntityKilled)();      /**< called when entity dies             */
};

void EntityInit(Entity** _entity);
void EntityTakeDamage(Entity** _entity, int damage);
void EntityApplyKnockback(Entity* _target, Vector2D* _force);