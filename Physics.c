/*!
\file   Physics.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic physics/collision implementation.
*/
#include "Physics.h"
#include "Isometric.h"
#include "Vector2D.h"
#include "GameObject.h"
#include <math.h>

static PhysicsList PhysicsObjectList; /**< list of physics objects currently active*/

/*!
\brief Call to initialize physics system
*/
void PhysicsInit()
{
  PhysicsObjectList.first = NULL;
  PhysicsObjectList.last = NULL;
}

/*!
\brief Creates a new physics object and adds it to the list of physics objects.

Note: parameters given in world coordinates (see isometric.c for transformation between world & screen coordinates)
\param _position Position for the new object
\param _size Size in units for the new object.
\return Returns a pointer to the newly created object.
*/
PhysicsObject* PhysicsCreateObject(Vector2D _position, float _size)
{
  //initialize object:
  PhysicsObject* newObject = malloc(sizeof(PhysicsObject));
  Vector2D zeroVec;
  zeroVec.x = 0;
  zeroVec.y = 0;
  newObject->position = _position;
  newObject->size = _size;
  newObject->velocity = zeroVec;
  newObject->next = NULL;
  newObject->prev = NULL;
  newObject->active = 1;
  newObject->onCollision = NULL;

  //update list:
  if (!PhysicsObjectList.first)
  {
    PhysicsObjectList.first = newObject;
    PhysicsObjectList.last = newObject;
  }
  else {
    newObject->prev = PhysicsObjectList.last;
    PhysicsObjectList.last->next = newObject;
    PhysicsObjectList.last = newObject;
  }
}

/*!
\brief returns the tile information at a given point
\param _x x position of the point being checked
\param _y y position of the point being checked
\return Returns the tiledata at the given point.
*/
int PhysicsPointTile(float _x, float _y)
{
  return IsoTileGet((int)_x, (int)_y) == 1;
}

/*!
\brief Simulates one step of physics.
*/
void PhysicsSimulate()
{
  if (PhysicsObjectList.first)
  {
    PhysicsObject* instance = PhysicsObjectList.first;
    while (instance)
    {
      //sim physics here

      //create a pointer to physics objects to test for collisions against
      PhysicsObject* collisionTest = instance->next;
      while (collisionTest)
      {
        float distBetweenSQ = Vector2DSquareDistance(&collisionTest->position, &instance->position);
       
        if (distBetweenSQ <= ((collisionTest->size + instance->size) / 2) * ((collisionTest->size + instance->size) / 2))
        {
          
          if (instance->onCollision)
          {
            instance->onCollision(instance->owner, collisionTest->owner);
          }
          if (collisionTest->onCollision)
          {
            //printf("A");
            collisionTest->onCollision(collisionTest->owner, instance->owner);
          }
          /*if (instance->owner->type == 1 && collisionTest->owner->type == 2)
          {
            printf("a");
          }*/
          
          
        }

        collisionTest = collisionTest->next;
      }

      Vector2DAdd(&instance->position, &instance->position, &instance->velocity);
      PhysicsTileCollisions(instance);
      
      
      
      //printf("%3f | %3f", instance->position.x, instance->position.y);

      
      //physics code ending here

      instance = instance->next;
      //printf("a");
    }
    //printf("cycledone");
  }
  
}

/*!
\brief Handles collisions with the tilemap
\param _instance physics object to update position of
*/
static void PhysicsTileCollisions(PhysicsObject* _instance)
{
  PhysicsObject* instance = _instance;
  if (IsoTileGet(
    (instance->position.x),
    (instance->position.y + 0.25))
    == 1 || IsoTileGet(
      (instance->position.x),
      (instance->position.y + 0.75))
    == 1)
  {
    //printf("left");
    instance->position.x = (int)(instance->position.x + 0.5);
  }

  //positive x (right):
  if (IsoTileGet(
    (instance->position.x + 1),
    (instance->position.y + 0.25))
    == 1 || IsoTileGet(
      (instance->position.x + 1),
      (instance->position.y + 0.75))
    == 1)
  {
    //printf("right");
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
    //printf("up");
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
    //printf("down");
    instance->position.y = (int)(instance->position.y + 0.5);
  }
}

/*!
\brief returns the square distance between two points (2D vectors)

\param _pos1 point 1
\param _pos2 point 2

Because seriously, passing by pointer just makes things more annoying when the object you're passing is exactly 8 bytes in size anyway.
\return Returns the distance between _pos1 and _pos2 squared.
*/
static float PhysicsDistSQ(Vector2D _pos1, Vector2D _pos2)
{
  //printf("%f", (_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y));
  return (_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y);
}



/*!
\brief removes given physics object

NOTE: pass it &pointer, not pointer

\param _input Address of pointer to physics object to remove

*/
void PhysicsRemoveObject(PhysicsObject** _input)
{
  if ((*_input)->prev)
  {

    if ((*_input)->next)
    {
      (*_input)->next->prev = (*_input)->prev;
      (*_input)->prev->next = (*_input)->next;
    }
    else
    {
      (*_input)->prev->next = NULL;
      PhysicsObjectList.last = (*_input)->prev;
    }

  }
  else
  {

    if ((*_input)->next)
    {
      (*_input)->next->prev = NULL;
      PhysicsObjectList.first = (*_input)->next;
    }
    else
    {
      PhysicsObjectList.first = NULL;
      PhysicsObjectList.last = NULL;
    }
  }
  free(*_input);
  *_input = NULL;
}

/*
LEAVING THIS HERE IN CASE I WANT TO TRY AND GET IT WORKING LATER ON:
DON'T TRY AND READ THIS CODE FOR YOUR OWN GOOD
LEAVING IT HERE IN CASE I DECIDE TO RESURRECT THE OVERLY FANCY CIRCLE COLLISIONS
Vector2D* movementVector = malloc(sizeof(Vector2D));
movementVector->x = instance->velocity.x;
movementVector->y = instance->velocity.y;
if (movementVector->x == 0 && movementVector->y == 0)
{
break;
}
Vector2DNormalize(movementVector, &instance->velocity);

Vector2D* differenceVector = malloc(sizeof(Vector2D));
Vector2DSub(differenceVector, &(collisionTest->position), &(instance->position));
//printf("%f %f a", differenceVector->x, differenceVector->y);

float dotProd = Vector2DDotProduct(movementVector, differenceVector);
printf("dP: %f\n movement: \n x: %f \n y: %f \n diff: \n x: %f \n y: %f\n", dotProd, movementVector->x, movementVector->y, differenceVector->x, differenceVector->y);
if (dotProd <= 0)
{
printf("nm", dotProd);
}
else
{
float diffLength = Vector2DLength(differenceVector);
//down the rabbit hole into "matH I get but don't want to do in my head" territory
float F = (diffLength * diffLength) - (dotProd * dotProd);

if (F >= distBetweenSQ)
{
}
else
{
float T = distBetweenSQ - F;

if (T < 0)
{
}
else
{
printf("loop");
float dist = dotProd - sqrt(T);

float magnitude = Vector2DLength(&instance->velocity);
//printf("%f, %f \n", magnitude, dist);
if (magnitude < dist)
{
}
else
{
printf("Collided \n");
Vector2DNormalize(&instance->velocity, &instance->velocity);
Vector2DScale(&instance->velocity, &instance->velocity, dist);
}
}
}

}*/