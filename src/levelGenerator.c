#include "levelGenerator.h"
#include "tile.h"
#include "util.h"
#include "action.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

// This will make it easier to make tiles and change the way tiles are made
int addTile(struct TileElement **tiles, float x, float y, int curr)
{
	struct Tile* tile = malloc(sizeof(*tile));
	//((tile->pos)) = malloc(sizeof((tile->pos)));
	create_tile(tile);
	tile->pos.x = x;
	tile->pos.y = y;
	game_add_tile(tiles, tile);
	//tiles[curr] = t;
	return 1;
};

// tag a new exit to the LLL of exits
void create_exit(struct Exit** exits, struct Exit* rtn)
{
	rtn->next = *exits;
	rtn->angle = 0;
	rtn->type = 0;
	(*exits) = rtn;
}

// this figures out where the exits are using shitty
// step by step derivatives
void gen_exits(struct CaveRoom* cr)
{
	float ang = 0;
	float last = 0;
	// iterate through angles slowly
	while(ang < M_PI*2.0f)
	{
		// get the value of the derivative at this angle
		float curr = cave_get_dir(cr, ang);
		if (curr < 0 && last > 0)
		{
			struct Exit* exit = malloc(sizeof(*exit));
			create_exit(&(cr->exits), exit);
			exit->angle = ang;
		}
		last = curr;
		ang += 0.1f;
	}
}

// setting up the cave room
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
	cr->exits = NULL;
};

// generate a cave room from a random generator
void gen_cave_room(struct CaveRoom* cr, struct CaveRoomGenerator caveGen)
{
	cr->exits = NULL;
	
	cr->origin.x = 0;
	cr->origin.y = 0;
	
	cr->radius = ((float)rand()/(float)RAND_MAX)*(caveGen.max_radius-caveGen.min_radius)+caveGen.min_radius;
	cr->rotation = ((float)rand()/(float)RAND_MAX)*(caveGen.minRot-caveGen.maxRot)+caveGen.minRot;
	
	pos_cpy(caveGen.max_trig_effect, &(cr->trig_effect));
	pos_sub(cr->trig_effect, caveGen.min_trig_effect, &(cr->trig_effect));
	pos_mul(cr->trig_effect, (float)rand()/(float)RAND_MAX, &(cr->trig_effect));
	pos_add(cr->trig_effect, caveGen.min_trig_effect, &(cr->trig_effect));
	pos_print(cr->trig_effect);
	
	pos_cpy(caveGen.max_trig_period, &(cr->trig_period));
	pos_sub(cr->trig_period, caveGen.min_trig_period, &(cr->trig_period));
	pos_mul(cr->trig_period, (float)rand()/(float)RAND_MAX, &(cr->trig_effect));
	pos_add(cr->trig_period, caveGen.min_trig_period, &(cr->trig_period));
	pos_flr(cr->trig_period, &(cr->trig_period));
}

// the "rel" means "relative" meaning the cave room's origin is 
// not taken into consideration.
int check_in_cave_room_rel(struct CaveRoom* cr, Pos2* pos)
{
	float angle = pos_angle_rel(*pos);
	float len = pos_len(*pos);
	if (len < cave_get_radius(cr, angle))
		return 0;
	return 1;
};

// 
float cave_get_radius(struct CaveRoom* cr, float rad)
{
	return (
		cr->trig_effect.x*sin(cr->trig_period.x*(rad+cr->rotation))+
		cr->trig_effect.y*cos(cr->trig_period.y*(rad+cr->rotation))+
		cr->radius);
}

float cave_get_dir(struct CaveRoom* cr, float rad)
{
	return (
		cr->trig_effect.x*cr->trig_period.x*cos(cr->trig_period.x*(rad+cr->rotation))-
		cr->trig_effect.y*cr->trig_period.y*sin(cr->trig_period.y*(rad+cr->rotation)));
}

int addCaveRoom(struct TileElement **tiles, struct CaveRoom* cr, int curr)
{
	int box_len = ceil(cr->radius+pos_most(cr->trig_effect));
	int box_size = box_len*box_len*4;
	int num_tries = 0;//-box_size;
	int added = 0;
	while (num_tries < box_size)
	{
		Pos2 tile_pos;
		tile_pos.x = floor(num_tries/(box_len*2))-box_len;
		tile_pos.y = num_tries%(box_len*2)-box_len;
		if (check_in_cave_room_rel(cr, &tile_pos) == 0)
			addTile(tiles, tile_pos.x+cr->origin.x, tile_pos.y+cr->origin.y, curr+added);
		++num_tries;
	}
	return added;
};

int check_intersect(struct CaveRoom* cr1, struct CaveRoom* cr2)
{
	Pos2 dst;
	pos_sub(cr2->origin, cr1->origin, &dst);
	float angle = pos_angle_rel(dst);
	float addAngle = 0.0f;
	while(addAngle < M_PI/2.0f)
	{
		cr1->rotation + addAngle;
		addAngle += 0.1f;
	}
}

int cave_branch(struct CaveRoom* main, struct CaveRoomGenerator* caveGen, struct CaveRoom* rtn)
{
	gen_cave_room(rtn, *caveGen);
	gen_exits(rtn);
	float ang1 = main->exits->angle;
	float mag1 = cave_get_radius(main, ang1);
	float ang2 = rtn->exits->angle;
	fprintf(stderr, "angle 1: %f, angle 2: %f\n", ang1, ang2);
	float mag2 = cave_get_radius(rtn, ang2);
	rtn->rotation += ang2-ang1+M_PI;
	rtn->origin.x = (int)round(cos(ang1)*(mag1+mag2)*1.0f);
	rtn->origin.y = (int)round(sin(ang1)*(mag1+mag2)*1.0f);
	/*
	struct Exit* exit = main->exits;
	while (exit != NULL)
	{
		
		exit = exit->next;
	}*/
}

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

void gen_cave_gen(struct CaveRoomGenerator* caveGen, 
	float min_radius,
	float max_radius,
	float min_trig_effect_sin,
	float min_trig_effect_cos,
	float max_trig_effect_sin,
	float max_trig_effect_cos,
	float min_trig_period_sin,
	float min_trig_period_cos,
	float max_trig_period_sin,
	float max_trig_period_cos,
	float minRot,
	float maxRot)
{
	caveGen->min_radius = min_radius;
	caveGen->max_radius = max_radius;
	caveGen->min_trig_effect.x = min_trig_effect_sin;
	caveGen->min_trig_effect.y = min_trig_effect_cos;
	caveGen->max_trig_effect.x = max_trig_effect_sin;
	caveGen->max_trig_effect.y = max_trig_effect_cos;
	caveGen->min_trig_period.x = min_trig_period_sin;
	caveGen->min_trig_period.y = min_trig_period_cos;
	caveGen->max_trig_period.x = max_trig_period_sin;
	caveGen->max_trig_period.y = max_trig_period_cos;
	caveGen->minRot = minRot;
	caveGen->maxRot = maxRot;
};

void generateLevel(struct TileElement** tiles, int num_tiles, struct CreatureElement** creatures, int num_creatures, struct ActionElement** actions)
{
	int curr = 0;
	srand(time(NULL));
	fprintf(stderr, "rnd: %i\n", rand());
	fprintf(stderr, "rnd2: %f\n", (float)rand()/(float)RAND_MAX);
	
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
	struct CaveRoomGenerator caveGen;
	gen_cave_gen(&caveGen, 
		7, 
		8, 
		7, 7, 
		9, 9, 
		4, 4, 
		6, 6, 
		0, M_PI*2);

	//create_cave_room(&cr1, 0, 0, 7, 2.0f, 3, 2, 4, 5);
	gen_cave_room(&cr1, caveGen);
	gen_exits(&cr1);
	struct CaveRoom cr2;
	cave_branch(&cr1, &caveGen, &cr2);
	addCaveRoom(tiles, &cr1, curr);
	//pos_print(cr2.origin);
	addCaveRoom(tiles, &cr2, curr);
	
	
	
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
	
	bj->items = malloc(sizeof(*bj->items));
	bj->items->next = NULL;
	bj->items->elem = malloc(sizeof(*bj->items->elem));
	item_create(bj->items->elem, ITM_BLUE_JELLY);
	
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
	
	struct TileElement* currTile = *tiles;
	while(currTile != NULL)
	{
		tile_make_walls(*tiles, currTile->elem);
		currTile = currTile->next;
	}
	
};
