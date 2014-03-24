#ifndef CREATURE_H_
#define CREATURE_H_

struct Creature;

#include "util.h"
#include "inventory.h"
#include "combat.h"

#define CREATURE_WIDTH 2.0f
#define CREATURE_HEIGHT 2.0f
#define CREATURE_DEPTH 2.0f

// types
#define TYP_PORTAL 1
#define TYP_PLAYER 2
#define TYP_GAS_BALL 3
#define TYP_BLUE_JELLY 4

// subtypes
#define STYP_EXISTS 1 << 0
#define STYP_SOLID 1 << 1 // idk about these 3. I just threw them in
#define STYP_GHOST 1 << 2
#define STYP_LIQUID 1 << 3

struct Creature
{
	int type;
	long sub_type;	// if there's more than 32 subtypes, add sub_type2 and change
					// creature_add_sub_type and the check function
	Pos2 pos;
	struct ItemElement* items;
	float angle;
	AnimationInstance* animInst;
	int texture;
	struct Damage* damage;
};

struct CreatureElement;
struct CreatureElement
{
	struct Creature* elem;
	struct CreatureElement* next;
};

void creature_draw(struct Creature* go, float viewAngle, float dt);
int creature_create(struct Creature* c, int type);
int creature_check_sub_type(struct Creature* go, long stype);
int creature_add_sub_type(struct Creature* go, long stype);

#endif
