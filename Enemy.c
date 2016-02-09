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
  //printf("%i||", _otherObject->type);
  if (_thisObject->type = entity_door && _otherObject->type == entity_player)
  {
    //GameObjectDestroy(&_thisObject);
    //LevelSetNext(level_mainMenu);
    EntityTakeDamage(&(_otherObject->entity), 1);
    printf("TAKING DAMAGE");
  }
}