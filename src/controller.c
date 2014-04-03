#include "controller.h"

#include <math.h>

#include "animation.h"
#include "game.h"
#include "action.h"
#include "util.h"
#include "item.h"

long keysDown;
long upKey = 1 << 0;
long leftKey = 1 << 1;
long downKey = 1 << 2;
long rightKey = 1 << 3;
long aKey = 1 << 4;
long dKey = 1 << 5;
long wKey = 1 << 6;
long mKey = 1 << 7;

struct Action* move_act;
struct ActionObject* move_angle;

struct Creature* controlled;

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

void switchItems(int dir)
{
	if (dir == 1 && controlled->items->next != NULL)
	{
		struct ItemElement* newActive = controlled->items->next;
		
		struct ItemElement* last;
		struct ItemElement* currElem = controlled->items;
		while (currElem != NULL)
		{
			last = currElem;
			currElem = currElem->next;
		}
		last->next = controlled->items;
		controlled->items->next = NULL;
		controlled->items = newActive;
	}
}

void updateController(float dt)
{
	action_add_subtype(move_act, SACT_DISABLED);
	if (keysDown & leftKey)
		goLeft(dt);
	if (keysDown & rightKey)
		goRight(dt);
	if (keysDown & aKey)
		goLeft(dt*0.2f);
	if (keysDown & dKey)
		goRight(dt*0.2f);
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

void controlObject(struct Creature *go)
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
	
	game_add_action(&(mainGame->acts), move_act);
}

void pickUp()
{
	//(struct ItemElement* items, struct Creature* creat, struct Item* last, float dist, struct ItemElement** rtn)
	struct ItemElement* nearItem;
	int itemsNear = creature_get_near(mainGame->items, controlled, NULL, 2.0f, &nearItem);
	if (itemsNear > 0)
	{
		item_remove_sub_type(nearItem->elem, SITM_IN_WORLD);
		items_add_item(&(controlled->items), nearItem->elem);
		items_remove_item(&(mainGame->items), nearItem->elem);
	}
}

void controlCamera()
{
	// Set the camera
	GLfloat up[3] = {0.0f, 1.0f, 0.0f};
	if (keysDown & mKey)
	{
		up[0] = sin(controlled->angle);
		up[1] = 0.0f;
		up[2] = -cos(controlled->angle);
	}
	
	float height = -1.0f;
	if (keysDown & mKey)
		height = -150.0f;
	
	GLfloat f[3] = {sin(controlled->angle), 0.0f, -cos(controlled->angle)};
	if (keysDown & mKey)
	{
		f[0] = 0.0f;
		f[1] = -1.0f;
		f[2] = 0.0f;
	}
	
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
	glTranslated(-controlled->pos.x*CREATURE_WIDTH*2, height, -controlled->pos.y*CREATURE_HEIGHT*2);
	
	//gluLookAt(	x, 1.0f, z,
	//		x+lx, 1.0f,  z+lz,
	//		0.0f, 1.0f,  0.0f);
}
void normalUp(unsigned char key, int x, int y)
{
	if (key == 'w')
		pickUp();
	if (key == 'm')
		keysDown &= ~mKey;
	if (key == 'e')
		switchItems(1);
	if (key == 'q')
		switchItems(-1);
	if (key == 'a')
		keysDown &= ~aKey;
	if (key == 'd')
		keysDown &= ~dKey;
	if (key == 27)
		exit(0);
	if (key == 32)
		punching = 0;
}

void normalDown(unsigned char key, int x, int y)
{
	if (key == 'a')
		keysDown |= aKey;
	if (key == 'd')
		keysDown |= dKey;
	if (key == 'm')
		keysDown |= mKey;
	if (key == 32)
	{
		if (controlled->items->elem->anim == NULL)
		{
			item_use(controlled, controlled->items->elem);
		}
	}
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
