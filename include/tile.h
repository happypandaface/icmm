#ifndef TILE_H
#define TILE_H

#define TILE_WIDTH 2.0f
#define TILE_HEIGHT 2.0f
#define TILE_DEPTH 2.0f

#include "util.h"

#define WALL_NORTH 1 << 0
#define WALL_WEST 1 << 1
#define WALL_SOUTH 1 << 2
#define WALL_EAST 1 << 3

struct Tile
{
	Pos2 pos;
	int walls;
};

struct TileElement;
struct TileElement
{
	struct Tile* elem;
	struct TileElement* next;
};

void create_tile(struct Tile* tile);
void tile_draw(struct Tile* tile);
int tiles_rectify(struct TileElement** tiles, Pos2* in, Pos2* out);
int tile_rectify(struct Tile* tile, Pos2* in, Pos2* out);
void tile_make_walls(struct TileElement* tiles, struct Tile* tile);
void tile_add_wall(struct Tile* tile, int wall);
void tile_remove_wall(struct Tile* tile, int wall);
int tile_check_wall(struct Tile* tile, int wall);

#endif
