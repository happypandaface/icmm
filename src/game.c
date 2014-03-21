#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <util.h>

#include "creature.h"
#include "action.h"

void game_loop(struct icmmGame* game, float dt)
{
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
	{
		struct ActionElement* currAction = game->acts;
		struct ActionElement* lastAction = NULL;
		while (currAction != NULL)
		{
			if (action_check_subtype(currAction->act, SACT_DISABLED) != 0)
			{
				if (action_perform(currAction->act, game, dt) == 1)
					if (lastAction == NULL)// remove the action
						game->acts = currAction->next;
					else
						lastAction->next = currAction->next;
			}
			lastAction = currAction;
			currAction = currAction->next;
		}
	}
}

void game_init(struct icmmGame* game)
{
	game->player = malloc(sizeof(*(game->player)));
	creature_create(game->player, TYP_PLAYER);
	game->player->inv = malloc(sizeof(*game->player->inv));
	game->player->inv->item = malloc(sizeof(*game->player->inv->item));
	item_create(game->player->inv->item, ITM_HAND);
	generateLevel(game->tiles, NUM_TILES, game->creatures, NUM_CREATURES);
	
	game->acts = NULL;
	
	controlObject(game->player);
}

void game_add_action(struct icmmGame* game, struct Action* act)
{
	struct ActionElement* ae = malloc(sizeof(*ae));
	ae->act = act;
	ae->next = game->acts;
	game->acts = ae;
}
