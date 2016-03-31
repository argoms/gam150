/*!
Project (working title): Epoch
\file   DamageNumbers.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Prototypes for text popups when entities take damage.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
GameObject* DamageTextCreate(Vector2D position, int damage);
void DamageTextSimulate(GameObject* instance);