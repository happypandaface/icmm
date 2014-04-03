#include "item.h"


#include <stdio.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <math.h>

#include "textures.h"
#include "action.h"

void item_create(struct Item* it, int type)
{
	it->type = type;
	it->sub_type = SITM_EXISTS;
	it->anim = NULL;
	if (type == ITM_HAND)
		it->texture = TEX_FIST;
	else
	if (type == ITM_BLUE_JELLY)
		it->texture = TEX_DEAD_JELLY;
	else
		it->texture = TEX_NOTHING;
}
void item_add_sub_type(struct Item* it, long stype)
{
	it->sub_type |= stype;
}
void item_remove_sub_type(struct Item* it, long stype)
{
	it->sub_type &= ~stype;
}
int item_check_sub_type(struct Item* it, long stype)
{
	if (it->sub_type & stype)
		return 0;
	return 1;
}
int item_use(struct Creature* creat, struct Item* item)
{
	if (item->type == ITM_HAND)
	{
		set_animation(&(item->anim), ANM_PUNCH);
		struct Action* punch;
		action_create(&punch, ACT_PUNCH);
		struct ActionObject* user;
		action_obj_create(&user, ACT_OBJ_CREATURE);
		user->creature = creat;
		action_add(punch, user, ACT_ROLE_USER);
		struct ActionObject* timer;
		action_obj_create(&timer, ACT_OBJ_FLOAT);
		timer->fvalue = 0.0f;
		action_add(punch, timer, ACT_ROLE_TIMER);
		struct ActionObject* maxTime;
		action_obj_create(&maxTime, ACT_OBJ_FLOAT);
		maxTime->fvalue = 0.3f;
		action_add(punch, maxTime, ACT_ROLE_ACTIVATE);
		game_add_action(&(mainGame->acts), punch);
	}
}

void item_draw_world(struct Item* it, float viewAngle, float dt)
{
	if (it->anim != NULL)
	{
		step_animation(it->anim, dt);
		load_texture(it->anim->currentFrame->texture);
	}else
	{
		load_texture(it->texture);
	}
	glPushMatrix();
	glTranslatef(it->world_pos.x*CREATURE_WIDTH*2, 0.0f, it->world_pos.y*CREATURE_HEIGHT*2);
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

void item_draw(struct Item* it, float dt)
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
void items_add_item(struct ItemElement** elems, struct Item* newElem)
{
	struct ItemElement* elemHolder = malloc(sizeof(*elemHolder));
	elemHolder->elem = newElem;
	elemHolder->next = *elems;
	(*elems) = elemHolder;
}
void items_remove_item(struct ItemElement** elems, struct Item* it)
{
	if ((*elems)->elem == it)
	{
		(*elems) = (*elems)->next;
	}else
	{
		struct ItemElement* itElem = (*elems);
		while(itElem->next->elem != it)
			itElem = itElem->next;
		itElem->next = itElem->next->next;
	}
	// should free the memory for the ItemElement created with
	// 		items_add_item
}
