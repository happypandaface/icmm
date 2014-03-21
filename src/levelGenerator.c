#include "levelGenerator.h"
#include "tile.h"
#include "util.h"

struct Room
{
	Pos2 pos;
	Pos2 dim;
};

void generateLevel(Tile * tiles, int num_tiles, Creature* creatures, int num_creatures)
{
	int curr = 0;
	struct Room r1;
	r1.pos.x = 1;
	r1.pos.y = 2;
	r1.dim.x = 5;
	r1.dim.y = 6;
	curr += addRoom(tiles, r1, curr);
	struct Room r2;
	r2.pos.x = 8;
	r2.pos.y = 3;
	r2.dim.x = 4;
	r2.dim.y = 7;
	curr += addRoom(tiles, r2, curr);
	struct Room path;
	while (curr < num_tiles)
	{
		Tile t;
		t.pos.x = curr;
		t.pos.y = curr;
		tiles[curr] = t;
		++curr;
	}
	
	
	int i = 0;
	while (i < num_creatures)
	{
		creature_create(&(creatures[i]), TYP_GAS_BALL);
		creatures[i].pos.x = 2;
		creatures[i].pos.y = 4;
		if (i == 1)
			creature_add_sub_type(&(creatures[i]), STYP_EXISTS);
		//printf("tile: x: %f y: %f\n", game->creatures[i].pos.x, game->creatures[i].pos.y);
		++i;
	}
};

int addTile(Tile *tiles, float x, float y, int curr)
{
	Tile t;
	t.pos.x = x;
	t.pos.y = y;
	tiles[curr] = t;
	return 1;
};

int addRoom(Tile *tiles, struct Room rm, int curr)
{
	int added = 0;
	int x = rm.pos.x;
	while (x < rm.pos.x+rm.dim.x)
	{
		int y = rm.pos.y;
		while (y < rm.pos.y+rm.dim.y)
		{
			added += addTile(tiles, x, y, curr+added);
			++y;
		}
		++x;
	}
	return added;
};