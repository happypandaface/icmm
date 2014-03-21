#ifndef ITEM_H_
#define ITEM_H_

#include "animation.h"

#define ITM_NOTHING 0
#define ITM_HAND 1
#define ITM_MM 2

#define SITM_EXISTS 1 << 0

typedef struct
{
	int type;
	long sub_type;
	int texture;
	AnimationInstance* anim;
	// not sure if an item should have a pos (for real world, probs should)
} Item;

void item_create(Item* it, int type);
void item_add_type(Item* it, long stype);
int item_check_type(Item* it, long stype);
void item_draw(Item* it, float dt);

#endif