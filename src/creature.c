#include "creature.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>

#include "animation.h"
#include "textures.h"

void creature_draw(struct Creature* go, float viewAngle, float dt)
{
	if (go->animInst != NULL)
	{
		step_animation(go->animInst, dt);
		load_texture(go->animInst->currentFrame->texture);
	}else
	{
		load_texture(go->texture);
	}
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
	glTexCoord2f (0, 1);
	glVertex3f(
		-CREATURE_WIDTH,
		-CREATURE_DEPTH,
		CREATURE_HEIGHT);
	glTexCoord2f (1, 1);
	glVertex3f(
		CREATURE_WIDTH,
		-CREATURE_DEPTH,
		CREATURE_HEIGHT);
	glTexCoord2f (1, 0);
	glVertex3f(
		CREATURE_WIDTH,
		-CREATURE_DEPTH,
		-CREATURE_HEIGHT);
	glEnd();
	glPopMatrix();
}

int creature_create(struct Creature* c, int type)
{
	c->type = type;
	c->inv = NULL;
	c->damage = malloc(sizeof(*(c->damage)));
	if (type == TYP_GAS_BALL)
	{
		c->damage->health = 2.0f;
		c->animInst = malloc(sizeof(*(c->animInst)));
		set_instance(anm_gas_ball, c->animInst);
	}else
	if (type == TYP_BLUE_JELLY)
	{
		c->damage->health = 2.0f;
		c->animInst = malloc(sizeof(*(c->animInst)));
		set_instance(anm_blue_jelly_jiggle, c->animInst);
	}
}

int creature_check_sub_type(struct Creature* c, long stype)
{
	if (c->sub_type & stype)
		return 0;
	return 1;
}

int creature_add_sub_type(struct Creature* c, long stype)
{
	c->sub_type |= stype;
	return 0;
}
