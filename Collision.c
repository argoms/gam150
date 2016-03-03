
#include "Collision.h"


/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int PointToCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
  float distance = Vector2DSquareDistance(pP, pCenter);

  //no collision if on rim
  if (distance < (Radius * Radius))
  {
    return 1;
  }
  return 0;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int PointToRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
  float left = (pRect->x) - Width / 2;
  float right = (pRect->x) + Width / 2;
  float bottom = (pRect->y) - Height / 2;
  float top = (pRect->y) + Height / 2;

  if (pPos->x < left)
  {
    return 0;
  }
  if (pPos->x > right)
  {
    return 0;
  }
  if (pPos->y < bottom)
  {
    return 0;
  }
  if (pPos->y > top)
  {
    return 0;
  }

  //exact touches are collisions
  return 1;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int CircleToCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
  float radius_sum_squared = (Radius0 + Radius1) * (Radius0 + Radius1);
  float center_distance = Vector2DSquareDistance(pCenter0, pCenter1);
  if (center_distance <= radius_sum_squared)
  {
    return 1;//collision
  }

  return 0;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int RectToRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
  //#1 is the one we compare to
  //rect1
  float left0 = (pRect0->x) - Width0 / 2;
  float right0 = (pRect0->x) + Width0 / 2;
  float bottom0 = (pRect0->y) - Height0 / 2;
  float top0 = (pRect0->y) + Height0 / 2;

  //rect2
  float left1 = (pRect1->x) - Width1 / 2;
  float right1 = (pRect1->x) + Width1 / 2;
  float bottom1 = (pRect1->y) - Height1 / 2;
  float top1 = (pRect1->y) + Height1 / 2;

  if (left0 > right1)
  {
    return 0;
  }
  if (left1 > right0)
  {
    return 0;
  }
  if (top0 < bottom1)
  {
    return 0;
  }
  if (top1 < bottom0)
  {
    return 0;
  }

  return 1;
}

int CircleToRectangle(Vector2D *pCenter0, float Radius0, Vector2D *pRect0, float Width0, float Height0)
{
  Vector2D p = *pCenter0; //get the center of the circle

  float left0 = (pRect0->x) - Width0 / 2;
  float right0 = (pRect0->x) + Width0 / 2;
  float bottom0 = (pRect0->y) - Height0 / 2;
  float top0 = (pRect0->y) + Height0 / 2;

  if (p.x > right0) //ifpx > right of triangle
  {
    p.x = right0;
  }
  else if (p.x < left0)
  {
    p.x = left0;
  }

  if (p.y > top0)
  {
    p.y = top0;
  }
  else if (p.y < bottom0)
  {
    p.y = bottom0;
  }


  //p is somewhere in the circle's perimeter
  float DistanceSquared;

  DistanceSquared = Vector2DSquareDistance(&p, pCenter0);//get the distance

  if (DistanceSquared <= (Radius0 * Radius0))
  {
    return 1; //collision
  }
  else
  {
    return 0; //no collision
  }
}
