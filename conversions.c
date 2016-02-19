/*****************************************************************************/
/*!
\file   conversions.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    DigiPen login: tarrant.mangasarian
\par    Course: GAM150
\date   2/14/2016
\brief
This file contains the functions for conversions
*/
/*****************************************************************************/
#include "conversions.h"

/*************************************************************************/
/*!
    \par   Function: DoubleToFloat

    \par Description: converts a double to a float
    
    \param d - double to be converted

    \return  A converted float
*/
/*************************************************************************/
float DoubleToFloat(double d)
{
  /* note to self, add checks later */
  
  float f;      /* float for the result       */
  f = (float)d; /* cast the double to a float */
  return f;     /* return the result          */
}

/*************************************************************************/
/*!
    \par   Function: DoubleToInt

    \par Description: converts a double to a int
    
    \param d - double to be converted

    \return A converted float
*/
/*************************************************************************/
int DoubleToInt(double d)
{
  /* note to self, add checks later */
  
  int i;      /* int for the result         */
  i = (int)d; /* cast the double to a int   */
  return i;   /* return the result          */
}

/*************************************************************************/
/*!
    \par   Function: FloatToInt

    \par Description: converts a float to a int
    
    \param - float to be converted

    \return  A converted int
*/
/*************************************************************************/
int FloatToInt(float f)
{
  /* note to self, add checks later */
  
  int i;      /* int for the result         */
  i = (int)f; /* cast the double to a int   */
  return i;   /* return the result          */
}

/*************************************************************************/
/*!
    \par   Function: IntToFloat

    \par Description: converts a int to a float
    
    \param i - int to be converted

    \return  A converted float
*/
/*************************************************************************/
float IntToFloat(int i)
{
  /* note to self, add checks later */
  
  float f;      /* int for the result         */
  f = (float)i; /* cast the double to a int   */
  return f;     /* return the result          */
}