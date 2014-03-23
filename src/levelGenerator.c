#include "levelGenerator.h"
#include "tile.h"
#include "util.h"
#include "action.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Room
{
	Pos2 pos;
	Pos2 dim;
};

struct CaveRoom
{
	Pos2 origin;
	float radius;
	float rotation;
	Pos2 trig_effect;
	Pos2 trig_period;
};

int addTile(struct TileElement **tiles, float x, float y, int curr)
{
	struct Tile* tile = malloc(sizeof(*tile));
	printf("tile size: %i\n", sizeof(*tile));
	//((tile->pos)) = malloc(sizeof((tile->pos)));
	pos_print(tile->pos);
	tile->pos.x = x;
	tile->pos.y = y;
	printf("set\n");
	game_add_tile(tiles, tile);
	printf("done\n");
	//tiles[curr] = t;
	return 1;
};

void create_cave_room(struct CaveRoom* cr, float x, float y, float radius, float rotation, float sin_effect, float cos_effect, float sin_period, float cos_period)
{
	cr->origin.x = x;
	cr->origin.y = y;
	cr->radius = radius;
	cr->rotation = rotation;
	cr->trig_effect.x = sin_effect;
	cr->trig_effect.y = cos_effect;
	cr->trig_period.x = sin_period;
	cr->trig_period.y = cos_period;
};

int check_in_cave_room_rel(struct CaveRoom* cr, Pos2* pos)
{
	float angle = pos_angle_rel(*pos);
	float len = pos_len(*pos);
	if (len < 	cr->trig_effect.x*sin(cr->trig_period.x*(angle+cr->rotation))+
				cr->trig_effect.y*cos(cr->trig_period.y*(angle+cr->rotation))+
				cr->radius)
		return 0;
	return 1;
};

int addCaveRoom(struct TileElement **tiles, struct CaveRoom* cr, int curr)
{
	int box_len = ceil(cr->radius+pos_most(cr->trig_effect));
	int box_size = box_len*box_len*4;
	//fprintf(stderr, "box_size: %i\n", box_size);
	int num_tries = 0;//-box_size;
	int added = 0;
	while (num_tries < box_size)
	{
		Pos2 tile_pos;
		tile_pos.x = floor(num_tries/(box_len*2))-box_len;
		tile_pos.y = num_tries%(box_len*2)-box_len;
		if (check_in_cave_room_rel(cr, &tile_pos) == 0)
			addTile(tiles, tile_pos.x, tile_pos.y, curr+added);
		++num_tries;
	}
	return added;
};

int addRoom(struct TileElement** tiles, struct Room rm, int curr)
{
	int added = 0;
	int x = rm.pos.x;
	while (x < rm.pos.x+rm.dim.x)
	{
		int y = rm.pos.y;
		while (y < rm.pos.y+rm.dim.y)
		{
			added += addTile(tiles, x, y, curr+added);
			++y;
		}
		++x;
	}
	return added;
};


void generateLevel(struct TileElement** tiles, int num_tiles, struct CreatureElement** creatures, int num_creatures, struct ActionElement** actions)
{
	int curr = 0;
	
	/*
	struct Room r1;
	r1.pos.x = 1;
	r1.pos.y = 2;
	r1.dim.x = 5;
	r1.dim.y = 6;
	curr += addRoom(tiles, r1, curr);
	
	
	struct Room r2;
	r2.pos.x = 8;
	r2.pos.y = 3;
	r2.dim.x = 4;
	r2.dim.y = 7;
	
	curr += addRoom(tiles, r2, curr);
	*/
	//struct Room path;
	/*
	while (curr < num_tiles)
	{
		struct Tile t;
		t.pos.x = curr;
		t.pos.y = curr;
		tiles[curr] = t;
		++curr;
	}*/
	
	
	struct CaveRoom cr1;
	create_cave_room(&cr1, 0, 0, 7, 0, 3, 2, 4, 5);
	addCaveRoom(tiles, &cr1, curr);
	struct Creature *creat = malloc(sizeof(*creat));
	creature_create(creat, TYP_GAS_BALL);
	creat->pos.x = 2;
	creat->pos.y = 4;
	creature_add_sub_type(creat, STYP_EXISTS);
	game_add_creature(creatures, creat);
	
	struct Creature *bj = malloc(sizeof(*bj));
	creature_create(bj, TYP_BLUE_JELLY);
	bj->pos.x = 4;
	bj->pos.y = 4;
	creature_add_sub_type(bj, STYP_EXISTS);
	game_add_creature(creatures, bj);
	
	struct Action* follow_act;
	action_create(&follow_act, ACT_FOLLOW);
	
	struct ActionObject* user;
	action_obj_create(&user, ACT_OBJ_CREATURE);
	user->creature = mainGame->player;
	action_add(follow_act, user, ACT_ROLE_USER);
	
	struct ActionObject* creatActObj;
	action_obj_create(&creatActObj, ACT_OBJ_CREATURE);
	creatActObj->creature = creat;
	action_add(follow_act, creatActObj, ACT_ROLE_TARGET);
	
	struct ActionObject* mag;
	action_obj_create(&mag, ACT_OBJ_FLOAT);
	mag->fvalue = 1.0f;
	action_add(follow_act, mag, ACT_ROLE_MAGNITUDE);
	
	// make sure it's disabled at the start
	//action_add_subtype(follow_act, SACT_DISABLED);
	
	game_add_action(actions, follow_act);
	
	
	
	printf("done generating\n");
};
