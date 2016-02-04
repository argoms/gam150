#pragma once
#include "Vector2D.h"
 
typedef struct PhysicsObject PhysicsObject;
typedef struct PhysicsList PhysicsList;

//linked list integrated into physics object
struct PhysicsObject
{
  
  Vector2D position;
  Vector2D velocity;
  Vector2D size;
  PhysicsObject* next;
  PhysicsObject* prev;
};

struct PhysicsList
{
  PhysicsObject* first;
  PhysicsObject* last;
};

void PhysicsInit();
PhysicsObject* PhysicsCreateObject(Vector2D _position, Vector2D _size);
void PhysicsSimulate();