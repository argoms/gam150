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
#include "MapGen.h"
#include "EnvironmentalEffects.h"
#include "EnvironmentAssets.h"

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
      //damage number popup
      DamageTextCreate((*_entity)->owner->physics->position, _damage);

      //actualy modify health value
      (*_entity)->health -= _damage;
      (*_entity)->wasDamaged = 1; // just got damaged


      //make a particle effect burst thingy
      Vector2D particleEffectRadius = Vec2(64, 64);
      GameObject* inst = (*_entity)->owner;
      SetParticleAnim(GetAsset_Animation(asset_particleHitEnemy));
      GameObject* particleEffect =  EffectCreate(Vec2(-10.f, -5.f), Vec2(20, 10), IsoWorldToScreen(&inst->physics->position), 
        16, -1.0f, Vec2(-4, 8), 0.9f, 0.5f, 32, particleEffectRadius, 0, GTint(1, 1, 1, 1.f));
      ParticleSetLifetime(particleEffect, 0.1f);
      ParticleApplyBehavior(particleBehavior_linearAlpha, particleEffect);

      if (BREIF_INVULNERABILITY_ENABLED)//if we allow brief invulnerability
      {
        BriefInvulnerability((*_entity)->owner, ONLY_PLAYER_BREIF_INVINCIBILITY_AFTER_DAMAGED);  //make the owner briefly invulnerable after taking damage
        (*_entity)->invincibilityRecoveryTime = 0; /* if it was recovering make it invincible */
      }
        
      //handle death (health 0 or less)
      if ((*_entity)->health < 1)
      {
        if ((*_entity)->owner->type == entity_enemy)
        {
          EnemyKilled((*_entity)->owner->parent);
        }
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