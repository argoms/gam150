/*!
Project (working title): Epoch
\file   conversions.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
This file contains the functions for conversions between variable types.

note by james: semi-deprecated
*/
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
  i = (int)f; /* cast the float to a int    */
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
  
  float f;      /* float for the result        */
  f = (float)i; /* cast the int to a int       */
  return f;     /* return the result           */
}

/*************************************************************************/
/*! 
\par DEPRECATED   Function: LongIntToFloat

\par Description: converts a long int to a float

\param li - long int to be converted

\return  A converted float
*/
/*************************************************************************/
//float LongIntToFloat(long int li)
//{
//  float f;      /* float for the result            */
//  f = (float)li;/* cast the long int to a float    */
//  return f;     /* return the result               */
//}

/*************************************************************************/
/*!
\par  DEPRECATED Function: LongFloatToInt

\par Description: converts a long float to a int

\param lf - long float to be converted

\return  A converted int
*/
/*************************************************************************/
//int LongFloatToInt(long float lf)
//{
//  int i;      /* int for the result            */
//  i = (int)lf;/* cast the long float to a int  */
//  return i;   /* return the result             */
//}

/*************************************************************************/
/*!
\par   Function: UnsignedLongToFloat

\par Description: converts a unsignedvlong int to a float

\param uli - unsigned long int to be converted

\return  A converted float
*/
/*************************************************************************/
float UnsignedLongToFloat(unsigned long int uli)
{
  float f;       /* float for the result                  */
  f = (float)uli;/* cast the unsigned long int to a float */
  return f;      /* return the result                     */
}