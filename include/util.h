#ifndef UTIL_H
#define UTIL_H

typedef struct
{
	float x;
	float y;
} Pos2;

void create_pos(Pos2* pos);
void pos_cpy(Pos2 p, Pos2* rtn);
void pos_sub(Pos2 p1, Pos2 p2, Pos2 *rtn);
float pos_angle_rel(Pos2 p);
float pos_len_sq(Pos2 p);
float pos_len(Pos2 p);
void pos_nor(Pos2 p1, Pos2 *rtn);
void pos_flr(Pos2 p, Pos2 *rtn);
void pos_mul(Pos2 p1, float factor, Pos2 *rtn);
void pos_add(Pos2 p1, Pos2 p2, Pos2 *rtn);
void pos_print(Pos2 p);
float pos_least(Pos2 p);
float pos_most(Pos2 p);

float nor_rad(float a);
float compare_rad(float a1, float a2);

#endif
