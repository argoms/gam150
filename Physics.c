#include "Physics.h"
#include "Isometric.h"

static PhysicsList PhysicsObjectList;

void PhysicsInit()
{
  PhysicsObjectList.first = NULL;
  PhysicsObjectList.last = NULL;
}

PhysicsObject* PhysicsCreateObject(Vector2D _position, Vector2D _size)
{
  PhysicsObject* newObject = malloc(sizeof(PhysicsObject));
  Vector2D zeroVec;
  zeroVec.x = 0;
  zeroVec.y = 0;
  newObject->position = _position;
  newObject->size = _size;
  newObject->velocity = zeroVec;
  newObject->next = NULL;
  newObject->prev = NULL;
  if (!PhysicsObjectList.first)
  {
    PhysicsObjectList.first = newObject;
    PhysicsObjectList.last = newObject;
  }
  else {
    PhysicsObjectList.last->next = newObject;
    PhysicsObjectList.last = newObject;
  }
}

int PhysicsPointTile(float _x, float _y)
{
  return IsoTileGet((int)_x, (int)_y) == 1;
}

void PhysicsSimulate()
{
  if (PhysicsObjectList.first)
  {
    PhysicsObject* instance = PhysicsObjectList.first;
    while (instance)
    {
      //sim physics here
      Vector2DAdd(&instance->position, &instance->position, &instance->velocity);
      
      //tile collisions:


      //instance->position.y += instance->velocity.y;


      //x goes up/right increasing, y goes up/left
      
      
      
      //negative x (left):
      if (IsoTileGet(
        (instance->position.x), 
        (instance->position.y + 0.25))
        == 1 || IsoTileGet(
          (instance->position.x),
          (instance->position.y + 0.75))
        == 1)
      {
        printf("left");
        instance->position.x = (int)(instance->position.x + 0.5);
      }

      //positive x (right):
      if (IsoTileGet(
        (instance->position.x + 1),
        (instance->position.y + 0.25))
        == 1 || IsoTileGet(
          (instance->position.x + 1),
          (instance->position.y + 75))
        == 1)
      {
        printf("right");
        instance->position.x = (int)(instance->position.x + 0.5);
      }



      
      
      //positive y (up):
      if (IsoTileGet(
        (instance->position.x + 0.25),
        (instance->position.y + 1))
        == 1 || IsoTileGet(
          (instance->position.x + 0.75),
          (instance->position.y + 1))
        == 1)
      {
        printf("up");
        instance->position.y = (int)(instance->position.y + 0.5);
      }

      //negative y (down)
      if (IsoTileGet(
        (instance->position.x + 0.25),
        (instance->position.y))
        == 1 || IsoTileGet(
          (instance->position.x + 0.75),
          (instance->position.y))
        == 1)
      {
        printf("down");
        instance->position.y = (int)(instance->position.y + 0.5);
      }
      
      //printf("%3f | %3f", instance->position.x, instance->position.y);

      
      //physics code ending here

      instance = instance->next;
      //printf("a");
    }
    //printf("cycledone");
  }
}