/*!
Project (working title): Epoch
\file   Graphics.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Graphics implementation front end handling sprite layering, dynamic sprite creation etc.

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "conversions.h"
#include "Compass.h"
#include "FancyBackground.h"

static SpriteList* spriteList; //list of sprites for game layer
static SpriteList* hudLayer; //list of sprites for hud layer
static MeshList* meshList;
static TextureList* textureList;

/*!
\brief Initializes graphics stuff.
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
  Background_Update();
  Background_Draw();

  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);
  //render sprites in list starting from the first item
  if (spriteList->first)
  {
    Sprite* spriteIndex = spriteList->first;
    while (spriteIndex)
    {
      AEGfxSetPosition(spriteIndex->x + spriteIndex->offset.x, spriteIndex->y + spriteIndex->offset.y);//set draw position
                                                       
      SimAnimation(spriteIndex); //update sprite texture offsets according to animation
      
      AEGfxTextureSet(spriteIndex->animation->texture,
        spriteIndex->animation->frameOffsetX * (spriteIndex->frame % spriteIndex->animation->frameWidth) - 1,
        spriteIndex->animation->frameOffsetY * (spriteIndex->frame / spriteIndex->animation->frameWidth) - 1);

      AEGfxSetBlendMode(spriteIndex->blendMode);
      
      AEGfxSetTintColor(spriteIndex->tint.red, spriteIndex->tint.green, spriteIndex->tint.blue, spriteIndex->tint.alpha);
      if (spriteIndex->specialFX != NULL)
      {
        spriteIndex->specialFX(spriteIndex);
      }

      AEGfxSetTransparency(1.0f);
      //AEGfxSetFullTransformWithZOrder(spriteIndex->x, spriteIndex->y, 1, 0, 1, 1);
      AEGfxMeshDraw(spriteIndex->animation->mesh, AE_GFX_MDM_TRIANGLES);

      //move to next sprite index
      spriteIndex = spriteIndex->lowerSprite;


    }
  }



  

  //render HUD in list starting from the first item
  if (hudLayer->first)
  {
    
    Sprite* spriteIndex = hudLayer->first;
    while (spriteIndex)
    {
      //printf("%p |", spriteIndex);
      SimAnimation(spriteIndex);
      AEGfxSetPosition(spriteIndex->x + spriteIndex->offset.x + CameraX
                      , spriteIndex->y + spriteIndex->offset.y + CameraY);//set draw position

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

  if (Compass_IsActive)
  {
    Compass_Draw();
  }

}


/*!
\brief Creates a mesh with given width & height

\param _width width of mesh
\param _height height of mesh
\param numframesX number of frames per row in the animation associated with this mesh
\param numframesX number of rows of frames in the animation associated with this mesh
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
\brief Loads a texture from file

\param _textureName name of texture string
*/
struct AEGfxTexture* GCreateTexture(char* _textureName)
{
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

Might be a mem leak with not freeing texture/meshlist objects, but can't actually free them for some reason
*/
void GFree()
{
  // Freeing the objects and textures
  MeshList* temp = meshList;
  MeshList* tempPrevious;

  TextureList* temp2 = textureList;
  TextureList* tempPrevious2;
  if (temp)
  {
    while (temp->next)
    {
      tempPrevious = temp;
      //free(tempPrevious);
      //printf("%i||", temp->item->vtxNum);

      //printf("%p))", temp->item);
      temp = temp->next;

      //printf("AAAAAAA %i", temp->item->vtxNum);
      AEGfxMeshFree(tempPrevious->item);
      //free(tempPrevious);


    }
    //printf("%i||", temp->item->vtxNum);
    //printf("BBBB");
    AEGfxMeshFree(temp->item);
    //printf("%p))", temp->item);
    //free(temp);
  }



  if (temp2)
  {
    while (temp2->next)
    {
      tempPrevious2 = temp2;
      //free(tempPrevious);
      //printf("%i||", temp->item->vtxNum);

      //printf("%p))", temp->item);
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
\brief creates sprite with given parameters
Sprite layer is initialized based on _spriteY param.

\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _animation is the animation object for the new sprite
\param _frameDelay the number of frames to wait in between changing frames for the animation
*/
Sprite* GCreateSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay)//struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh)
{

  //initialize sprite variables:
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
  newSprite->isHud = 0;
  newSprite->offset.x = 0;
  newSprite->offset.y = 0;
  newSprite->blendMode = AE_GFX_BM_BLEND;
  newSprite->tint = GTint(1.0f, 1.0f, 1.0f, 1.0f);
  newSprite->owner = NULL;

  //newSprite->animation->mesh
  //update sprite list:
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
\brief creates a hud sprite with given parameters
Sprite is appended to last on the list (more recently created elements are rendered first.
+
\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _animation is the animation for the new sprite
\param _frameDelay the number of frames to wait in between changing frames for the animation
*/
Sprite* GCreateHudSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay)//struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh)
{
  Sprite* newSprite = malloc(sizeof(struct Sprite));
  newSprite->x = _spriteX;
  newSprite->y = _spriteY;
  //newSprite->texture = _texture;
  //newSprite->mesh = _mesh;
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
\brief Generates an animation from an image file with specified info.
Does not create texture & mesh objects as part of the process.

\param _numFrames the number of frames in the animation PER ROW (aka the number of columns)
\param _numRows the number of rows in the animaton
\param _texture pointer to the texture to be used
\param _mesh pointer to the mesh to be used
*/
Animation* GCreateAnimation(int _numFrames, struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh, int _numRows)
{
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
\brief Call to simulate animation (updating frame number)

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
\param _direction y direction of change
*/
void GSortSprite(Sprite* _sprite, float _direction)
{
  //if (_direction >= 0) //assuming that the sprite is going upwards:
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
\brief Creates a tint struct.
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