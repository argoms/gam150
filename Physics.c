/*!
\file   Physics.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic physics/collision implementation.

Changelog:
2/26/16   Matt - Modified tilecollision checking functions.
2/29/16   Matt - Finished tilecollision fixes
                 Added circle to rect collision function
                 Added coordinate snapping function
*/
#include "Physics.h"
#include "Isometric.h"
#include "Vector2D.h"
#include "GameObject.h"
#include <math.h>
#include <stdlib.h>
#include "conversions.h"

/* Flags for collisions */
enum Collisions
{
  COLLISION_RIGHT   = 1,
  COLLISION_LEFT    = 1 << 1,
  COLLISION_TOP     = 1 << 2,
  COLLISION_BOTTOM  = 1 << 3
};

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
  newObject->lastValidPosition = _position;
  newObject->size = _size;
  newObject->velocity = zeroVec;
  newObject->next = NULL;
  newObject->prev = NULL;
  newObject->active = 1;
  newObject->onCollision = NULL;
  newObject->angle = 0.0f;
  newObject->insideTile = 0;

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

  return newObject;
  
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
      GSortSprite(instance->owner->sprite, 0);
      
      
      //printf("%3f | %3f", instance->position.x, instance->position.y);

      
      //physics code ending here

      instance = instance->next;
      //printf("a");
    }
    //printf("cycledone");
  }
  
}

/*!
\brief  Snaps a coordinate to a cell
\param  coordinate is the coordinate to snap
        side is where to snap relative to
          use Collision flags:
          COLLISION_LEFT
          COLLISION_RIGHT
          COLLISION_TOP
          COLLISION_BOTTOM
*/
void SnaptoCell(float *coordinate, unsigned side)
{
  /* If snapping from bottom or left, snap the value by finding its integral part and adding 1 */
  if (side == COLLISION_BOTTOM || side == COLLISION_LEFT)
    *coordinate = (int)(*coordinate) + 1.f;

  /* Else, if snapping from right or top, snap the value by finding its integral part */
  else
    *coordinate = (int)(*coordinate);
}

/*!
\brief Tests collision between a circle and a rectangle
\param pCircle  center of the circle
       radius   radius of the circle
       pRect    center of rectangle
       width    width of rectangle
       height   height of rectangle
*/
static int CircleToRectCollTest(Vector2D * pCircle, float radius, Vector2D *pRect, float width, float height)
{
  /* Create a point to test from the circle's center */
  Vector2D testpoint = { pCircle->x, pCircle->y };

  // if the point is higher than the rectangle's top, then set its height to the rectangle's top position
  if (testpoint.y > pRect->y + height / 2.f)
    testpoint.y = pRect->y + height / 2.f;

  // if the point is lower than the rectangle's bottom, then set its height to the bottom position
  else if (testpoint.y < pRect->y - height / 2.f)
    testpoint.y = pRect->y - height / 2.f;

  // if the point is further left than the rectangle's left corners, set its x position to the left position
  if (testpoint.x < pRect->x - width / 2.f)
    testpoint.x = pRect->x - width / 2.f;

  // if the point is further right than the rectangle's right corners, then set its x position to the right position
  else if (testpoint.x > pRect->x + width / 2.f)
    testpoint.x = pRect->x + width / 2.f;

  // Test for collision
  if (Vector2DSquareDistance(pCircle , &testpoint) < radius * radius)
    return 1;

  return 0;
}

/*!
\brief Handles collisions with the tilemap
\param _instance physics object to update position of
*/
static void PhysicsTileCollisions(PhysicsObject* _instance)
{
  //PhysicsObject* instance = _instance;

  unsigned int collisionFields = 0; /* Flag for collisions */

  /*
  Get the object's collider information.
  Some conversions had to be made because objects are not centered on origin.
  */

  float 
    radius = _instance->size / 2.f,               /* The radius of the object */
    centerX = _instance->position.x + radius,     /* X coordinate of center */
    centerY = _instance->position.y + radius;     /* Y coordinate of center */
  int
    left = FloatToInt(_instance->position.x),     /* Left side of object in map units */
    right = FloatToInt(centerX + radius),         /* Right side of object in map units */
    top = FloatToInt(centerY + radius),           /* Top of object in map units */
    bottom = FloatToInt(_instance->position.y);   /* Bottom of object in map units */

  Vector2D colliderPos = Vec2(centerX, centerY);  /* Position of the object's collider, converted for collisions */

  /* Update collision flags */

  /* 
    First, do simple binary checks on the object hotspots.
    Then, if binary check indicates a collision tile, do a more detailed collision check
    using circle to rect test.
  */

  /* Check top-top-left */
  if (IsoTileGet(centerX - radius / 2.f, top) == 1)
  {
    Vector2D tilePos = { (FloatToInt)(centerX - radius / 2.f) + 0.5f, top + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_TOP;
  }

  /* Check top-top-right */
  if (IsoTileGet(centerX + radius / 2.f, top) == 1)
  {
    Vector2D tilePos = { (FloatToInt)(centerX + radius / 2.f) + 0.5f, top + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_TOP;
  }

  /* Check left-top-left */
  if (IsoTileGet(left, centerY + radius / 2.f) == 1)
  {
    Vector2D tilePos = { left + 0.5f, (FloatToInt)(centerY + radius / 2.f) + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_LEFT;
  }

  /* Check left-bottom-left */
  if (IsoTileGet(left, centerY - radius / 2.f) == 1)
  {
    Vector2D tilePos = { left + 0.5f, (FloatToInt)(centerY - radius / 2.f) + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_LEFT;
  }

  /* Check right-top-right */
  if (IsoTileGet(right, centerY + radius / 2.f) == 1)
  {
    Vector2D tilePos = { right + 0.5f, (FloatToInt)(centerY + radius / 2.f) + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_RIGHT;
  }

  /* Check right-bottom-right */
  if (IsoTileGet(right, centerY - radius / 2.f) == 1)
  {
    Vector2D tilePos = { right + 0.5f, (FloatToInt)(centerY - radius / 2.f) + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_RIGHT;
  }

  /* Check bottom-bottom-left */
  if (IsoTileGet(centerX - radius / 2.f, bottom) == 1)
  {
    Vector2D tilePos = { (FloatToInt)(centerX - radius / 2.f) + 0.5f, bottom + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_BOTTOM;
  }

  /* Check bottom-bottom-right */
  if (IsoTileGet(centerX + radius / 2.f, bottom) == 1)
  {
    Vector2D tilePos = { (FloatToInt)(centerX + radius / 2.f) + 0.5f, bottom + 0.5f };

    if (CircleToRectCollTest(&colliderPos, radius, &tilePos, 1, 1))
      collisionFields |= COLLISION_BOTTOM;
  }

  /* Check the collision flags and act based on them */

  /* If there is no collision, update the last known position to current and return */
  if (!collisionFields)
  {
    _instance->lastValidPosition = Vec2(_instance->position.x, _instance->position.y);
    return;
  }

  /* If colliding on leftt, snap position */
  if (collisionFields & COLLISION_LEFT)
  {
    /* Snap the x position from the left */
    SnaptoCell(&_instance->position.x, COLLISION_LEFT); 
  }

  /* If colliding on right, snap position */
  else if (collisionFields & COLLISION_RIGHT)
  {
    /* Snap the x position from the right */
    SnaptoCell(&_instance->position.x, COLLISION_RIGHT);
  }

  /* If colliding on top, snap position */
  if (collisionFields & COLLISION_TOP)
  {
    /* Snap the y position from the top */
    SnaptoCell(&_instance->position.y, COLLISION_TOP);
  }

  /* If colliding on bottom, snap position */
  else if (collisionFields & COLLISION_BOTTOM)
  {
    /* Snap the y position from the bottom */
    SnaptoCell(&_instance->position.y, COLLISION_BOTTOM); 
  }

  /* 
    If the object has moved too far from its last known position due to snapping,
    return it to the last known position.
  */
  if (Vector2DSquareDistance(&_instance->position, &_instance->lastValidPosition) > 0.1f)
    _instance->position = Vec2(_instance->lastValidPosition.x, _instance->lastValidPosition.y);

  /* Else, the snapping was successful, so update the last known position to current */
  else
    _instance->lastValidPosition = Vec2(_instance->position.x, _instance->position.y);
  
  // Deprecated code. Noah's code
  ////Collision test for left
  //if (IsoTileGet(
  //  FloatToInt(instance->position.x), 
  //  FloatToInt(instance->position.y + 0.25f)) == 1
  //  ||
  //  IsoTileGet(FloatToInt(instance->position.x), 
  //    FloatToInt(instance->position.y + 0.75f)) == 1)
  //{
  //  //printf("left");
  //  instance->position.x = IntToFloat(FloatToInt(instance->position.x + 0.5f));

  //  // This chunk handles if the player would be inside the tilemap
  //  // Basically more precise adjustments, same for all four directions
  //  PhysicsIsInsideTile(instance);
	 // if (instance->insideTile)
	 // {
  //    instance->position.x = IntToFloat(FloatToInt(instance->position.x + 0.5f));
  //    instance->insideTile = 0;
  //    printf("bounce FROM left");
	 // }
  //}

  ////positive x (right):
  //if (IsoTileGet(
  //  FloatToInt(instance->position.x + 1),
  //  FloatToInt(instance->position.y + 0.25f))
  //  == 1 || IsoTileGet(
  //    FloatToInt(instance->position.x + 1),
  //    FloatToInt(instance->position.y + 0.75f))
  //  == 1)
  //{
  //  //printf("right");
  //  instance->position.x = IntToFloat(FloatToInt(instance->position.x + 0.5f));

  //  PhysicsIsInsideTile(instance);
	 // if (instance->insideTile)
	 // {
  //    instance->position.x = IntToFloat(FloatToInt(instance->position.x + -0.5f));
  //    instance->insideTile = 0;
  //    printf("bounce FROM right");
	 // }
  //}

  ////positive y (up):
  //if (IsoTileGet(
  //  FloatToInt(instance->position.x + 0.25f),
  //  FloatToInt(instance->position.y + 1))
  //  == 1 || IsoTileGet(
  //    FloatToInt(instance->position.x + 0.75f),
  //    FloatToInt(instance->position.y + 1))
  //  == 1)
  //{
  //  //printf("up");
  //  instance->position.y = IntToFloat(FloatToInt(instance->position.y + 0.5f));

  //  PhysicsIsInsideTile(instance);
	 // if (instance->insideTile)
	 // {
  //    instance->position.y = instance->position.y = IntToFloat(FloatToInt(instance->position.y + -1.0f));
  //    instance->insideTile = 0;
  //    printf("bounce FROM up");
	 // }
  //}

  ////negative y (down)
  //if (IsoTileGet(FloatToInt(instance->position.x + 0.25f), FloatToInt(instance->position.y))== 1 
  //  || IsoTileGet(FloatToInt(instance->position.x + 0.75f), FloatToInt(instance->position.y)) == 1)
  //{
  //  //printf("down");
  //  instance->position.y = IntToFloat(FloatToInt(instance->position.y + 0.5f));

  //  PhysicsIsInsideTile(instance);
	 // if (instance->insideTile)
	 // {
  //    instance->position.y = IntToFloat(FloatToInt(instance->position.y + 1.0f));
  //    instance->insideTile = 0;
  //    printf("bounce FROM down");
	 // }
  //}
}

/*
\brief sets a simple flag is the instance would be inside a tile

\param the instance's physics component
*/
static void PhysicsIsInsideTile(PhysicsObject* _instance)
{
  if (IsoTileGet(FloatToInt(_instance->position.x), FloatToInt(_instance->position.y)) == 1
    || IsoTileGet(FloatToInt(_instance->position.x), FloatToInt(_instance->position.y)) == 1)  /* redundant? */
  {
    _instance->insideTile = 1;
  }
  else
    _instance->insideTile = 0;
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

/**************************************************************************************************
Function      : PhysicsSetVelocity
Description   : Sets the velocity of the physics component.
Input         : _instance is the physics object,
                x is the x velocity,
                y is the y velocity.
Output        : No output.
**************************************************************************************************/
void PhysicsSetVelocity(PhysicsObject* _instance, float x, float y)
{
  _instance->velocity.x = x;
  _instance->velocity.y = y;
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