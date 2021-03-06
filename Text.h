/*!
Project (working title): Epoch
\file   Text.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Interface for creating/moving/setting text objects (groups of sprites).
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "GameObject.h"
#include <stdlib.h>
#pragma once
typedef struct TextString TextString;
typedef struct TextChar TextChar;
/*!
\struct TextString
\brief Container for groups of text characters (extended sprites)
*/
struct TextString
{
  int length; /**< number of characters in the string*/
  char* string; /**< pointer to string being displayed*/
  TextChar* first; /**< pointer to the first character displayed*/
  float x; /**< x position of string*/
  float y; /**< y position of string*/
};

/*!
\struct TextChar
\brief Visually represents an individual character of text
*/
struct TextChar
{
  char value; /**< ASCII value for character to display*/
  Sprite* sprite; /**< pointer to sprite object displaying the character*/
  TextChar* next; /**< pointer to next character in text string*/
};

void TextInit();
TextString* TextCreateString(char* _string, float _x, float _y);
TextString* TextCreateHUDString(char* _string, float _x, float _y);
void TextStringSetPos(TextString* _textString, float _x, float _y);
void TextRemoveString(TextString* _textString);
void TextStringSet(TextString** _textString, char* _string);
void TextHUDStringSet(TextString** _textString, char* _string);
void TextStringAddOffset(TextString* _textString, Vector2D offset);
void TextStringSetTint(TextString* _textString, Tint newTint);