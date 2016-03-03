#pragma once

#include "Vector2D.h"


int PointToCircle(Vector2D *pP, Vector2D *pCenter, float Radius);
int PointToRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height);
int CircleToCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1);
int RectToRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1);
int CircleToRectangle(Vector2D *pCenter0, float Radius0, Vector2D *pRect0, float Width0, float Height0);
