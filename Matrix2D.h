/*!
Project (working title): Epoch
\file   Matrix2D.h
\author Matt Chasengnou
\par    email: chasengnou.m@digipen.edu
\par    Course: GAM150
\par    Created:  1/19/16
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Provides functions for matrix operations

\ChangeLog
-1/20/16      :  Changes made to Matrix2DTranspose

-NOTE: Don't count code from this. Reused from CS230.
*/

#ifndef MATRIX2D_H
#define MATRIX2D_H


#include "Vector2D.h"

typedef struct Matrix2D
{
	float m[3][3];
}Matrix2D;

/*
This function sets the values of pResult using the values from pMtx
*/
void Matrix2DSet(Matrix2D *pResult, Matrix2D *pMtx);


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult);

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx);

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1);

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y);

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y);

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle);

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle);

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec);


#endif