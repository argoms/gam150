/*!
Project (working title): Epoch
\file   EnvironmentalAssets.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
functions to load assets that may end up shared between multiple files/scopes

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "GameObject.h"
#include "Graphics.h"
#include "Isometric.h"

enum AssetsList
{
  asset_particleGate,
  asset_particleHitEnemy,
  asset_particleHitPlayer,
  asset_smokeParticle,
  asset_particleDoor
};

void EnvironmentAssetsInitialize();
Animation* GetAsset_Animation(int assetName);