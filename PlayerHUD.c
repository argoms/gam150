/*!
Project (working title): Epoch
\file   PlayerHUD.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Manages the player's in-game HUD

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "PlayerHUD.h"
#include "PlayerEntity.h"
#include "GameLevel.h"
#include "Text.h"

static TextString* healthText = NULL;

#define HEART_SCALE 10 //number of health points per heart
void UpdatePlayerHealthHUD()
{
  GameObject* player = GetPlayerObject();
  {
    char hpstring[20] = "Health:            ";
    int tempHP = player->entity->health;
    int count = 0;

    while (tempHP > 0 && count < 10)  // whoever hard coded this needs to stop
    {
      count++;
      hpstring[6 + count] = 3;
      tempHP -= HEART_SCALE;
    }
    if (healthText)
    {
      TextRemoveString(healthText);
    }
    healthText = TextCreateHUDString(hpstring, -300, -200);
  }
}
