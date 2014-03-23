#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#define TEX_NOTHING 0
#define TEX_STONE_WALL 1
#define TEX_GASEOUS_BALL_1 2
#define TEX_GASEOUS_BALL_2 3
#define TEX_FIST 4
#define TEX_PUNCH 5
#define TEX_BLUE_JELLY_1 6
#define TEX_BLUE_JELLY_2 7
#define TEX_DEAD_JELLY 8

GLuint swTexture;
GLuint gbTexture;
GLuint gb2Texture;
GLuint handTexture;
GLuint punchTexture;
GLuint bJelly1Texture;
GLuint bJelly2Texture;
GLuint dJellyTexture;

void load_texture(int i);

#endif
