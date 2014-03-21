#ifndef VEC_MATH_H_
#define VEC_MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>

void crs(GLfloat *u, GLfloat *v, GLfloat *rtn);

float lenSq(GLfloat *v);

float len(GLfloat *v);

float nor(GLfloat *v, GLfloat *rtn);
#endif