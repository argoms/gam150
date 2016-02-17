#pragma once
//#include "GameObject.h"
typedef struct Entity Entity;
typedef struct GameObject GameObject;
/*!
\struct entity
\brief Character objects (enemies, players etc.)
*/
struct Entity
{
  GameObject* owner; /**< pointer to owner gameobject of this entity instance*/

  int health; /**< number of hitpoints remaining*/
  int maxHealth; /**< maximum number of hitpoints*/
  
  void(*onEntityKilled)(); /**< called when entity dies*/
};

void EntityInit(Entity** _entity);
void EntityTakeDamage(Entity** _entity, int damage);