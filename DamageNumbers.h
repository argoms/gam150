/*!
Project (working title): Epoch
\file   DamageNumbers.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Prototypes for text popups when entities take damage.

*/
#pragma once
GameObject* DamageTextCreate(Vector2D position, int damage, int isPlayer);
void DamageTextSimulate(GameObject* instance);