#include "creature.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>

void creature_draw(Creature* go, float viewAngle)
{
	glPushMatrix();
	glTranslatef(go->pos.x*CREATURE_WIDTH*2, 0.0f, go->pos.y*CREATURE_HEIGHT*2);
	glRotatef(-viewAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f (0, 0);
	glVertex3f(
		-CREATURE_WIDTH,
		-CREATURE_DEPTH,
		-CREATURE_HEIGHT);
	glTexCoord2f (1, 0);
	glVertex3f(
		-CREATURE_WIDTH,
		-CREATURE_DEPTH,
		CREATURE_HEIGHT);
	glTexCoord2f (1, 1);
	glVertex3f(
		CREATURE_WIDTH,
		-CREATURE_DEPTH,
		CREATURE_HEIGHT);
	glTexCoord2f (0, 1);
	glVertex3f(
		CREATURE_WIDTH,
		-CREATURE_DEPTH,
		-CREATURE_HEIGHT);
	glEnd();
	glPopMatrix();
}

int creature_create(Creature* c, int type)
{
	c->inv = NULL;
}

int creature_check_sub_type(Creature* c, long stype)
{
	if (c->sub_type & stype)
		return 0;
	return 1;
}

int creature_add_sub_type(Creature* c, long stype)
{
	c->sub_type |= stype;
	return 0;
}
