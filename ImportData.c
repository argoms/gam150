#include <string.h> /* strlen, strcmp, strcpy              */
#include <stdio.h>  /* FILE                                */
#include "Enemy.h"
#include "EnemyAnimations.h"
#include "Physics.h"
#include "Entity.h"
#include "Vector2D.h"

/*
\brief
Populates the game level with one enemy from a text file
REFER TO EXAMPLE TEXT FILE AND COMMENTS IN THIS CODE TO SEE HOW IT WORKS

\param x
x
\param y
y position
\param file
Name of the text file
\param player
A pointer to the player

\returns
GameObject enemy created
*/
GameObject* ImportEnemyData(float x, float y, const char *file, GameObject* player)
{
  GameObject* newEnemy;
  FILE *infile = fopen(file, "r");
  if (infile)
  {
	  while (!feof(infile))
	  {
		int enemyType;    /* Enemy type, refer to enum in GameObject.h | REMEMBER THIS TAKES AN INTEGER NUMBER NOT A STRING */
		float size;       /* Size */

		int health;         /* Duh */
		float chaseSpeed;     /* Chasing speed */
		float patrolSpeed;    /* Default patroling state speed*/
		float detectRange;    /* Detect range */
		float knockbackForce; /* Knockback force when the enemy collides with the player, NOT the attack knockback */

		float attackCooldown;       /* Attack cooldown timer, set this to 0 as default, setting it to something else might end up with weird behavior */
		float attackCooldownLength; /* The actual length of the attack cooldown */
		float attackWindup;         /* Attack windup timer, set this to 0 as default, code will set this to attackWindupLength by default anyways */
		float attackWindupLength;   /* The actual length of the attack windup */
		float attackRange;          /* Attack range of enemy, applies to both melee and range enemies. Set it higher for ranged enemies, obviously */
		int attackDamage;           /* Self explanatory, applies to both melee and projectile enemies*/
		float attackKnockback;      /* Knockback the PLAYER feels when hit by an ENEMY'S attack */
		float enemyProjectileSpeed; /* Projectile speed for ranged enemies, set it to 0 for melee enemies obviously. Setting it to anything but 0 for melee enemies will do nothing anyways */

		Animation* enemyAnimation; /* Forward declaration for the enemy animation */

		fscanf(infile, "ENEMY - %i\n", &enemyType);
		// Handles unchangable enemy information
		// Each enemy will only have one animation set specific to it anyways
		switch (enemyType)
		{
		case 1:
			enemyType = ENEMY_TYPE_MELEE;
			// REPLACE THE BELOW WITH PROPER ENEMY SPRITE/ANIMATION INFORMATION
			enemyAnimation = GCreateAnimation(1,
				GCreateTexture("isotilePlaceholder1.png"),
				GCreateMesh(128.f, 64.f, 1, 1),
				1);
			break;
		case 2:
			enemyType = ENEMY_TYPE_MELEE_BIG;
			break;
		case 3:
			enemyType = ENEMY_TYPE_MELEE_CHARGE;
			break;
		case 4:
			enemyType = ENEMY_TYPE_RANGED;
			// REPLACE THE BELOW WITH PROPER ENEMY SPRITE/ANIMATION INFORMATION
			enemyAnimation = GCreateAnimation(1,
				GCreateTexture("isotilePlaceholder1.png"),
				GCreateMesh(128.f, 64.f, 1, 1),
				1);
			break;
		case 5:
			enemyType = ENEMY_TYPE_RANGED_ARC;
			break;
		case 6:
			enemyType = ENEMY_TYPE_RANGED_HOMING;
			break;
		case 7:
			enemyType = ENEMY_TYPE_HEALER;
			break;
		case 8:
			enemyType = ENEMY_TYPE_SLIME;
			break;
		case 9:
			enemyType = ENEMY_TYPE_SHIELD;
			break;
		}

		fscanf(infile, "  SIZE     - %f\n", &size);
		fscanf(infile, "  HEALTH   - %i\n", &health);

		fscanf(infile, "  CHASE SPEED     - %f\n", &chaseSpeed);
		fscanf(infile, "  PATROL SPEED    - %f\n", &patrolSpeed);
		fscanf(infile, "  DETECT RANGE    - %f\n", &detectRange);
		fscanf(infile, "  KNOCKBACK FORCE - %f\n", &knockbackForce);

		fscanf(infile, "  ENEMY ATTACK COOLDOWN        - %f\n", &attackCooldown);
		fscanf(infile, "  ENEMY ATTACK COOLDOWN LENGTH - %f\n", &attackCooldownLength);
		fscanf(infile, "  ENEMY ATTACK WINDUP          - %f\n", &attackWindup);
		fscanf(infile, "  ENEMY ATTACK WINDUP LENGTH   - %f\n", &attackWindupLength);
		fscanf(infile, "  ENEMY ATTACK RANGE           - %f\n", &attackRange);
		fscanf(infile, "  ENEMY ATTACK DAMAGE          - %f\n", &attackDamage);
		fscanf(infile, "  ENEMY ATTACK KNOCKBACK FORCE - %f\n", &attackKnockback);
		fscanf(infile, "  ENEMY PROJECTILE SPEED       - %f\n", &enemyProjectileSpeed);

		Entity* enemyEntity = malloc(sizeof(Entity));
		enemyEntity->maxHealth = health;

		EntityInit(&enemyEntity);

		// Here the enemy is created using the text data
		// Physics component is initialized and the sprite/animation is initialized here as well
		GameObject* newEnemy = EnemyCreate(PhysicsCreateObject(Vec2(x, y), size), GCreateSprite(0, 40, enemyAnimation, 1), enemyEntity, entity_enemy, enemyType,
			chaseSpeed, detectRange, knockbackForce, attackCooldown, attackCooldownLength,
			attackWindup, attackWindupLength, attackRange, attackKnockback, attackDamage, enemyProjectileSpeed);

		//Now we initialize function pointers and give our enemy a target
		newEnemy->physics->onCollision = &EnemyOnCollision;
		newEnemy->simulate = &EnemySimulate;
		newEnemy->initialize = &EnemyInitialize;
		newEnemy->entity->onEntityKilled = &EnemyOnKilled;
		newEnemy->target = player;
		newEnemy->initialize(newEnemy);

		newEnemy->entity->health = health;

		EnemyAnimationInitialize(newEnemy);
		//newEnemy->physics->position.x = 2;
		//newEnemy->physics->position.y = 2;
		printf("Created an enemy!");

		//CLOSE THE FILE
		fclose(infile);

		return newEnemy;
	}
  }
}