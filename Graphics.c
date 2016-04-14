/*!
Project (working title): Epoch
\file   Graphics.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Graphics implementation front end handling sprite layering, dynamic sprite creation etc.

On the use of linked lists:
I really really don't trust arrays. Not sure if it's visual studio compiler or a c thing in general,
but arrays have indexed very strangely between release/debug modes for me, even in seemingly clean and
simple code. In particles, for example (not in this file), indexing from 0 -> array length - 1 would not
index through every instance in the array in debug mode, while 0 -> array length would. On the other hand,
doing 0 ->array length in release mode crashes (as expected, since you go 1 index out of bounds), while
0 -> array length - 1 indexes through and applies the effect to all the items in the array. Stuff's weird.

*/
#include "Graphics.h"
#include "AEEngine.h"
#include "conversions.h"
#include "Compass.h"
#include "FancyBackground.h"
#include "ColorFilter.h"

static SpriteList* spriteList; /**< list of sprites to draw*/
static SpriteList* hudLayer; /**< list of hud sprites to draw on top*/
static MeshList* meshList; /**< list of meshes loaded (in wrappers)*/
static TextureList* textureList; /**< list of textures loaded (in wrappers)*/
#define MAX_SPRITE_SIZE 1920 //maximum sprite size assumed for culling

/*!
\brief Initializes graphics management system (setting up lists of objects etc.)
*/
void GInitialize()
{
  //create sprite list
  spriteList = malloc(sizeof(SpriteList));
  spriteList->first = NULL;
  spriteList->last = NULL;

  //create hud layer
  hudLayer = malloc(sizeof(SpriteList));
  hudLayer->first = NULL;
  hudLayer->last = NULL;

}

/*!
\brief Call function every frame to render graphics.
*/
void GRender()
{
  //MATT'S CODE
  Background_Update();
  Background_Draw();
  //

  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  //grab camera position:
  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);

  //render sprites in list starting from the first item
  if (spriteList->first)
  {
    Sprite* spriteIndex = spriteList->first;
    //grab sprite object (linked list plus graphical info)

    int lastBlendMode = AE_GFX_BM_BLEND; //caches the blend mode


    while (spriteIndex)
    {
      if (!spriteIndex->animation)
      {
        spriteIndex = spriteIndex->lowerSprite;
        continue;
      }

      SimAnimation(spriteIndex); //update sprite texture offsets according to animation
      
      //culling:
      float drawPosX = spriteIndex->x + spriteIndex->offset.x;
      float drawPosY = spriteIndex->y + spriteIndex->offset.y;

      //if out of bounds, index to next sprite
      if (drawPosX - MAX_SPRITE_SIZE> AEGfxGetWinMaxX() || drawPosX + MAX_SPRITE_SIZE < AEGfxGetWinMinX() 
        || drawPosY - MAX_SPRITE_SIZE > AEGfxGetWinMaxY() || drawPosY + MAX_SPRITE_SIZE <  AEGfxGetWinMinY())
      {
        spriteIndex = spriteIndex->lowerSprite;
        continue;
      }

      //otherwise, set up sprite drawing:
      AEGfxSetPosition(drawPosX, drawPosY);//set draw position
      
      //set texture based on sprite's animation data (see animation struct for more details)
      AEGfxTextureSet(spriteIndex->animation->texture,
        spriteIndex->animation->frameOffsetX * (spriteIndex->frame % spriteIndex->animation->frameWidth) - 1,
        spriteIndex->animation->frameOffsetY * (spriteIndex->frame / spriteIndex->animation->frameWidth) - 1);

      //adjust blend modes only if needed
      /*if (spriteIndex->blendMode != lastBlendMode)
      {
        AEGfxSetBlendMode(spriteIndex->blendMode);
      }*/
      AEGfxSetBlendMode(spriteIndex->blendMode);
      
      AEGfxSetTintColor(spriteIndex->tint.red, spriteIndex->tint.green, spriteIndex->tint.blue, spriteIndex->tint.alpha);

      //DEPRECATED (jack's code?)
      if (spriteIndex->specialFX != NULL)
      {
        //spriteIndex->specialFX(spriteIndex);
      }
      //

      AEGfxSetTransparency(1.0f);
      //AEGfxSetFullTransformWithZOrder(spriteIndex->x, spriteIndex->y, 1, 0, 1, 1);
      AEGfxMeshDraw(spriteIndex->animation->mesh, AE_GFX_MDM_TRIANGLES);

      //move to next sprite index
      spriteIndex = spriteIndex->lowerSprite;


    }
  }

  /* Draw filter over scene. */
  ColorFilter_Draw();
  
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); //reset blend more to texture in case colorfilter changes it

  //render HUD in list starting from the first item
  if (hudLayer->first)
  {
    //all this stuff acts the same as the previous sprite indexing, but it ignores camera position
    //and draws afterwards so it's layered on top.
    Sprite* spriteIndex = hudLayer->first;
    while (spriteIndex)
    {
      SimAnimation(spriteIndex);
      AEGfxSetPosition(spriteIndex->x + spriteIndex->offset.x + CameraX
                      , spriteIndex->y + spriteIndex->offset.y + CameraY);//set draw position to ignore cam

      AEGfxTextureSet(spriteIndex->animation->texture,
        spriteIndex->animation->frameOffsetX * (spriteIndex->frame % spriteIndex->animation->frameWidth) - 1,
        spriteIndex->animation->frameOffsetY * (spriteIndex->frame / spriteIndex->animation->frameWidth) - 1);
      
      AEGfxSetTintColor(spriteIndex->tint.red, spriteIndex->tint.green, spriteIndex->tint.blue, spriteIndex->tint.alpha);

      AEGfxSetTransparency(1.0f);

      AEGfxSetBlendMode(spriteIndex->blendMode);
      
      AEGfxMeshDraw(spriteIndex->animation->mesh, AE_GFX_MDM_TRIANGLES);
      
      spriteIndex = spriteIndex->lowerSprite;


    }
    //printf("a \n");
  }

  //render compass if applicable (matt's code):
  if (Compass_IsActive)
  {
    Compass_Draw();
  }

}


/*!
\brief Creates a mesh with given width & height, also updates list of loaded meshes for unloading later on.

\param _width width of mesh
\param _height height of mesh
\param numframesX number of frames per row in the animation associated with this mesh
\param numframesX number of rows of frames in the animation associated with this mesh

\return Returns a pointer to the newly created mesh (vertex list)
*/
struct AEGfxVertexList* GCreateMesh(float _width, float _height, float _numFramesX, float _numFramesY)
{
  //calculate the percentage of the total texture that the mesh will occupy:
  float frameScaleX = 1 / _numFramesX;
  float frameScaleY = 1 / _numFramesY;

  AEGfxVertexList* temp;
  _width *= 0.5;
  _height *= 0.5;

  //create square mesh with given parameters:
  AEGfxMeshStart();
  AEGfxTriAdd(
    -_width, -_height, 0x00FF00FF, 0.0f, frameScaleY,
    _width, -_height, 0x00FFFF00, frameScaleX, frameScaleY,
    -_width, _height, 0x00F00FFF, 0.0f, 0.0f);

  AEGfxTriAdd(
    _width, -_height, 0x00FFFFFF, frameScaleX, frameScaleY,
    _width, _height, 0x00FFFFFF, frameScaleX, 0.0f,
    -_width, _height, 0x00FFFFFF, 0.0f, 0.0f);

  temp = AEGfxMeshEnd();

  //update the list of created meshes:
  if (meshList)
  {
    MeshList* index = meshList;
    MeshList* newMesh = malloc(sizeof(meshList));
    newMesh->item = temp;
    newMesh->next = meshList;
    meshList = newMesh;
  }
  else {
    meshList = malloc(sizeof(meshList));
    meshList->item = temp;
    meshList->next = NULL;

  }
  return temp;
  AE_ASSERT_MESG(temp, "Failed to create mesh!!");

}

/*!
\brief Loads a texture from file, also updating the list of textures loaded so they can be unloaded later on.

\param _textureName name of texture string

\return Returns a pointer to the newly loaded texture.
*/
struct AEGfxTexture* GCreateTexture(const char* _textureName)
{
  //load texture according to given name
  AEGfxTexture* temp;
  temp = AEGfxTextureLoad(_textureName);

  //update list of laded textures
  if (textureList)
  {
    TextureList* index = textureList;
    TextureList* newTex = malloc(sizeof(TextureList));
    newTex->item = temp;
    newTex->next = textureList;
    textureList = newTex;
  }
  else {
    textureList = malloc(sizeof(textureList));
    textureList->item = temp;
    textureList->next = NULL;

  }

  return temp;
  AE_ASSERT_MESG(temp, "Failed to create texture!!");

}

/*!
\brief Call this function to unload everything once the gameplay is over.

*/
void GFree()
{
  // Freeing the objects and textures
  MeshList* temp = meshList;
  MeshList* tempPrevious;

  TextureList* temp2 = textureList;
  TextureList* tempPrevious2;

  //freeing meshes (index through linked list, free objects)
  if (temp)
  {
    while (temp->next)
    {
      tempPrevious = temp;
      temp = temp->next;
      AEGfxMeshFree(tempPrevious->item);
      //free(tempPrevious);


    }
    AEGfxMeshFree(temp->item);
  }

  //freeing textures (indexing through another linked list):
  if (temp2)
  {
    while (temp2->next)
    {
      tempPrevious2 = temp2;
      temp2 = temp2->next;
      if (tempPrevious2->item)
      {
        AEGfxTextureUnload(tempPrevious2->item);
      }
    }
    //printf("%i||", temp->item->vtxNum);
    if (temp2->item)
    {
      AEGfxTextureUnload(temp2->item);
    }
    //printf("%p))", temp->item);
    //free(temp);
  }

  //free(meshList);
  free(spriteList);
  free(hudLayer);
  meshList = NULL;
  textureList = NULL;
  //AEGfxTextureUnload(pTex1);
}


/*!
\brief Creates sprite with given parameters- these are the actual objects drawn on screen.
Sprite layer is initialized based on _spriteY param.

\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _animation is the animation object for the new sprite
\param _frameDelay the number of frames to wait in between changing frames for the animation

\return Returns a pointer to the newly created sprite.
*/
Sprite* GCreateSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay)//struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh)
{

  //initialize sprite variables:
  Sprite* newSprite = malloc(sizeof(struct Sprite));

  //initialize sprite values (see sprite struct documentation for descriptions of each value)
  newSprite->x = _spriteX;
  newSprite->y = _spriteY;
  newSprite->higherSprite = NULL;
  newSprite->lowerSprite = NULL;
  newSprite->animation = _animation;
  newSprite->specialFX = NULL;
  newSprite->frame = 0;
  newSprite->timer = 0;
  newSprite->paused = 0;
  newSprite->frameDelay = _frameDelay;
  newSprite->isHud = 0;
  newSprite->offset.x = 0;
  newSprite->offset.y = 0;
  newSprite->blendMode = AE_GFX_BM_BLEND;
  newSprite->tint = GTint(1.0f, 1.0f, 1.0f, 1.0f);
  newSprite->owner = NULL;

  //update sprite render list:
  if (!spriteList->first) //if first, set first in list
  {
    spriteList->first = newSprite;
  }
  else
  {
    //otherwise, sort through the list until the correct y position is found for layering, and insert self there

    Sprite* index = spriteList->first;


    if (newSprite->y > index->y) //if higher than currently highest object, take place as first in list
    {
      spriteList->first = newSprite;
      newSprite->lowerSprite = index;
      index->higherSprite = newSprite;
    }
    else
    {
      //starting from the top, go down the spritelist until the appropriate sprite layer can be found

      while (index->lowerSprite && index->lowerSprite->y > _spriteY)
      {
        index = index->lowerSprite;
      }
      if (index->lowerSprite)
      {
        index->lowerSprite->higherSprite = newSprite;
        newSprite->lowerSprite = index->lowerSprite;
      }
      index->lowerSprite = newSprite;
      newSprite->higherSprite = index;

    }
  }
  return newSprite;
}

/*!
\brief Creates a hud sprite with given parameters, if there was operator overloading I wouldn't have duplicated this :I
Sprite is appended to last on the list (more recently created elements are rendered first every time, unlike regular sprites' y-based layering).

\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _animation is the animation for the new sprite
\param _frameDelay the number of frames to wait in between changing frames for the animation

\return Returns a pointer to the newly created sprite.
*/
Sprite* GCreateHudSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay)//struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh)
{
  //initialize sprite values (see sprite struct documentation for descriptions of each value)
  Sprite* newSprite = malloc(sizeof(struct Sprite));
  newSprite->x = _spriteX;
  newSprite->y = _spriteY;
  newSprite->higherSprite = NULL;
  newSprite->lowerSprite = NULL;
  newSprite->animation = _animation;
  newSprite->specialFX = NULL;
  newSprite->frame = 0;
  newSprite->timer = 0;
  newSprite->paused = 0;
  newSprite->frameDelay = _frameDelay;
  newSprite->isHud = 1;
  newSprite->offset.x = 0;
  newSprite->offset.y = 0;
  newSprite->blendMode = AE_GFX_BM_BLEND;
  newSprite->tint = GTint(1.0f, 1.0f, 1.0f, 1.0f);
  newSprite->owner = NULL;

  //update render list:
  if (!hudLayer->first) //if first, set first in list
  {
    hudLayer->first = newSprite;
    hudLayer->last = newSprite;
  }
  else
  {
    if (hudLayer->last)
    {
      hudLayer->last->lowerSprite = newSprite;
    }
    hudLayer->last = newSprite;

  }
  return newSprite;
}

/*!
\brief Removes sprite object
Pass the address of a pointer to the sprite, not the pointer itself: &sprite instead of sprite assuming that sprite is a pointer to a sprite struct.
If only we could pass by reference in c.

\param _input pointer to pointer to sprite (remember to add the &)
*/
void GRemoveSprite(Sprite** _input)
{
  SpriteList* spriteLayer; //pointer to the layer that the sprite is in

  if (_input)
  {
    switch ((*_input)->isHud) //check what layer the sprite is in, set the layer pointer accordingly
    {

    case 1:
      spriteLayer = hudLayer;
      break;
    default:
    case 0:
      spriteLayer = spriteList;
      break;
    }


    if ((*_input)->higherSprite)
    {

      if ((*_input)->lowerSprite)
      {
        (*_input)->lowerSprite->higherSprite = (*_input)->higherSprite;
        (*_input)->higherSprite->lowerSprite = (*_input)->lowerSprite;
      }
      else
      {
        (*_input)->higherSprite->lowerSprite = NULL;
        spriteLayer->last = (*_input)->higherSprite;
      }

    }
    else
    {

      if ((*_input)->lowerSprite)
      {
        (*_input)->lowerSprite->higherSprite = NULL;
        spriteLayer->first = (*_input)->lowerSprite;
      }
      else
      {
        spriteLayer->first = NULL;
        spriteLayer->last = NULL;
      }
    }
    free(*_input);
    *_input = NULL;

  }
}


/*!
\brief Generates an animation from an image file with specified info. Animations can be shared between multiple sprites 
(and render a different frame within the animation per sprite)
Does not create texture & mesh objects as part of the process.

\param _numFrames the number of frames in the animation PER ROW (aka the number of columns)
\param _numRows the number of rows in the animaton
\param _texture pointer to the texture to be used
\param _mesh pointer to the mesh to be used

\return Returns a pointer to the newly created animation.
*/
Animation* GCreateAnimation(int _numFrames, struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh, int _numRows)
{
  //more information on what each of these values do can be found in the animation struct documentation
  Animation* newAnim = malloc(sizeof(Animation));

  newAnim->length = _numFrames * _numRows;

  newAnim->frameOffsetX = 1 / (float)_numFrames;
  newAnim->frameOffsetY = 1 / (float)_numRows;

  newAnim->frameWidth = _numFrames;
  newAnim->frameHeight = _numRows;

  newAnim->texture = _texture;
  newAnim->mesh = _mesh;
  return newAnim;
}

/*!
\brief Call to simulate animation (updating frame number based on elapsed frames)

\param _input pointer to sprite
*/
void SimAnimation(Sprite* _input)
{
  if (!(_input->paused))
  {
    ++_input->timer; //only increment if not paused
  }
  if (_input->frameDelay <= _input->timer)
  {
    ++_input->frame;
    _input->timer = 0;

  }
  if (_input->frame >= _input->animation->length)
  {
    _input->frame = 0;
  }
}

/*!
\brief sorts sprite layer according to y position
Call this function when a sprite changes y position to make sure it is layered correctly.
I'm aware that the loop could probably be optimized way better. It kept breaking on me, so I went overkill on safety checks ok? D:

\param _sprite input sprite object
\param _direction y direction of change (DEPRECATED)
*/
void GSortSprite(Sprite* _sprite, float _direction)
{
  //DEPRECATED: due to some weird edge cases, checking is now done in both directions no matter what direction the sprite moved in.
  //Function parameters not changed due to legacy code.
  //if (_direction >= 0) //assuming that the sprite is going upwards:
  //DEPRECATION ENDS HERE 
  {

    if (_sprite->higherSprite && spriteList->first != _sprite) //if not at the top...
    {

      Sprite* oldHigher; //old higher sprite to be swapped

      //keep moving self up through list until the highest object has a higher y value than the sprite
      while (_sprite->higherSprite && _sprite->higherSprite->y < _sprite->y)
      {
        
        oldHigher = _sprite->higherSprite;

        if (_sprite->lowerSprite)
        {
          _sprite->lowerSprite->higherSprite = oldHigher;
          oldHigher->lowerSprite = _sprite->lowerSprite;
        }
        else
        {
          //if the sprite was the lowest before, update that fact with the new lowest sprite
          oldHigher->lowerSprite = NULL;
          spriteList->last = oldHigher;
        }

        _sprite->lowerSprite = oldHigher;

        if (oldHigher->higherSprite)
        {
          _sprite->higherSprite = oldHigher->higherSprite;
          oldHigher->higherSprite->lowerSprite = _sprite;
          oldHigher->higherSprite = _sprite;
        }
        else
        {
          //if the replaced sprite was the highest sprite, update the pointer to the first sprite in the list
          _sprite->higherSprite = NULL;
          oldHigher->higherSprite = _sprite;
          spriteList->first = _sprite;
          break;
        }

      }

    }

  }
  //if (_direction <= 0) //same process, but going downwards instead of upwards
  {
    if (_sprite->lowerSprite && spriteList->last != _sprite)
    {
      
      Sprite* oldLower;
      while (_sprite->lowerSprite && _sprite->lowerSprite->y > _sprite->y)
      {

        oldLower = _sprite->lowerSprite;
        if (_sprite->higherSprite)
        {
          _sprite->higherSprite->lowerSprite = oldLower;
          oldLower->higherSprite = _sprite->higherSprite;
        }
        else
        {
          oldLower->higherSprite = NULL;
          spriteList->first = oldLower;
        }

        _sprite->higherSprite = oldLower;

        if (oldLower->lowerSprite)
        {
          _sprite->lowerSprite = oldLower->lowerSprite;
          oldLower->lowerSprite->higherSprite = _sprite;
          oldLower->lowerSprite = _sprite;
        }
        else
        {

          _sprite->lowerSprite = NULL;
          oldLower->lowerSprite = _sprite;
          spriteList->last = _sprite;
          break;
        }


      }

    }
  }

}

/*!
\brief Creates a tint struct. Think of this as a wannabe constructor.
\param r Red value of tint
\param g Green value of tint
\param b Blue value of tint

\return Returns new tint object (by value)
*/
Tint GTint(float r, float g, float b, float a)
{
  Tint newTint;
  newTint.red = r;
  newTint.green = g;
  newTint.blue = b;
  newTint.alpha = a;
  return newTint;
}