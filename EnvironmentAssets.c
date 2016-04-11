//functions to load assets that may end up shared between multiple files/scopes
#include "EnvironmentAssets.h"
#include "PlayerSmoke.h"


static Animation* Asset_particleGate;
static Animation* Asset_particleHitEnemy;
static Animation* Asset_smokeParticle;
static Animation* Asset_particleDoor;

void EnvironmentAssetsInitialize()
{
  Asset_particleGate = GCreateAnimation(1,
    GCreateTexture("animations/world/cloudTemplate.png"),
    GCreateMesh(24.f, 16.f, 1, 1),
    1);

  Asset_particleHitEnemy = GCreateAnimation(1,
    GCreateTexture("animations/world/particle_simpleGreen.png"),
    GCreateMesh(8.f, 8.f, 1, 1),
    1);   

  Asset_particleDoor = GCreateAnimation(1,
    GCreateTexture("animations/world/particle_largeBlue.png"),
    GCreateMesh(16.f, 16.f, 1, 1),
    1);

  Asset_smokeParticle = GCreateAnimation(1,
    GCreateTexture("animations/world/particle_simpleGray.png"),
    GCreateMesh(16.f, 16.f, 1, 1),
    1);

  InitializePlayerSmoke();


}

Animation* GetAsset_Animation(int assetName)
{
  switch (assetName)
  {
  case asset_particleGate:
    return Asset_particleGate;
    break;
  case asset_particleHitEnemy:
  case asset_particleHitPlayer:
    return Asset_particleHitEnemy;
  case asset_smokeParticle:
    return Asset_smokeParticle;
    break;
  case asset_particleDoor:
    return Asset_particleDoor;
    break;
  }
}