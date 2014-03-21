#ifndef TILE_H
#define TILE_H

#include "util.h"

#define TILE_WIDTH 2.0f
#define TILE_HEIGHT 2.0f
#define TILE_DEPTH 2.0f

typedef struct
{
	Pos2 pos;
} Tile;

void tile_draw(Tile* tile);

#endif