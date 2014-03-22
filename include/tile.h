#ifndef TILE_H
#define TILE_H

#define TILE_WIDTH 2.0f
#define TILE_HEIGHT 2.0f
#define TILE_DEPTH 2.0f

#include "util.h"

struct Tile
{
	Pos2 pos;
};

struct TileElement;
struct TileElement
{
	struct Tile* elem;
	struct TileElement* next;
};

void tile_draw(struct Tile* tile);
int tiles_rectify(struct TileElement** tiles, Pos2* in, Pos2* out);
int tile_rectify(struct Tile* tile, Pos2* in, Pos2* out);

#endif
