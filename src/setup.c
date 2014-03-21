#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include<math.h>

#include "c_image.h"
#include "levelGenerator.h"
#include "tile.h"
#include "creature.h"
#include "vec_math.h"
#include "controller.h"
#include "animation.h"
#include "HUD.h"
#include "textures.h"
#include "game.h"

GLuint sProgram;

AnimationInstance animInst;

void changeSize(int w, int h)
{
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

float fraction = 0.1f;

float time = 0;

void renderScene(void)
{
	float dt=glutGet(GLUT_ELAPSED_TIME)-time;
	time=glutGet(GLUT_ELAPSED_TIME);
	dt /= 1000.0f;
	
	game_loop(mainGame, dt);
	
	// this helps with alpha
	// texures still need to be pre sorted
	glEnable( GL_BLEND );
	glClearColor(0.0,0.0,0.0,0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	updateController(dt);
	
	controlCamera();
	
	glEnable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	{
		glBindTexture(GL_TEXTURE_2D, swTexture);
		glBegin(GL_QUADS);
		int i = 0;
		while (i < NUM_TILES)
		{
			tile_draw(&(mainGame->tiles[i]));
			++i;
		}
		glEnd();
	}
	{
		step_animation(&animInst, dt);
		load_texture(animInst.currentFrame->texture);
		float degs = getCameraAngle();
		//
		//printf("angle: %f\n", degs);
		int i = 0;
		while (i < NUM_CREATURES)
		{
			if (creature_check_sub_type(&(mainGame->creatures[i]), STYP_EXISTS) == 0)
				creature_draw(&(mainGame->creatures[i]), degs);
			++i;
		}
	}
	HUD_draw(mainGame, dt);
	
	
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		printf("GL ERROR: %s\n", gluErrorString(err));

	glutSwapBuffers();
}


const GLchar *vert_shader =
	"#version 130\n\
	out vec2 fragmentTexCoord;\n\
    void main(void) {\n\
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n\
		fragmentTexCoord = gl_MultiTexCoord0.xy;\n\
    }";

const GLchar *frag_shader =
    "#version 130\n\
	uniform sampler2D texture0;\n\
	in vec2 fragmentTexCoord;\n\
	void main() {\n\
		gl_FragColor = texture2D(texture0, fragmentTexCoord);\
    }";
	//	gl_FragColor = vec4(texture2D(texture0, fragmentTexCoord).x, 0.0, 0.0, 0.5);\
	
void makeTexture(cImage *cimg, GLuint *tex)
{
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, cimg->bytes_per_pixel, cimg->width, cimg->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cimg->pixel_data);
}

void dispose()
{
	// this should free all the tiles and their animations.
	// also the player
	//free(player);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	mainGame = malloc(sizeof(*mainGame));
	game_init(mainGame);
	controller_init();
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("rhm");
	
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
	glutKeyboardFunc(normalDown);
	glutKeyboardUpFunc(normalUp);
	glutSpecialFunc(specialDown);
	glutSpecialUpFunc(specialUp);
	
	
	GLenum err = glewInit();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vert_shader, NULL);
	glShaderSource(fShader, 1, &frag_shader, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	
	int error = glGetError();
	GLchar infoLog[1024];
	GLsizei size;
	glGetShaderInfoLog(vShader, 1024, &size, infoLog);
	//printf("vert shader: %s \n", infoLog);
	glGetShaderInfoLog(fShader, 1024, &size, infoLog);
	//printf("frag shader: %s \n", infoLog);
	//printf("shader compile error: %s \n", gluErrorString(error));
	
	sProgram = glCreateProgram();
	glAttachShader(sProgram, vShader);
	glAttachShader(sProgram, fShader);
	glLinkProgram(sProgram);
	glUseProgram(sProgram);
	
	makeTexture(&stoneWall, &swTexture);
	makeTexture(&gaseousBall, &gbTexture);
	makeTexture(&gaseousBall2, &gb2Texture);
	makeTexture(&hand, &handTexture);
	makeTexture(&punchFist, &punchTexture);
	
	/*
	int i = 0;
	while (i < NUM_CREATURES)
	{
		game->creatures[i].pos.x = 2;
		game->creatures[i].pos.y = 4;
		printf("tile: x: %f y: %f\n", game->creatures[i].pos.x, game->creatures[i].pos.y);
		++i;
	}*/
	
	
	/*
	new_animation(&anim);
	add_frame(&anim, TEX_GASEOUS_BALL_1, 1.0f);
	add_frame(&anim, TEX_GASEOUS_BALL_2, 1.0f);*/
	setup_animations();
	set_instance(anm_gas_ball, &animInst);
	
	glutMainLoop();
	
	return 1;
}