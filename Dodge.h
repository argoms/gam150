#pragma once

#include "AEEngine.h"
#include "Entity.h"
#include "Enemy.h"
//#include "PlayerEntity.h"


#define PLAYER_IFRAMES                     16  /* frames for player to be invincible      */
#define PLAYER_IFRAME_RECOVORY             60  /* cooldown before player can dodge        */
#define SPEED_BONUS_MODIFIER              1.5f /* modify player velocity while invincible */

void UpdateEntity(GameObject *obj);
void Dodge(int input_key);
void PlayerDodge();