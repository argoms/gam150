/*!
Project (working title): Epoch
\file   Entity.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for entities- objects that can take/receive damage.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Entity.h"
#include "GameObject.h"
#include "DamageNumbers.h"
#include "Dodge.h"

#define BREIF_INVULNERABILITY_ENABLED  1    /* allow breif invulnerability after a hit    */
#define BREIF_INVULNERABILITY_DISABLED 0    /* disallow brief invunerability after a hit  */
/*!
\brief initializes entity vars
\param _entity Pointer to pointer to entity object to initialize
*/
void EntityInit(Entity** _entity)
{
  (*_entity)->health = (*_entity)->maxHealth;
  (*_entity)->onEntityKilled = 0;
  (*_entity)->canBeDamaged = 1;
  (*_entity)->invincibilityTime = 1;
  (*_entity)->invincibilityRecoveryTime = 0;

}

/*!
\brief Called to deal damage to a given entity/gameobject

\param _entity Pointer to pointer to entity object that is taking damage.
\param _damage Amount of damage to deal
*/
void EntityTakeDamage(Entity** _entity, int _damage)
{
  if (_entity)
  {
    //allow it to take damage if its flag saying that itcanbedamaged is false
    if ( (*_entity)->canBeDamaged != 0)
    {
      DamageTextCreate((*_entity)->owner->physics->position, _damage);
      (*_entity)->health -= _damage;
      (*_entity)->wasDamaged = 1; // just got damaged

      if (BREIF_INVULNERABILITY_ENABLED)//if we allow brief invulnerability
      {
        BriefInvulnerability((*_entity)->owner, ONLY_PLAYER_BREIF_INVINCIBILITY_AFTER_DAMAGED);  //make the owner briefly invulnerable after taking damage
        (*_entity)->invincibilityRecoveryTime = 0; /* if it was recovering make it invincible */
      }
        
      //printf("OW, %i left", (*_entity)->health);
      if ((*_entity)->health < 1)
      {
        (*_entity)->onEntityKilled(((*_entity)->owner));
      }
    }
  }
}

/*!
\brief Applies knockback to a given entity

\param _target Pointer to entity that is being knocked back
\param _force Vector2D representing knockback force (magnitude of vector representing magnitude of force)
*/
void EntityApplyKnockback(Entity* _target, Vector2D* _force)
{
  _target->owner->physics->velocity.x += _force->x;
  _target->owner->physics->velocity.y += _force->y;
}