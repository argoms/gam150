#pragma once
typedef struct Entity Entity;
/*!
\struct entity
\brief Character objects (enemies, players etc.)
*/
struct Entity
{
  int health; /**< number of hitpoints remaining*/
  int maxHealth; /**< maximum number of hitpoints*/
  void(*onEntityKilled)(); /**< called when entity dies*/
};

void EntityInit(Entity** _entity);
void EntityTakeDamage(Entity** _entity, int damage);