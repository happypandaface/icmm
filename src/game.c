#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <util.h>

#include "creature.h"
#include "action.h"

void game_loop(struct icmmGame* game, float dt)
{
	/*
	{
		int i = 0;
		while (i < NUM_CREATURES)
		{
			if (creature_check_sub_type(&(game->creatures[i]), STYP_EXISTS) == 0)
			{
				Pos2 dst;
				pos_sub(game->player->pos, game->creatures[i].pos, &dst);
				float len = pos_len(dst);
				if (len > 1)
				{
					pos_nor(dst, &dst);
					pos_mul(dst, dt*2.0f, &dst);
					pos_add(game->creatures[i].pos, dst, &(game->creatures[i].pos));
				}
			}
			++i;
		}
	}
	*/
	{
		struct ActionElement* currAction = game->acts;
		struct ActionElement* lastAction = NULL;
		while (currAction != NULL)
		{
			if (action_check_subtype(currAction->act, SACT_DISABLED) != 0)
			{
				if (action_perform(currAction->act, game, dt) == 1)
				{
					if (lastAction == NULL)// remove the action
					{
						lastAction = game->acts;
						if (game->acts == currAction)
						{
							game->acts = currAction->next;
						}else
						{
							while(lastAction->next != currAction)
								lastAction = lastAction->next;
							lastAction->next = currAction->next;
						}
					}else
						lastAction->next = currAction->next;
				}
			}
			lastAction = currAction;
			currAction = currAction->next;
		}
	}
}

void game_init(struct icmmGame* game)
{
	game->acts = NULL;
	game->tiles = NULL;
	game->creatures = NULL;
	game->items = NULL;
	
	game->player = malloc(sizeof(*(game->player)));
	creature_create(game->player, TYP_PLAYER);
	game->player->pos.x = 0;
	game->player->pos.y = 0;
	game->player->items = malloc(sizeof(*game->player->items));
	game->player->items->next = NULL;
	game->player->items->elem = malloc(sizeof(*game->player->items->elem));
	item_create(game->player->items->elem, ITM_HAND);
	
	controlObject(game->player);
	
	generateLevel(&(game->tiles), NUM_TILES, &(game->creatures), NUM_CREATURES, &(game->acts));
	
}

void game_add_action(struct ActionElement** elems, struct Action* act)
{
	struct ActionElement* ae = malloc(sizeof(*ae));
	ae->act = act;
	ae->next = *elems;
	(*elems) = ae;
}

void game_add_tile(struct TileElement** elems, struct Tile* elem)
{
	struct TileElement* elemObj = malloc(sizeof(*elemObj));
	elemObj->elem = elem;
	elemObj->next = *elems;
	(*elems) = elemObj;
}

void game_add_creature(struct CreatureElement** elems, struct Creature* elem)
{
	struct CreatureElement* elemObj = malloc(sizeof(*elemObj));
	elemObj->elem = elem;
	elemObj->next = *elems;
	(*elems) = elemObj;
}

void game_remove_creature(struct icmmGame* game, struct Creature* creat)
{
	if (game->creatures->elem == creat)
	{
		game->creatures = game->creatures->next;
	}else
	{
		struct CreatureElement* creatElem = game->creatures;
		while(creatElem->next->elem != creat)
			creatElem = creatElem->next;
		creatElem->next = creatElem->next->next;
	}

}
