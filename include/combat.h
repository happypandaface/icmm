#ifndef COMBAT_H_
#define COMBAT_H_

#define DAMAGE_PUNCH 1.0f

// The combat file just holds damage for the
// Creature class. And the function to damage
// Creatures

struct Damage
{
	float health;
};

float combat_damage(struct Damage*, float amnt);

#endif
