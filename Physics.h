/*!
\file   Physics.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic physics/collision implementation.
*/
#pragma once
#include "Vector2D.h"
 
typedef struct PhysicsObject PhysicsObject;
typedef struct PhysicsList PhysicsList;

/*!
\struct PhysicsObject
\brief linked list integrated into physics object
*/
struct PhysicsObject
{
  
  Vector2D position; /**< world position of object*/
  Vector2D velocity; /**< world velocity of object*/
  Vector2D size; /**< world size of object*/
  PhysicsObject* next; /**< next object in linked list*/
  PhysicsObject* prev; /**< previous object in linked list*/
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
PhysicsObject* PhysicsCreateObject(Vector2D _position, Vector2D _size);
void PhysicsSimulate();