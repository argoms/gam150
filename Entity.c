#include "Entity.h"

/*!
\brief initializes entity vars
*/
void EntityInit(Entity** _entity)
{
  (*_entity)->health = (*_entity)->maxHealth;
  (*_entity)->onEntityKilled = 0;

}

void EntityTakeDamage(Entity** _entity, int damage)
{
  
  (*_entity)->health -= damage;
  if ((*_entity)->health < 1)
  {
    (*_entity)->onEntityKilled();
  }
}