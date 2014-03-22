#ifndef LEVEL_GENERATOR_H_
#define LEVEL_GENERATOR_H_
#include "tile.h"
#include "creature.h"
#include "action.h"

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

void generateLevel(struct TileElement**, int, struct CreatureElement** creatures, int, struct ActionElement**);

#endif