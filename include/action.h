#ifndef ACTION_H_
#define ACTION_H_

#include "item.h"
#include "creature.h"
#include "util.h"
#include "tile.h"
#include "game.h"

// actions are every thing,
// from telekinetically grabbing an item, to punching a ghost

// action types
#define ACT_NOTHING 0
#define ACT_PUNCH 1
#define ACT_MOVE 2

// action sub types
#define SACT_EXISTS 1 << 0
#define SACT_DISABLED 1 << 1
#define SACT_PHYSICAL 1 << 2

// action roles
#define ACT_ROLE_NOTHING 0
#define ACT_ROLE_USER 1
#define ACT_ROLE_TARGET 2
#define ACT_ROLE_WEAPON 3
#define ACT_ROLE_TOOL 4
#define ACT_ROLE_DIRECTION 5
#define ACT_ROLE_COUNT 6
#define ACT_ROLE_POSITION 7
#define ACT_ROLE_CHAINED 8
#define ACT_ROLE_INSTANT 9
#define ACT_ROLE_TIMER 10
#define ACT_ROLE_ACTIVATE 11
#define ACT_ROLE_MAGNITUDE 12
#define ACT_ROLE_ANGLE 13

// action object values IDS
#define ACT_OBJ_NOTHING 0
#define ACT_OBJ_CREATURE 1
#define ACT_OBJ_ITEM 2
#define ACT_OBJ_FLOAT 3
#define ACT_OBJ_INT 4
#define ACT_OBJ_TILE 5
#define ACT_OBJ_POS 6
#define ACT_OBJ_ACTION 7

struct Action;
// this defines what the action is applied to
struct ActionObject;
struct ActionObject
{
	int role;
	int value_type;
	Creature* creature;
	Item* item;
	float fvalue;
	int ivalue;
	Tile* tile;
	Pos2* pos;
	struct Action* act;
	
	struct ActionObject* next;
};

struct Action
{
	int type;
	long sub_type;
	
	struct ActionObject* actObj;
};

void action_create(struct Action** act, int type);
void action_obj_create(struct ActionObject** actObj, int type);
void action_add_subtype(struct Action* act, long sub_type);
void action_remove_subtype(struct Action* act, long sub_type);
int action_check_subtype(struct Action* act, long sub_type);
// this adds objects to the action and are assigned a role
void action_add(struct Action* act, struct ActionObject* a, int role);
// this is used by action_perform to get the values 
int action_get(struct ActionObject* act, struct ActionObject** rtn, int value_type, int role, int num);
int action_perform(struct Action* act, icmmGame* game, float dt);
// to prevent leaks
void action_destroy(struct Action** act);

#endif