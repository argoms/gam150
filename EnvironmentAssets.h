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