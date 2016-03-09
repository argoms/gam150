#pragma once
#include "Graphics.h"
#include "AEEngine.h"
#include "GameObject.h"
#include <stdio.h>
//#include "Entity.h"
//#include "Enemy.h"
#include "PlayerEntity.h"

#define BRIEF_IFRAMES_RECOVERY             10  /* frames for recovery before granting brief invulnerability */
#define BRIEF_IFRAMES                      30  /* i frames granted after taking damage                      */
#define PLAYER_IFRAMES                     30  /* frames for player to be invincible                        */
#define PLAYER_IFRAME_RECOVORY             10  /* cooldown before player can dodge                          */ //was 10
#define SPEED_BONUS_MODIFIER              1.5f /* modify player velocity while invincible                   */
#define DODGE_ALPHA_MODIFIER              0.35f
#define DODGE_RED_MODIFIER                0.35f
#define DODGE_BLUE_MODIFIER               0.35f
#define DODGE_GREEN_MODIFIER              0.35f
#define EPSILON                           0.00001f


void UpdateEntityIFs(GameObject *obj);
void Dodge(int input_key, GameObject *obj);
void ResetColor(GameObject *GameObj);
void BriefInvulnerability(GameObject *GameObj, int EntityType);
void IdleDodge(GameObject *GameObj);
void CheckDrag();


//void PlayerDodge();
//dddddddddddddigufgusgugu