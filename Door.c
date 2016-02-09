#include "Door.h"

/*!
\brief example/default collision event function

\param _thisObject pointer to gameobject of obect 1
\param _otherObject pointer to gameobject of object 2
*/
void DoorDefaultOnCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i||", _otherObject->type);
  if (_thisObject->type = entity_door && _otherObject->type == entity_player)
  {
    //GameObjectDestroy(&_thisObject);
    LevelSetNext(level_mainMenu);
    printf("DOOR ME");
  }
}