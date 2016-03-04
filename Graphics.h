/*!
\file   Graphics.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Graphics implementation front end handling sprite layering, dynamic sprite creation etc.
*/
#pragma once
#include "Vector2D.h"

typedef struct Animation Animation;
typedef struct Sprite Sprite;
typedef struct SpriteList SpriteList;
typedef struct MeshList MeshList;
typedef struct TextureList TextureList;
typedef struct Tint Tint;

/*!
\struct Tint
\brief Contains all info for a given tint.
*/
struct Tint
{
  float red;
  float green;
  float blue;
  float alpha;
  //AEGfxSetTintColor()
};

/*!
\struct Sprite
\brief Sprite object

Also acts as a linked list between sprites for layering resolution. Sprites are layered with a painter's algorithm (back first), assuming higher Y is further back.
*/
struct Sprite
{
  struct Animation* animation; /**< pointer to the sprite's animation*/
  float x; /**< x position of sprite*/
  float y; /**< y position of sprite*/
  Sprite* lowerSprite; /**< pointer to the next lowest (lower y position) sprite in the environment*/
  Sprite* higherSprite; /**< pointer to the next highest (higher y position) sprite in the environment*/

  int frame; /**< current frame being played*/
  int timer; /**< internal timer for frame delay calculations*/
  int paused; /**< bool, whether or not to play*/

  float frameDelay; /**< how many engine frames to wait before changing animation frame*/

  int isHud; /**< whether or not the sprite is part of the hud, set to 1 if it is, 0 otherwise*/
  Vector2D offset; /**< additional visual offset to individual sprite*/

  float Xscale; /**< width of sprite  */
  float Yscale; /**< height of sprite */

  unsigned int blendMode; /**< blend mode for sprite rendering*/
  struct Tint tint; /**<tint for rendering*/

  void(*specialFX)(void);
};



/*!
\struct SpriteList
\brief info for linked list of sprites
*/
struct SpriteList
{
  Sprite* first; /**< first sprite in list*/
  Sprite* last; /**< last sprite in list*/
};


/*!
\struct Animation
\brief contains information about animations
Note: animation spritesheet should be 1 frame high- all frames on horizontal strip.
*/
struct Animation
{
  struct AEGfxVertexList* mesh; /**< pointer to the mesh for the animation*/
  struct AEGfxTexture* texture; /**< pointer to texture of animation*/


  int length; /**< length of animation in frames*/

  int frameWidth; /**< number of frames per row*/
  int frameHeight; /**< number of frames per column (number of rows)*/
  float frameOffsetX; /**< amount to offset, aka 1/(num frames)*/
  float frameOffsetY; /**< amount to offset y*/
};


//structs for lists of loaded objects:
/*!
\struct MeshList
\brief info for linked list of meshes
*/
struct MeshList
{
  struct AEGfxVertexList* item; /**< data portion of list*/
  MeshList* next; /**< pointer to next object in list*/
};

/*!
\struct TextureList
\brief info for linked list of textures
*/
struct TextureList
{
  struct AEGfxTexture* item; /**< data portion of list*/
  TextureList* next; /**< pointer to next object in list*/
};

void GRender(); //call every frame to render sprites

void GInitialize(); //call before render once to initialize

void GFree(); //call after game/level is over to free resources

Sprite* GCreateSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay); //creates a sprite struct

void GSortSprite(Sprite* sprite, float direction); //call every time the sprite changes y position

Animation* GCreateAnimation(int _numFrames, struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh, int _numRows); //creates an animation

void SimAnimation(Sprite* _input); //call every frame on every sprite to animate

void GRemoveSprite(Sprite** _input); //call to remove a sprite on screen

Sprite* GCreateHudSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay); //call to create a sprite on the hud layer

struct AEGfxVertexList* GCreateMesh(float _width, float _height, float _numFramesX, float _numFramesY); //call to create a mesh

struct AEGfxTexture* GCreateTexture(char* _textureName); //call to create a texture

Tint GTint(float r, float g, float b, float a); //fast constructor for tint object