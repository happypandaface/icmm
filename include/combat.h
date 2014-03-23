#ifndef COMBAT_H_
#define COMBAT_H_

#define DAMAGE_PUNCH 1.0f

struct Damage
{
	float health;
};

float combat_damage(struct Damage*, float amnt);

#endif
