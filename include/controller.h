#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include "vec_math.h"
#include "creature.h"

#define MOVE_SPEED 10.0f
#define TURN_SPEED 5.0f

// The controller should maybe have a struct?
// the punching variable isn't used anymore.
// the controller sets up the camera to the
// perspective of the controlled object
// (which is set through the method "controlObject"
// 

int punching;

float getCameraAngle();
void updateController(float dt);
void controlCamera();
void controlObject(struct Creature *go);
void controller_init();
void normalUp(unsigned char key, int x, int y);
void normalDown(unsigned char key, int x, int y);
void specialDown(int key, int xx, int yy);
void specialUp(int key, int xx, int yy);

#endif
