#include "Physics.h"

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

void PhysicsSimulate()
{
  if (PhysicsObjectList.first)
  {
    PhysicsObject* instance = PhysicsObjectList.first;
    while (instance)
    {
      //sim physics here
      Vector2DAdd(&instance->position, &instance->position, &instance->velocity);

      //stop that. stop trying things.

      instance = instance->next;
      //printf("a");
    }
    //printf("cycledone");
  }
}