#include "util.h"

#include <math.h>
#include <stdio.h>

void pos_sub(Pos2 p1, Pos2 p2, Pos2 *rtn)
{
	rtn->x = p1.x - p2.x;
	rtn->y = p1.y - p2.y;
}

float pos_len_sq(Pos2 p)
{
	return p.x*p.x+p.y*p.y;
}

float pos_len(Pos2 p)
{
	return sqrt(pos_len_sq(p));
}

void pos_nor(Pos2 p, Pos2 *rtn)
{
	float len = pos_len(p);
	rtn->x = p.x/len;
	rtn->y = p.y/len;
}

void pos_mul(Pos2 p, float factor, Pos2 *rtn)
{
	rtn->x = p.x*factor;
	rtn->y = p.y*factor;
}

void pos_add(Pos2 p1, Pos2 p2, Pos2 *rtn)
{
	rtn->x = p1.x+p2.x;
	rtn->y = p1.y+p2.y;
}

void pos_print(Pos2 p)
{
	char cx[48];
	snprintf(cx, sizeof(cx), "%f",p.x);
	char cy[48];
	snprintf(cy, sizeof(cy), "%f",p.y);
	printf("x: %s, y: %s\n", cx, cy);
}