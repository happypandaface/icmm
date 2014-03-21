#ifndef GAME_H_
#define GAME_H_

#include "tile.h"
#include "creature.h"

#define NUM_TILES 2048
#define NUM_CREATURES 2048
#define NUM_ACTIONS 2048

struct ActionElement;
struct ActionElement
{
	struct Action* act;
	struct ActionElement* next;
};

typedef struct
{
	Creature* player;
	Tile tiles[NUM_TILES];
	Creature creatures[NUM_CREATURES];
	struct ActionElement* acts;
}icmmGame;

icmmGame* mainGame;

void game_loop(icmmGame* game, float dt);
void game_init(icmmGame* game);
void game_add_action(icmmGame* game, struct Action* act);

#endif