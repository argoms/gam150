/* SHITTY HEADER */
/* Noah made dis */
/* Like a few days ago */
/* The file name is right there */

/* Generic state machine that handles AI states */
/* NOT an object. Simply a file that handles AI generically because no inheritance in C */
/* Probably a better way but fuk it this similar to how I handle animation states anyways */
/* Similar to how our engine works and how the animation state machine works */

/* It's really just a way for me to organize code because otherwise I'd kill myself dealing with this shite */
/* TLDR: All this thing does is call the Start, update, and exit functions of the AI objects */

#pragma once
#include "GameObject.h"
#include "Enemy.h"

void ESMachineRun(GameObject* enemy);

void ESMachineStart(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineExit(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineStateChange(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineMeleeStateChange(GameObject* enemy);

void ESMachineRangedStateChange(GameObject* enemy);

void ESMachineMeleeBigStateChange(GameObject* enemy);