/*!
Project (working title): Epoch
\file   PlayerAnimations.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Stores animation object constructor info for player's animations. Basically a bunch of hard coded variables.
*/
#include "Graphics.h"
#include "AEEngine.h"
#include "EntityAnimation.h"
AnimationSet* PlayerAnimationCreateWalk()
{
  //load animations:
  AEGfxVertexList* walkMesh = GCreateMesh(256.f, 256.f, 12, 1); //mesh for walking (12 frames)
  //AEGfxVertexList* swordMesh = GCreateMesh(256.f, 256.f, 1, 13); //mesh for sword (13 frames)
  //AEGfxVertexList* idleMesh = GCreateMesh(256.f, 256.f, 1, 1); //mesh for idle (1 frames)
  int walkFrames = 12; //number of frames in walk animation
  //int idleFrames = 1; //number of frames in idle animation
  //int swordFrames = 13;

  Animation* walkAnims[16] = {
    GCreateAnimation(walkFrames,
    GCreateTexture("animations/player/Run0.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run1.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run2.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run3.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run4.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run5.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run6.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run7.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run8.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run9.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run10.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run11.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run12.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run13.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run14.png"),
      walkMesh,
      1),
    GCreateAnimation(walkFrames,
      GCreateTexture("animations/player/Run15.png"),
      walkMesh,
      1)
  };
  return CreateAnimationSet(walkAnims);
}

AnimationSet* PlayerAnimationCreateIdle()
{
  AEGfxVertexList* idleMesh = GCreateMesh(256.f, 256.f, 1, 1); //mesh for idle (1 frames)
  int idleFrames = 1; //number of frames in idle animation

  Animation* idleAnims[16] = {
    GCreateAnimation(idleFrames,
    GCreateTexture("animations/player/Idle0.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle1.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle2.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle3.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle4.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle5.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle6.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle7.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle8.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle9.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle10.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle11.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle12.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle13.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle14.png"),
      idleMesh,
      1),
    GCreateAnimation(idleFrames,
      GCreateTexture("animations/player/Idle15.png"),
      idleMesh,
      1)
  };
  return CreateAnimationSet(idleAnims);
}

AnimationSet* PlayerAnimationCreateSword()
{
  //load animations:
  AEGfxVertexList* swordMesh = GCreateMesh(256.f, 256.f, 13, 1); //mesh for sword (13 frames)
  int swordFrames = 13;

  Animation* swordAnims[16] = {
    GCreateAnimation(swordFrames,
    GCreateTexture("animations/player/Sword0.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword1.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword2.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword3.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword4.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword5.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword6.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword7.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword8.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword9.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword10.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword11.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword12.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword13.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword14.png"),
      swordMesh,
      1),
    GCreateAnimation(swordFrames,
      GCreateTexture("animations/player/Sword15.png"),
      swordMesh,
      1)
  };
  return CreateAnimationSet(swordAnims);
}