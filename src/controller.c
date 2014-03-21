#include "controller.h"

#include <math.h>

#include "animation.h"
#include "game.h"
#include "action.h"

long keysDown;
long upKey = 1 << 0;
long leftKey = 1 << 1;
long downKey = 1 << 2;
long rightKey = 1 << 3;

struct Action* move_act;
struct ActionObject* move_angle;

void controller_init()
{
	//move_act = malloc(sizeof(*move_act));
}

void goLeft(float dt)
{
	controlled->angle -= dt*TURN_SPEED;
	//lx = sin(angle);
	//lz = -cos(angle);
}
void goRight(float dt)
{
	controlled->angle += dt*TURN_SPEED;
	//lx = sin(angle);
	//lz = -cos(angle);
}
void goUp(float dt)
{
	controlled->pos.x += sin(controlled->angle)*dt*MOVE_SPEED;
	controlled->pos.y += -cos(controlled->angle)*dt*MOVE_SPEED;
	//x += lx * dt*MOVE_SPEED;
	//z += lz * dt*MOVE_SPEED;
}
void goDown(float dt)
{
	controlled->pos.x -= sin(controlled->angle)*dt*MOVE_SPEED;
	controlled->pos.y -= -cos(controlled->angle)*dt*MOVE_SPEED;
	//x -= lx * dt*MOVE_SPEED;
	//z -= lz * dt*MOVE_SPEED;
}

float getCameraAngle()
{
	return controlled->angle/3.14159f*180.0f;
}

void updateController(float dt)
{
	action_add_subtype(move_act, SACT_DISABLED);
	if (keysDown & leftKey)
		goLeft(dt);
	if (keysDown & rightKey)
		goRight(dt);
	if (keysDown & downKey)
	{
		action_remove_subtype(move_act, SACT_DISABLED);
		move_angle->fvalue = M_PI;
		//goDown(dt);
	}else
	if (keysDown & upKey)
	{
		action_remove_subtype(move_act, SACT_DISABLED);
		move_angle->fvalue = 0;
		//goUp(dt);
	}
}

void controlObject(Creature *go)
{
	controlled = go;
	
	action_create(&move_act, ACT_MOVE);
	
	struct ActionObject* user;
	action_obj_create(&user, ACT_OBJ_CREATURE);
	user->creature = controlled;
	action_add(move_act, user, ACT_ROLE_USER);
	
	struct ActionObject* mag;
	action_obj_create(&mag, ACT_OBJ_FLOAT);
	mag->fvalue = 5.0f;
	action_add(move_act, mag, ACT_ROLE_MAGNITUDE);
	
	action_obj_create(&move_angle, ACT_OBJ_FLOAT);
	move_angle->fvalue = 0.0f;
	action_add(move_act, move_angle, ACT_ROLE_ANGLE);
	
	// make sure it's disabled at the start
	action_add_subtype(move_act, SACT_DISABLED);
	
	game_add_action(mainGame, move_act);
}

void controlCamera()
{
	// Set the camera
	GLfloat up[3] = {0.0f, 1.0f, 0.0f};
	
	GLfloat f[3] = {sin(controlled->angle), 0.0f, -cos(controlled->angle)};
	GLfloat fNor[3];
	nor(f, fNor);
	
	GLfloat s[3];
	crs(fNor, up, s);
	GLfloat sNor[3];
	nor(s, sNor);
	
	GLfloat u[3];
	crs(sNor, fNor, u);
	
	GLfloat camMat[4][4];
	
	camMat[0][0] = s[0];
	camMat[1][0] = s[1];
	camMat[2][0] = s[2];
	camMat[3][0] = 0.0f;
	
	camMat[0][1] = u[0];
	camMat[1][1] = u[1];
	camMat[2][1] = u[2];
	camMat[3][1] = 0.0f;
	
	camMat[0][2] = -f[0];
	camMat[1][2] = -f[1];
	camMat[2][2] = -f[2];
	camMat[3][2] = 0.0f;
	
	camMat[0][3] = 0.0f;
	camMat[1][3] = 0.0f;
	camMat[2][3] = 0.0f;
	camMat[3][3] = 1.0f;
	
	glLoadIdentity();
	glMultMatrixf(camMat[0]);
	glTranslated(-controlled->pos.x*CREATURE_WIDTH*2, -1.0f, -controlled->pos.y*CREATURE_HEIGHT*2);
	
	//gluLookAt(	x, 1.0f, z,
	//		x+lx, 1.0f,  z+lz,
	//		0.0f, 1.0f,  0.0f);
}

void normalDown(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		if (controlled->inv->item->anim == NULL)
		{
			set_animation(&(controlled->inv->item->anim), ANM_PUNCH);
			struct Action* punch;
			action_create(&punch, ACT_PUNCH);
			struct ActionObject* user;
			action_obj_create(&user, ACT_OBJ_CREATURE);
			user->creature = controlled;
			action_add(punch, user, ACT_ROLE_USER);
			struct ActionObject* timer;
			action_obj_create(&timer, ACT_OBJ_FLOAT);
			timer->fvalue = 0.0f;
			action_add(punch, timer, ACT_ROLE_TIMER);
			struct ActionObject* maxTime;
			action_obj_create(&maxTime, ACT_OBJ_FLOAT);
			maxTime->fvalue = 0.3f;
			action_add(punch, maxTime, ACT_ROLE_ACTIVATE);
			game_add_action(mainGame, punch);
		}
	}
}

void normalUp(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 32)
		punching = 0;
}

void specialDown(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_LEFT :
			keysDown |= leftKey;
			break;
		case GLUT_KEY_RIGHT :
			keysDown |= rightKey;
			break;
		case GLUT_KEY_UP :
			keysDown |= upKey;
			break;
		case GLUT_KEY_DOWN :
			keysDown |= downKey;
			break;
	}
}

void specialUp(int key, int xx, int yy)
{
	switch (key)
	{
		case GLUT_KEY_LEFT :
			keysDown &= ~leftKey;
			break;
		case GLUT_KEY_RIGHT :
			keysDown &= ~rightKey;
			break;
		case GLUT_KEY_UP :
			keysDown &= ~upKey;
			break;
		case GLUT_KEY_DOWN :
			keysDown &= ~downKey;
			break;
	}
}