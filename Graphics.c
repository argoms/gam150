#include "Graphics.h"
#include "AEEngine.h"





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
    //EXAMPLE CODE, REMOVE OUT WHEN USING
  {
    //if (sprite)
      //GRemoveSprite(&sprite);
  }
    //EXAMPLE CODE ENDS HERE



  
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  //render sprites in list starting from the first item
  if (spriteList->first)
  {
    Sprite* spriteIndex = spriteList->first;
    while (spriteIndex)
    {
      SimAnimation(spriteIndex);
      AEGfxSetPosition(spriteIndex->x, spriteIndex->y);
      AEGfxTextureSet(spriteIndex->animation->texture,
        spriteIndex->animation->frameOffsetX * (spriteIndex->frame % spriteIndex->animation->frameWidth) - 1,
        spriteIndex->animation->frameOffsetY * (spriteIndex->frame / spriteIndex->animation->frameWidth) - 1);
      
      //printf("%2f mahlen", spriteIndex->animation->frameOffsetY);
      AEGfxSetTransparency(1.0f);
      AEGfxMeshDraw(spriteIndex->animation->mesh, AE_GFX_MDM_TRIANGLES);
      spriteIndex = spriteIndex->lowerSprite;


    }
  }

  
  //render HUD in list starting from the first item
  if (hudLayer->first && 0)
  {
    Sprite* spriteIndex = hudLayer->first;
    while (spriteIndex)
    {
      SimAnimation(spriteIndex);
      AEGfxSetPosition(spriteIndex->x, spriteIndex->y);
      AEGfxTextureSet(spriteIndex->animation->texture, spriteIndex->animation->frameOffsetX * spriteIndex->frame, 0.0f);
      AEGfxSetTransparency(1.0f);
      AEGfxMeshDraw(spriteIndex->animation->mesh, AE_GFX_MDM_TRIANGLES);
      spriteIndex = spriteIndex->lowerSprite;


    }
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
  float frameScaleX = 1/_numFramesX;
  float frameScaleY = 1 / _numFramesY;
  AEGfxVertexList* temp;
  _width *= 0.5;
  _height *= 0.5;

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
 //  temp;
  return temp;
  AE_ASSERT_MESG(temp, "Failed to create mesh!!");
  
}

/*!
\brief Loads a texture

\param _textureName name of texture string
*/
struct AEGfxTexture* GCreateTexture(char* _textureName)
{
  AEGfxTexture* temp;
  
  temp = AEGfxTextureLoad(_textureName);

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
  if (temp)
  {
    while (temp->next)
    {
      tempPrevious = temp;
      //free(tempPrevious);
      //printf("%i||", temp->item->vtxNum);
      AEGfxMeshFree(temp->item);
      //printf("%p))", temp->item);
      temp = temp->next;
      
      
    }
    //printf("%i||", temp->item->vtxNum);
    AEGfxMeshFree(temp->item);
    //printf("%p))", temp->item);
    //free(temp);
  }

  

  if (temp2)
  {
    while (temp2->next)
    {
      tempPrevious2 = temp;
      //free(tempPrevious);
      //printf("%i||", temp->item->vtxNum);
      AEGfxTextureUnload(temp2->item);
      //printf("%p))", temp->item);
      temp2 = temp2->next;
    }
    //printf("%i||", temp->item->vtxNum);
    AEGfxTextureUnload(temp2->item);
    //printf("%p))", temp->item);
    //free(temp);
  }

  //free(meshList);
  free(spriteList);
  free(hudLayer);
  //AEGfxTextureUnload(pTex1);
}


/*!
\brief creates sprite with given parameters
Sprite layer is initialized based on _spriteY param.

\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _texture is a pointer to what texture for the sprite
\param _mesh is a pointer to the vertex list (mesh) for the sprite
\param _frameDelay the number of frames to wait in between changing frames for the animation
*/
Sprite* GCreateSprite(float _spriteX, float _spriteY, Animation* _animation, float _frameDelay)//struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh)
{
 
  Sprite* newSprite = malloc(sizeof(struct Sprite));
  newSprite->x = _spriteX;
  newSprite->y = _spriteY;
  //newSprite->texture = _texture;
  //newSprite->mesh = _mesh;
  newSprite->higherSprite = NULL;
  newSprite->lowerSprite = NULL;
  newSprite->animation = _animation;
  newSprite->frame = 0;
  newSprite->timer = 0;
  newSprite->paused = 0;
  newSprite->frameDelay = _frameDelay;
  newSprite->isHud = 0;


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
\brief creates sprite with given parameters
Sprite is appended to last on the list (more recently created elements are rendered first.

\param _spriteX is the x position of the created sprite
\param _spriteY is the y position of the created sprite
\param _texture is a pointer to what texture for the sprite
\param _mesh is a pointer to the vertex list (mesh) for the sprite
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
  newSprite->frame = 0;
  newSprite->timer = 0;
  newSprite->paused = 0;
  newSprite->frameDelay = _frameDelay;
  newSprite->isHud = 1;


  if (!hudLayer->first) //if first, set first in list
  {
    hudLayer->first = newSprite;
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

\param _input pointer to pointer to sprite
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
    free(*_input); //note: may also free animation? no time to test, will check later
    *_input = NULL;
    
  }
}


/*!
\brief Generates an animation from an image file with specified info.
Does not create texture & mesh objects as part of the process.

\param _numFrames the number of frames in the animation PER ROW
\param _numRows the number of rows in the animaton
\param _texture pointer to the texture to be used
\param _mesh pointer to the mesh to be used
*/
Animation* GCreateAnimation(float _numFrames, struct AEGfxTexture* _texture, struct AEGfxVertexList* _mesh, int _numRows)
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
\brief Call to simulate animation (changing frames)

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
    //printf("%i mahlen", _input->frame);
    _input->frame = 0;
    //printf("a");

  }
}
/*!
\brief sorts sprite layer according to y position
Call this function when a sprite changes y position to make sure it is layered correctly. 
I'm aware that the loop could probably be optimized way better. It kept breaking on me, so I went overkill on safety checks ok? D:

\param _sprite input sprite object
\param _direction y direction of change
*/
void SortSprite(Sprite* _sprite, float _direction)
{
  if (_direction > 0) //assuming that the sprite is going upwards:
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
          
          _sprite->higherSprite = NULL;
          oldHigher->higherSprite = _sprite;
          spriteList->first = _sprite;
          break;
        }
        
      }
      
    }

  }
  else if(_direction < 0) //same process, but going downwards instead of upwards
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