#pragma once
#include "Graphics.h"
#include "AEEngine.h"
#include "GameObject.h"
#include <stdio.h>
//#include "Entity.h"
//#include "Enemy.h"
//#include "PlayerEntity.h"


#define PLAYER_IFRAMES                     30  /* frames for player to be invincible      */
#define PLAYER_IFRAME_RECOVORY             10  /* cooldown before player can dodge        */
#define SPEED_BONUS_MODIFIER              1.5f /* modify player velocity while invincible */
#define DODGE_ALPHA_MODIFIER              0.2f
#define DODGE_RED_MODIFIER                0.2f
#define DODGE_BLUE_MODIFIER               0.2f
#define DODGE_GREEN_MODIFIER              0.2f


void UpdateEntityIFs(GameObject *obj);
void Dodge(int input_key, GameObject *obj);
void ResetColor(GameObject *GameObj);
//void PlayerDodge();