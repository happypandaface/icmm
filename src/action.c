#include "action.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void action_create(struct Action** act, int type)
{
	(*act) = malloc(sizeof((**act)));
	(*act)->actObj = NULL;
	(*act)->type = type;
}
void action_obj_create(struct ActionObject** actObj, int value_type)
{
	(*actObj) = malloc(sizeof(**actObj));
	(*actObj)->value_type = value_type;
}
void action_add(struct Action* act, struct ActionObject* nextObj, int role)
{
	nextObj->role = role;
	nextObj->next = act->actObj;
	act->actObj = nextObj;
}
int action_get(struct ActionObject* act, struct ActionObject** rtn, int value_type, int role, int num)
{
	struct ActionObject* currAct = act;
	while (currAct != NULL)
	{
		if (currAct->role == role &&
			currAct->value_type == value_type)
		{
			(*rtn) = currAct;
			return 0;
		}
		currAct = currAct->next;
	}
	return 1;
}
int action_perform(struct Action* act, struct icmmGame* game, float dt)
{
	if (act->type == ACT_NOTHING)
	{
	}else
	if (act->type == ACT_PUNCH)
	{
		struct ActionObject* actTime;
		struct ActionObject* timer;
		struct ActionObject* user;
		int err = 0;
		err += action_get(act->actObj, &actTime, ACT_OBJ_FLOAT, ACT_ROLE_ACTIVATE, 0);
		err += action_get(act->actObj, &timer, ACT_OBJ_FLOAT, ACT_ROLE_TIMER, 0);
		err += action_get(act->actObj, &user, ACT_OBJ_CREATURE, ACT_ROLE_USER, 0);
		if (err == 0)
		{
			timer->fvalue += dt;
			if (timer->fvalue > actTime->fvalue)
			{
				
				return 1;
			}
		}else
		{
			printf("action removed due to error\n");
			return 2;
		}
	}else
	if (act->type == ACT_MOVE)
	{
		struct ActionObject* angle;
		struct ActionObject* mag;
		struct ActionObject* user;
		int err = 0;
		err += action_get(act->actObj, &angle, ACT_OBJ_FLOAT, ACT_ROLE_ANGLE, 0);
		err += action_get(act->actObj, &mag, ACT_OBJ_FLOAT, ACT_ROLE_MAGNITUDE, 0);
		err += action_get(act->actObj, &user, ACT_OBJ_CREATURE, ACT_ROLE_USER, 0);
		if (err == 0)
		{
			user->creature->pos.x += sin(user->creature->angle+angle->fvalue)*dt*mag->fvalue;
			user->creature->pos.y += -cos(user->creature->angle+angle->fvalue)*dt*mag->fvalue;
			tiles_rectify(&(game->tiles), &(user->creature->pos), &(user->creature->pos));
		}else
		{
			printf("action removed due to error\n");
			return 2;
		}
	}else
	if (act->type == ACT_FOLLOW)
	{
		struct ActionObject* mag;
		struct ActionObject* target;
		struct ActionObject* user;
		int err = 0;
		err += action_get(act->actObj, &target, ACT_OBJ_CREATURE, ACT_ROLE_TARGET, 0);
		err += action_get(act->actObj, &mag, ACT_OBJ_FLOAT, ACT_ROLE_MAGNITUDE, 0);
		err += action_get(act->actObj, &user, ACT_OBJ_CREATURE, ACT_ROLE_USER, 0);
		if (err == 0)
		{
			Pos2 dst;
			pos_sub(user->creature->pos, target->creature->pos, &dst);
			float len = pos_len(dst);
			if (len > 1)
			{
				pos_nor(dst, &dst);
				pos_mul(dst, dt*mag->fvalue, &dst);
				pos_add(target->creature->pos, dst, &(target->creature->pos));
			}
		}else
		{
			printf("action removed due to error\n");
			return 2;
		}
	}
	return 0;
}
void action_destroy(struct Action** act)
{
	
}
void action_add_subtype(struct Action* act, long sub_type)
{
	act->sub_type |= sub_type;
}
void action_remove_subtype(struct Action* act, long sub_type)
{
	act->sub_type &= ~sub_type;
}
int action_check_subtype(struct Action* act, long sub_type)
{
	if (act->sub_type & sub_type)
		return 0;
	return 1;
}
