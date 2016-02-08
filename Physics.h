/*!
\file   Physics.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic physics/collision implementation.

Weird quirks:
Tilemap treats physics objects as squares, while they treat each other as circles.
This isn't strictly intended, and you're welcome to try and chang everything to circles.
*/
#pragma once
#include "Vector2D.h"

typedef struct PhysicsObject PhysicsObject;
typedef struct PhysicsList PhysicsList;
typedef struct GameObject GameObject;
/*!
\struct PhysicsObject
\brief linked list integrated into physics object
*/
struct PhysicsObject
{
  
  Vector2D position; /**< world position of object*/
  Vector2D velocity; /**< world velocity of object*/
  float size; /**< world size of object*/
  PhysicsObject* next; /**< next object in linked list*/
  PhysicsObject* prev; /**< previous object in linked list*/
  int active; /**< whether or not object should be simulated*/
  GameObject* owner; /**< game object that this physics object belongs to*/
  
};

/*!
\struct PhysicsList
\brief contains simple info about the active list of physics objects
*/
struct PhysicsList
{
  PhysicsObject* first; /**< first object in physics linked list*/
  PhysicsObject* last; /**< last object in physics linked list*/
};

void PhysicsInit();
PhysicsObject* PhysicsCreateObject(Vector2D _position, float _size);
void PhysicsSimulate();
static void PhysicsTileCollisions(PhysicsObject* _instance);
void PhysicsRemoveObject(PhysicsObject** _input);