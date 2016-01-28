#include "Text.h"
#include "AEEngine.h"
#include "Graphics.h"
#include <stdlib.h>
static AEGfxVertexList*	charMesh; /*! mesh of each lettter*/
static AEGfxTexture *charFont; /*! texture containing font sprite*/
static Animation* charAnim; /*! animation of font*/
static int textHeight; /*! height of font*/
static int textWidth; /*! width of font*/

/*!
\brief Initialize text stuff
*/
void TextInit()
{
  charFont = GCreateTexture("dffont.png");
  charMesh = GCreateMesh(16, 24, 16, 16);
  charAnim = GCreateAnimation(16, charFont, charMesh, 16);
  textHeight = 20;
  textWidth = 16;
}

/*!
\brief Creates a text string at given position
\param _string text to be created
\param _x x position of text
\param _y y position of text
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
    Sprite* newChar = GCreateSprite(_x + i * textWidth, _y, charAnim, 9999);\
    TextChar* newTextChar = malloc(sizeof(TextChar));
    newChar->frame = _string[i];
    printf("%c|", _string[i]);
   
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