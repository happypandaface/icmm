#ifndef LEVEL_GENERATOR_H_
#define LEVEL_GENERATOR_H_
#include "tile.h"
#include "creature.h"
#include "action.h"


struct Room
{
	Pos2 pos;
	Pos2 dim;
};

struct Exit;
struct Exit
{
	float angle;
	int type;
	struct Exit* next;
};
struct CaveRoom
{
	Pos2 origin;
	float radius;
	float rotation;
	Pos2 trig_effect;
	Pos2 trig_period;
	struct Exit* exits;
};
struct CaveRoomGenerator
{
	float min_radius;
	float max_radius;
	Pos2 min_trig_effect;
	Pos2 max_trig_effect;
	Pos2 min_trig_period;
	Pos2 max_trig_period;
	float minRot;
	float maxRot;
};

void create_exit(struct Exit** exits, struct Exit* rtn);
void gen_exits(struct CaveRoom* cr);
void generateLevel(struct TileElement**, int, struct CreatureElement** creatures, int, struct ActionElement**);
float cave_get_radius(struct CaveRoom* cr, float rad);
float cave_get_dir(struct CaveRoom* cr, float rad);

#endif