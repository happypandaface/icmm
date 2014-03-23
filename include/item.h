#ifndef ITEM_H_
#define ITEM_H_

struct Item;
struct ItemElement;

#include "animation.h"
#include "creature.h"

#define ITM_NOTHING 0
#define ITM_HAND 1
#define ITM_MM 2

#define SITM_EXISTS 1 << 0
#define SITM_IN_WORLD 1 << 1// meaning in the level, not in inventory
                            // basically to be drawn

struct ItemElement
{
	struct Item* elem;
	struct ItemElement* next;
};

struct Item
{
	int type;
	long sub_type;
	int texture;
	AnimationInstance* anim;
	Pos2 world_pos;
	// not sure if an item should have a pos (for real world, probs should)
};

void item_create(struct Item* it, int type);
void item_add_sub_type(struct Item* it, long stype);
int item_check_sub_type(struct Item* it, long stype);
void item_draw(struct Item* it, float dt);
void item_draw_world(struct Item* it, float view, float dt);
int item_use(struct Creature* creat, struct Item* item);
void item_add_to_items(struct ItemElement**, struct Item* it);

#endif
