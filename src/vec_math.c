#include "vec_math.h"

void crs(GLfloat *u, GLfloat *v, GLfloat *rtn)
{
	rtn[0] = u[1]*v[2]-u[2]*v[1];
	rtn[1] = -(u[0]*v[2]-u[2]*v[0]);
	rtn[2] = (u[0]*v[1]-u[1]*v[0]);
}

float lenSq(GLfloat *v)
{
	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}

float len(GLfloat *v)
{
	return sqrt(lenSq(v));
}

float nor(GLfloat *v, GLfloat *rtn)
{
	float length = len(v);
	rtn[0] = v[0]/length;
	rtn[1] = v[1]/length;
	rtn[2] = v[2]/length;
}