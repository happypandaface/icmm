#include "action.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

#include "creature.h"
#include "combat.h"

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
	(*actObj)->next = NULL;
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

int objects_get_cone(
	struct icmmGame* game, 
	Pos2 start, 
	float dir, 
	float angle, 
	float startDist, 
	float endDist, 
	int value_type, 
	int object_type, 
	long sub_type, 
	struct ActionObject** rtn)
{
	int numFound = 0;
	float rad_angle = angle/180.0f*M_PI/2.0f;
	struct CreatureElement* creatElem = game->creatures;
	while (creatElem != NULL)
	{
		Pos2 diff;
		pos_sub(start, creatElem->elem->pos, &diff);
		float dist = pos_len(diff);
		float creatAngle = pos_angle_rel(diff)-M_PI/2.0f;
		float diffAngle = compare_rad(creatAngle, dir);
		if (	object_type == -1 && // this means any object
			dist >= startDist &&
			dist <= endDist &&
			abs(diffAngle) < rad_angle)
		{
			struct ActionObject* currReturn;
			action_obj_create(&currReturn, value_type);
			currReturn->creature = creatElem->elem;
			if (*rtn == NULL)
				(*rtn) = currReturn;
			else
			{
				struct ActionObject* temp = *rtn;
				(*rtn) = currReturn;
				currReturn->next = temp;
			}
			++numFound;
		}
		creatElem = creatElem->next;
	}
	return numFound;
}
int objects_get_ring(
	struct icmmGame* game, 
	Pos2 start, 
	float startDist, 
	float endDist, 
	int value_type, 
	int object_type, 
	long sub_type, 
	struct ActionObject** rtn)
{
	return objects_get_cone(
		game,
		start,
		0.0f,
		360.0f,
		startDist,
		endDist,
		value_type,
		object_type,
		sub_type,
		rtn);
}

int action_perform(struct Action* act, struct icmmGame* game, float dt)
{
	if (act->type == ACT_NOTHING)
	{
	}else
	if (act->type == ACT_DAMAGE)
	{
		struct ActionObject* damage;
		struct ActionObject* target;
		struct ActionObject* damageType;
		struct ActionObject* user;
		int err = 0;
		err += action_get(act->actObj, &damageType, ACT_OBJ_LONG, ACT_ROLE_TYPE, 0);
		err += action_get(act->actObj, &damage, ACT_OBJ_FLOAT, ACT_ROLE_MAGNITUDE, 0);
		err += action_get(act->actObj, &user, ACT_OBJ_CREATURE, ACT_ROLE_USER, 0);
		err += action_get(act->actObj, &target, ACT_OBJ_CREATURE, ACT_ROLE_TARGET, 0);
		if (err == 0)
		{
			float health = combat_damage(target->creature->damage, DAMAGE_PUNCH);
			fprintf(stderr,"health left: %f\n", health);
			if (health <= 0)
			{
				struct ItemElement* creatItems = target->creature->items;
				while (creatItems != NULL)
				{
					struct Item* it = creatItems->elem;
					it->world_pos.x = target->creature->pos.x;
					it->world_pos.y = target->creature->pos.y;
					item_add_sub_type(it, SITM_IN_WORLD);
					items_add_item(&(game->items), it);
					items_remove_item(&(target->creature->items), it);
					creatItems = creatItems->next;
				}
				game_remove_creature(game, target->creature);
			}
			return 1;
		}else
		{
			printf("action removed due to error\n");
			return 2;
		}
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
				struct ActionObject* rtn = NULL;

				int punched = objects_get_cone(
					mainGame, 
					user->creature->pos, 
					user->creature->angle, 
					15.0f, 
					0.0f, 
					2.0f, 
					ACT_OBJ_CREATURE, 
					-1, 
					LONG_MAX, 
					&rtn);
				fprintf(stderr, "num hit: %i\n", punched);
				if (punched > 0)
				{
					struct ActionObject* currCreat = rtn;
					struct ActionObject* targetCreat;
					// get the closest one
					float dist = -1.0f;
					while (currCreat != NULL)
					{
						Pos2 distPos;
						pos_sub(currCreat->creature->pos, user->creature->pos, &distPos);
						float currDist = pos_len(distPos);
						if (dist == -1.0f || currDist < dist)
						{
							dist = currDist;
							targetCreat = currCreat;
						}
						currCreat = currCreat->next;	
					}

					struct Action* dmgAct;
					action_create(&dmgAct, ACT_DAMAGE);
					struct ActionObject* damage;
					action_obj_create(&damage, ACT_OBJ_FLOAT);
					damage->fvalue = DAMAGE_PUNCH;
					struct ActionObject* target;
					action_obj_create(&target, ACT_OBJ_CREATURE);
					target->creature = targetCreat->creature;
					struct ActionObject* damageType;
					action_obj_create(&damageType, ACT_OBJ_LONG);
					damageType->lvalue = 0;
					struct ActionObject* user2;
					action_obj_create(&user2, ACT_OBJ_CREATURE);
					user2->creature = user->creature;
					action_add(dmgAct, damage, ACT_ROLE_MAGNITUDE);
					action_add(dmgAct, damageType, ACT_ROLE_TYPE);
					action_add(dmgAct, user2, ACT_ROLE_USER);
					action_add(dmgAct, target, ACT_ROLE_TARGET);
					game_add_action(&(game->acts), dmgAct);
					
				}
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
