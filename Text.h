#pragma once
typedef struct TextString TextString;
typedef struct TextChar TextChar;
/*!
\struct TextString

*/
struct TextString
{
  int length;
  char* string;
  TextChar* first;
  float x;
  float y;
};

struct TextChar
{
  char value;
  TextChar* next;
};

void TextInit();
TextString* TextCreateString(char* _string, float _x, float _y);