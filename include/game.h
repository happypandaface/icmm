#include "tile.h"
#ifndef GAME_H_
#define GAME_H_

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

struct icmmGame
{
	Creature* player;
	struct TileElement* tiles;
	struct CreatureElement* creatures;
	struct ActionElement* acts;
};

struct icmmGame* mainGame;

void game_loop(struct icmmGame* game, float dt);
void game_init(struct icmmGame* game);
void game_add_action(struct ActionElement** elems, struct Action* act);
void game_add_tile(struct TileElement** elems, struct Tile* tile);
void game_add_creature(struct CreatureElement** elems, Creature* tile);

#endif
