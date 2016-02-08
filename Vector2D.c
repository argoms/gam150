//@James
/*-------------------------------------------------------------------------- -
Project Title : CS 230 : Project 2 Part 1
File Name : Vector2D.c
Author : James Do (provided by instructor)
Creation Date : 15 January 2016
Purpose : Contains vector manipulation functions
History
-  15 Jan- Created
-  20 Jan- Finished according to assignment specifications
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/
#include "Vector2D.h"
#include <math.h>

#define EPSILON 0.0001
#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D *pResult)
{
  pResult->x = 0;
  pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
  pResult->x = x;
  pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
  Vector2DScale(pResult, pVec0, -1);
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
  pResult->x = pVec0->x + pVec1->x;
  pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
  pResult->x = pVec0->x - pVec1->x;
  pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
  if (pVec0->x != 0 || pVec0->y != 0)
  {
    float scalar = 1 / (Vector2DLength(pVec0));
    Vector2DScale(pResult, pVec0, scalar);
  }
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
  pResult->x = pVec0->x * c;
  pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
  Vector2DScale(pResult, pVec0, c);
  Vector2DAdd(pResult, pResult, pVec1);
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
  Vector2DScale(pResult, pVec0, c);
  Vector2DSub(pResult, pResult, pVec1);
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
  return sqrtf(Vector2DSquareLength(pVec0));
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
  return pVec0->x * pVec0->x + pVec0->y * pVec0->y;
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
  float diffX = pVec0->x - pVec1->x;
  float diffY = pVec0->y - pVec1->y;
  return sqrtf(diffX * diffX + diffY * diffY);

}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
  float diffX = pVec0->x - pVec1->x;
  float diffY = pVec0->y - pVec1->y;
  return (diffX * diffX + diffY * diffY);
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
  float result = 0;
  result += pVec0->x * pVec1->x;
  result += pVec0->y * pVec1->y;
  return result;
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
  pResult->x = cos(angle);
  pResult->y = sin(angle);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
  float piConversion = 180 / PI; //pi conversion constant
  Vector2DFromAngleDeg(pResult, angle*piConversion);
}

// ---------------------------------------------------------------------------

Vector2D Vec2(float _x, float _y)
{
  Vector2D newObj;
  newObj.x = _x;
  newObj.y = _y;
  return newObj;
}