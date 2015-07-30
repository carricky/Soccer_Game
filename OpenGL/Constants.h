#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <GL/glut.h>
enum AxisType
{
	X_AXIS = 0,
	Y_AXIS,
	Z_AXIS
};

enum Orientation
{
	X_POSITIVE = 5,	
	Z_POSITIVE,
	Z_NEGATIVE,
	X_NEGATIVE
};

static const float MINIMUM_VELOCITY = 10;
static const float MAXIMUM_VELOCITY = 120;
static const float MAXIMUM_ANGLE = 90;

static const float MAXIMUM_DEPTH = -20;

static const float MINIMUM_HEIGHT = -1;

//initial ball position
static const float INITIAL_BALL_X = 0;
static const float INITIAL_BALL_Y = -1.5;
static const float INITIAL_BALL_Z = 3;
static const float WALL_X = -7;
static const float WALL_Z = 0;
static const float whiteWeak[4] = { 0.2f, 0.2f, 0.2f, 1.0f };


const GLsizei TexturesNum{ 13 };
GLuint gTextures[TexturesNum];

GLint HumanTex = 0;
GLint BlockTex = 1;
GLint TreeTex = 2;
GLint LeafTex = 4;
GLint EarthTex = 6;
GLint GrassTex = 7;
GLint BallTex = 8;
GLint SunTex = 9;
GLint MoonTex = 10;
GLint TigerTex = 11;
GLint Block2Tex = 12;
GLint night = 0;
GLint windFlag = 0;
GLfloat light_pos[] = { -3, 2, -8, 1 };
GLfloat moveDistance = 0.4;
GLfloat manMoveX = 0;
GLfloat manMoveZ = 0;
GLfloat manMoveY = 0;
bool isCamera = true;
GLint shoot = 0;
#endif