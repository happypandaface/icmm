#include "item.h"
#include "textures.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <math.h>

void item_create(Item* it, int type)
{
	it->type = type;
	it->sub_type = SITM_EXISTS;
	it->anim = NULL;
	if (type == ITM_HAND)
		it->texture = TEX_FIST;
	else
		it->texture = TEX_NOTHING;
}
void item_add_type(Item* it, long stype)
{
	it->sub_type |= stype;
}
int item_check_type(Item* it, long stype)
{
	if (it->sub_type & stype)
		return 0;
	return 1;
}
void item_draw(Item* it, float dt)
{
	glLoadIdentity();
	if (it->anim != NULL)
	{
		if (it->anim->timesLooped > 0)
		{
			unload_animation(&(it->anim));
		}else
		{
			step_animation(it->anim, dt);
			load_texture(it->anim->currentFrame->texture);
		}
	}
	if (it->anim == NULL)
		load_texture(it->texture);
	glPushMatrix();
	glTranslatef(2.0f, -2.0f, 0.0f);
	glBegin(GL_QUADS);
	glTexCoord2f (0, 1);
	glVertex3f(
		-2.0f,
		-2.0f,
		-10.0f);
	glTexCoord2f (1, 1);
	glVertex3f(
		2.0f,
		-2.0f,
		-10.0f);
	glTexCoord2f (1, 0);
	glVertex3f(
		2.0f,
		2.0f,
		-10.0f);
	glTexCoord2f (0, 0);
	glVertex3f(
		-2.0f,
		2.0f,
		-10.0f);
	glEnd();
	glPopMatrix();
}