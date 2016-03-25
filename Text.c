/*!
\file   Text.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions for creating/moving/setting text objects (groups of sprites).
*/
#include "Graphics.h"
#include "Text.h"
#include "AEEngine.h"

#include <stdlib.h>
static AEGfxVertexList*	charMesh; /**< mesh of each lettter*/
static AEGfxTexture *charFont; /**< texture containing font sprite*/
static Animation* charAnim; /**< animation of font*/
static int textHeight; /**< height of font*/
static int textWidth; /**< width of font*/

/*!
\brief Initialize text stuff
*/
void TextInit()
{
  charFont = GCreateTexture("animations/dffont.png");
  charMesh = GCreateMesh(32, 48, 16, 16);
  charAnim = GCreateAnimation(16, charFont, charMesh, 16);
  textHeight = 48;
  textWidth = 32;
}

/*!
\brief Creates a text string at given position
\param _string text to be created
\param _x x position of text
\param _y y position of text
\return Returns a pointer to the new text string.
*/
TextString* TextCreateString(char* _string, float _x, float _y)
{
  int i = 0; /*! index var*/
  TextString* result = malloc(sizeof(TextString));
  TextChar* latestChar = NULL;
  //result->length = strlen(_string);
  //
  while (_string[i])
  {
    //create new letter & sprite
    Sprite* newChar = GCreateSprite(_x + i * textWidth, _y, charAnim, 1);
    TextChar* newTextChar = malloc(sizeof(TextChar));
    newTextChar->sprite = newChar;
    newChar->frame = _string[i];
    newChar->paused = 1;

    //printf("%c|", _string[i]);
   
    //update the character linked lists:
    newTextChar->value = _string[i];
    newTextChar->next = NULL;

    if (latestChar)
    {
      latestChar->next = newTextChar;
      latestChar = newTextChar;
    }
    else {
      result->first = newTextChar;
      latestChar = newTextChar;
    }
    i++;
  }

  return result;
}

/*!
\brief Creates a text string at given position on the hud layer
\param _string text to be created
\param _x x position of text
\param _y y position of text
\return Returns a pointer to the new text string.
*/
TextString* TextCreateHUDString(char* _string, float _x, float _y)
{
  int i = 0; /*! index var*/
  TextString* result = malloc(sizeof(TextString));
  TextChar* latestChar = NULL;
  //result->length = strlen(_string);
  //
  while (_string[i])
  {
    
    //create new letter & sprite
    Sprite* newChar = GCreateHudSprite(_x + i * textWidth, _y, charAnim, 1);
    TextChar* newTextChar = malloc(sizeof(TextChar));
    newTextChar->sprite = newChar;
    newChar->frame = _string[i];
    newChar->paused = 1;

    //printf("%c|", _string[i]);

    //update the character linked lists:
    newTextChar->value = _string[i];
    newTextChar->next = NULL;

    if (latestChar)
    {
      latestChar->next = newTextChar;
      latestChar = newTextChar;
    }
    else {
      result->first = newTextChar;
      latestChar = newTextChar;
    }
    i++;
  }

  return result;
}

/*!
\brief moves a given text string to a given position

\param _textString TextString object to move
\param _x x position to move it to
\param _y y position to move it to
*/
void TextStringSetPos(TextString* _textString, float _x, float _y)
{
  TextChar* textChar = _textString->first;
  int i = 0;
  _textString->x = _x;
  _textString->y = _y;
  
  while (textChar)
  {
    textChar->sprite->x = _x + i * textWidth;
    textChar->sprite->y = _y;
    i++;
    textChar = textChar->next;
  }
}

/*!
\brief removes a text string

\param _textString text string to remove
*/
void TextRemoveString(TextString* _textString)
{
  TextChar* textChar = _textString->first;
  TextChar* tempPrevious = textChar;
  while (textChar)
  {
    GRemoveSprite(&textChar->sprite);
    tempPrevious = textChar;
    textChar = textChar->next;
    free(tempPrevious);
  }
  free(_textString);
}

/*!
\brief sets text string to given string, currently broken? Stopped working some point after testing


\param _textString address of the text string (type &textPointer, not textPointer)
\param _string string to overwrite
*/
void TextStringSet(TextString** _textString, char* _string)
{
  float tempX = (*_textString)->x;
  float tempY = (*_textString)->y;
  TextRemoveString(*_textString);
  //free(*_textString);
  *_textString = TextCreateString(_string, tempX, tempY);
}

/*!
\brief sets text HUD string to given string

\param _textString address of the text string (type &textPointer, not textPointer)
\param _string string to overwrite
*/
void TextHUDStringSet(TextString** _textString, char* _string)
{
  float tempX = (*_textString)->x;
  float tempY = (*_textString)->y;
  TextRemoveString(*_textString);
  //free(*_textString);
  *_textString = TextCreateHUDString(_string, tempX, tempY);
  printf("hudset");
}

void TextStringAddOffset(TextString* _textString, Vector2D offset)
{
  TextChar* textChar = _textString->first;
  int i = 0;
 // _textString->x = _x;
 // _textString->y = _y;

  while (textChar)
  {
    textChar->sprite->offset.x += offset.x;
    textChar->sprite->offset.y += offset.y;
    i++;
    textChar = textChar->next;
  }
}