#include "combat.h"

float combat_damage(struct Damage* dam, float amnt)
{
	dam->health -= amnt;
	return dam->health;
}
