/*!
Project (working title): Epoch
\file   Matrix2D.c  
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

#include "Matrix2D.h"
#include <math.h>

#define PI 3.14159265358979323846f
#define DEG2RAD (PI/180.0f)

/*
This function sets the values of pResult using the values from pMtx
*/
void Matrix2DSet(Matrix2D *pResult, Matrix2D *pMtx)
{
  /* Set the first row of the matrix. */
  pResult->m[0][0] = pMtx->m[0][0];
  pResult->m[0][1] = pMtx->m[0][1];
  pResult->m[0][2] = pMtx->m[0][2];
  /* Set the second row of the matrix. */
  pResult->m[1][0] = pMtx->m[1][0];
  pResult->m[1][1] = pMtx->m[1][1];
  pResult->m[1][2] = pMtx->m[1][2];
  /* Set the third row of the matrix. */
  pResult->m[2][0] = pMtx->m[2][0];
  pResult->m[2][1] = pMtx->m[2][1];
  pResult->m[2][2] = pMtx->m[2][2];
}

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	/* Set the first row of the identity matrix. */
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	/* Set the second row of the identity matrix. */
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = 0;
	/* Set the third row of the identity matrix. */
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	/* Set the first row of the transpose matrix. */
	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[0][2] = pMtx->m[2][0];
	/* Set the second row of the transpose matrix. */
	pResult->m[1][0] = pMtx->m[0][1];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[1][2] = pMtx->m[2][1];
	/* Set the third row of the transpose matrix. */
	pResult->m[2][0] = pMtx->m[0][2];
	pResult->m[2][1] = pMtx->m[1][2];
	pResult->m[2][2] = pMtx->m[2][2];
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{	
	/* Calculate the values for each element of pResult. */
	float a1b1 = (pMtx0->m[0][0] * pMtx1->m[0][0]) + (pMtx0->m[0][1] * pMtx1->m[1][0]) 
		+ (pMtx0->m[0][2] * pMtx1->m[2][0]);
	float a1b2 = (pMtx0->m[0][0] * pMtx1->m[0][1]) + (pMtx0->m[0][1] * pMtx1->m[1][1])
		+ (pMtx0->m[0][2] * pMtx1->m[2][1]);
	float a1b3 = (pMtx0->m[0][0] * pMtx1->m[0][2]) + (pMtx0->m[0][1] * pMtx1->m[1][2])
		+ (pMtx0->m[0][2] * pMtx1->m[2][2]);

	float a2b1 = (pMtx0->m[1][0] * pMtx1->m[0][0]) + (pMtx0->m[1][1] * pMtx1->m[1][0])
		+ (pMtx0->m[1][2] * pMtx1->m[2][0]);
	float a2b2 = (pMtx0->m[1][0] * pMtx1->m[0][1]) + (pMtx0->m[1][1] * pMtx1->m[1][1])
		+ (pMtx0->m[1][2] * pMtx1->m[2][1]);
	float a2b3 = (pMtx0->m[1][0] * pMtx1->m[0][2]) + (pMtx0->m[1][1] * pMtx1->m[1][2])
		+ (pMtx0->m[1][2] * pMtx1->m[2][2]);

	float a3b1 = (pMtx0->m[2][0] * pMtx1->m[0][0]) + (pMtx0->m[2][1] * pMtx1->m[1][0])
		+ (pMtx0->m[2][2] * pMtx1->m[2][0]);
	float a3b2 = (pMtx0->m[2][0] * pMtx1->m[0][1]) + (pMtx0->m[2][1] * pMtx1->m[1][1])
		+ (pMtx0->m[2][2] * pMtx1->m[2][1]);
	float a3b3 = (pMtx0->m[2][0] * pMtx1->m[0][2]) + (pMtx0->m[2][1] * pMtx1->m[1][2])
		+ (pMtx0->m[2][2] * pMtx1->m[2][2]);

	/* Assign values to pResult's matrix. */
	pResult->m[0][0] = a1b1;
	pResult->m[0][1] = a1b2;
	pResult->m[0][2] = a1b3;

	pResult->m[1][0] = a2b1;
	pResult->m[1][1] = a2b2;
	pResult->m[1][2] = a2b3;

	pResult->m[2][0] = a3b1;
	pResult->m[2][1] = a3b2;
	pResult->m[2][2] = a3b3;
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	/* Set the first row of the matrix. */
	pResult->m[0][0] = 1;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = x;
	/* Set the second row of the matrix. */
	pResult->m[1][0] = 0;
	pResult->m[1][1] = 1;
	pResult->m[1][2] = y;
	/* Set the third row of the matrix. */
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	/* Set the first row of the matrix. */
	pResult->m[0][0] = x;
	pResult->m[0][1] = 0;
	pResult->m[0][2] = 0;
	/* Set the second row of the matrix. */
	pResult->m[1][0] = 0;
	pResult->m[1][1] = y;
	pResult->m[1][2] = 0;
	/* Set the third row of the matrix. */
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	/* Convert the angle to radians. */
	Angle *= DEG2RAD;
	/* Calculate the rotation matrix. */
	Matrix2DRotRad(pResult, Angle);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	/* Set the first row of the matrix. */
	pResult->m[0][0] = (float)cos(Angle);
	pResult->m[0][1] = -(float)sin(Angle);
	pResult->m[0][2] = 0;
	/* Set the second row of the matrix. */
	pResult->m[1][0] = (float)sin(Angle);
	pResult->m[1][1] = (float)cos(Angle);
	pResult->m[1][2] = 0;
	/* Set the third row of the matrix. */
	pResult->m[2][0] = 0;
	pResult->m[2][1] = 0;
	pResult->m[2][2] = 1;
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Assumes that pVec is a vector, not a point.
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{	
	/* Calculate the x value of the new vector. */
	float x = (pMtx->m[0][0] * pVec->x) + (pMtx->m[0][1] * pVec->y) + pMtx->m[0][2];
	/* Calculate the y value of the new vector. */
	float y = (pMtx->m[1][0] * pVec->x) + (pMtx->m[1][1] * pVec->y) + pMtx->m[1][2];

	/* Set values of pResult. */
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------
