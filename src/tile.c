#include "tile.h"

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

void tile_draw(struct Tile* tile)
{
	glTexCoord2f (0, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2);
	glTexCoord2f (1, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	glTexCoord2f (1, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	glTexCoord2f (0, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2);
}

int tiles_rectify(struct TileElement** tiles, Pos2* in, Pos2* out)
{
	Pos2 dim;
	Pos2 totDiff;
	dim.x = 1;
	dim.y = 1;
	float lowest = -1.0f;
	struct TileElement* currTileElem = *tiles;
	while (currTileElem != NULL)
	{
		Pos2 diff;
		pos_sub(*in, currTileElem->elem->pos, &diff);
		if (diff.x > 0.0f)
			if (diff.x <= 1.0f)
				diff.x = 0.0f;
			else
				diff.x -= 1.0f;
		if (diff.y > 0.0f)
			if (diff.y <= 1.0f)
				diff.y = 0.0f;
			else
				diff.y -= 1.0f;
		float len = pos_len(diff);
		if (len == 0.0f)
		{
			totDiff.x = diff.x;
			totDiff.y = diff.y;
			break;
		}
		if (	lowest == -1.0f ||
			len < lowest)
		{
			lowest = len;
			totDiff.x = diff.x;
			totDiff.y = diff.y;
		}
		currTileElem = currTileElem->next;
	}
	//fprintf(stderr, "tiles\n");
	//if (lowest == 0.0f)
	//	fprintf(stderr, "ontiles\n");
	out->x = in->x - totDiff.x;
	out->y = in->y - totDiff.y;
	return 0;
}
int tile_rectify(struct Tile* tile, Pos2* in, Pos2* out)
{
	return 0;
}
