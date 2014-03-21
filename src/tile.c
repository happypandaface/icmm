#include "tile.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

void tile_draw(Tile* tile)
{
	glTexCoord2f (0, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2-TILE_WIDTH,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2-TILE_HEIGHT);
	glTexCoord2f (1, 0);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2-TILE_WIDTH,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT);
	glTexCoord2f (1, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2+TILE_HEIGHT);
	glTexCoord2f (0, 1);
	glVertex3f(
		tile->pos.x*TILE_WIDTH*2+TILE_WIDTH,
		-TILE_DEPTH,
		tile->pos.y*TILE_HEIGHT*2-TILE_HEIGHT);
}