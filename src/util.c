#include "util.h"

#include <math.h>
#include <stdio.h>

void create_pos(Pos2* pos)
{
	pos->x = 0;
	pos->y = 0;
}
void pos_sub(Pos2 p1, Pos2 p2, Pos2 *rtn)
{
	rtn->x = p1.x - p2.x;
	rtn->y = p1.y - p2.y;
}

float pos_angle_rel(Pos2 p)
{
	return atan2(p.y, p.x);
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

void pos_flr(Pos2 p, Pos2 *rtn)
{
	rtn->x = floor(p.x);
	rtn->y = floor(p.y);
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

void pos_cpy(Pos2 p, Pos2* rtn)
{
	rtn->x = p.x;
	rtn->y = p.y;
}

void pos_print(Pos2 p)
{
	char cx[48];
	snprintf(cx, sizeof(cx), "%f",p.x);
	char cy[48];
	snprintf(cy, sizeof(cy), "%f",p.y);
	fprintf(stderr, "x: %s, y: %s\n", cx, cy);
}
float pos_least(Pos2 p)
{
	if (abs(p.x) > abs(p.y))
		return p.y;
	return p.x;
}
float pos_most(Pos2 p)
{
	if (abs(p.x) < abs(p.y))
		return p.y;
	return p.x;
}
float nor_rad(float a)
{
	float a_rad = a;
	while (a_rad > M_PI)
		a_rad -= M_PI*2.0f;
	while (a_rad < -M_PI)
		a_rad += M_PI*2.0f;
	return a_rad;
}
float compare_rad(float a1, float a2)
{
	float a1_nor = nor_rad(a1);
	float a2_nor = nor_rad(a2);
	float diff1 = a1_nor - a2_nor;
	float diff2 = a2_nor - a1_nor;
	if (abs(diff1) < abs(diff2))
		return diff1;
	else
		return diff2;
}
