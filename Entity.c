/*!
\file   Entity.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for entities- objects that can take/receive damage.
*/
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
  printf("OW, %i left", (*_entity)->health);
  if ((*_entity)->health < 1)
  {
    (*_entity)->onEntityKilled(&_entity);
  }
}