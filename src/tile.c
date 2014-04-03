#include "tile.h"

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "textures.h"

void create_tile(struct Tile* tile)
{
	create_pos(&(tile->pos));
	tile->walls = 0;
}
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
		
	glTexCoord2f (0, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2,
		TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2);
	glTexCoord2f (1, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2,
		TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	glTexCoord2f (1, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
		TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	glTexCoord2f (0, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
		TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2);
	if (tile_check_wall(tile, WALL_SOUTH) == 0)
	{
		glTexCoord2f (0, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (0, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
	}
	if (tile_check_wall(tile, WALL_NORTH) == 0)
	{
		glTexCoord2f (0, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
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
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	}
	if (tile_check_wall(tile, WALL_WEST) == 0)
	{
		glTexCoord2f (0, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
		glTexCoord2f (0, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2+TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	}
	if (tile_check_wall(tile, WALL_EAST) == 0)
	{
		glTexCoord2f (0, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 0);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2);
		glTexCoord2f (1, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			-TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
		glTexCoord2f (0, 1);
		glVertex3f(
			tile->pos.x*TILE_WIDTH*2,
			TILE_DEPTH,
			tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT*2);
	}
	
}

void tile_add_wall(struct Tile* tile, int wall)
{
	tile->walls |= wall;
}
void tile_remove_wall(struct Tile* tile, int wall)
{
	tile->walls &= ~wall;
}

int tile_check_wall(struct Tile* tile, int wall)
{
	if (tile->walls & wall)
		return 0;
	return 1;
}

void tile_make_walls(struct TileElement* tiles, struct Tile* tile)
{
	tile_add_wall(tile, WALL_SOUTH | WALL_NORTH | WALL_EAST | WALL_WEST);
	struct TileElement* currTile = tiles;
	while(currTile != NULL)
	{
		Pos2 diff;
		pos_sub(tile->pos, currTile->elem->pos, &diff);
		if (
			(diff.x == 0 || diff.y == 0) &&
			(abs(diff.x) == 1 || abs(diff.y) == 1))
		{
			if (diff.y == -1)
				tile_remove_wall(tile, WALL_NORTH);
			if (diff.y == 1)
				tile_remove_wall(tile, WALL_SOUTH);
			if (diff.x == -1)
				tile_remove_wall(tile, WALL_WEST);
			if (diff.x == 1)
				tile_remove_wall(tile, WALL_EAST);
		}
		currTile = currTile->next;
	}
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
	//if (lowest == 0.0f)
	out->x = in->x - totDiff.x;
	out->y = in->y - totDiff.y;
	return 0;
}
int tile_rectify(struct Tile* tile, Pos2* in, Pos2* out)
{
	return 0;
}
