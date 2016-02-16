#include "Enemy.h"
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
#include <stdio.h>
void EnemySimulate()
{
}

/*!
\brief example/default collision event function

\param _thisObject pointer to gameobject of obect 1
\param _otherObject pointer to gameobject of object 2
*/
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  printf("%i||", _thisObject->type);
  if (_thisObject->type == entity_enemy && _otherObject->type == entity_player)
  {
    //GameObjectDestroy(&_thisObject);
    //LevelSetNext(level_mainMenu);
    EntityTakeDamage(&(_otherObject->entity), 2);
    printf("TAKING DAMAGE");
  }
}

/*!
\brief called on enemy's death

\param _self pointer to enemy object being killed
*/
void EnemyOnKilled(GameObject* _self)
{
  printf("Ided");
}