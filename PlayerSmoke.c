

static GameObject* smokeEffect;
/*!
\brief Instantiates the player smoke effect (and hides it). Call this on level load.
*/
void InitializePlayerSmoke()
{
  smokeEffect = CreatePlayerSmoke();
 
}

/*!
\brief Set smoke particle system state

\param input Use particle_active and particle_inactive to set this.

*/
void SetSmoke(int input)
{
  EffectSource* smokeComponent = (EffectSource*)(smokeEffect->miscData);

  smokeComponent->state = input;
}

GameObject* CreatePlayerSmoke()
{
  SetParticleAnim(GetAsset_Animation(asset_particleHitEnemy));
  return EffectCreate(Vec2(-10.f, -5.f), Vec2(20, 10), Vec2(0, 0),
    64, 0.02f, Vec2(-4, 8), 0.9f, 0.2f, 32, Vec2(30, 30), 0, GTint(1, 1, 1, 1.f));
}

/*!
\brief Given a world coordinate, translates the particle system to the appropriate screen coordinate.
*/
void UpdateSmokePosition(Vector2D worldPos)
{
  if (!smokeEffect)
  {
    
  }
  EffectSource* smokeComponent = (EffectSource*)(smokeEffect->miscData);
  smokeComponent->position = IsoWorldToScreen(&worldPos);
}