#ifndef _WALL_H
#define _WALL_H

#include <GL/glut.h>
#include "Drawer.h"
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"

class Wall
{
private:
	Coordinates coordinates;
	RotationAngle rotation;

public:
	Wall(float x, float y, float z);
	void draw(void);

};

Wall::Wall(float x, float y, float z)
{
	coordinates.set(X_AXIS, x);
	coordinates.set(Y_AXIS, y);
	coordinates.set(Z_AXIS, z);

}

void Wall::draw(void)
{

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//物体自转和移动
	glTranslatef(coordinates.get(X_AXIS), coordinates.get(Y_AXIS), coordinates.get(Z_AXIS));
	//glRotatef(SelfRotAngle, NormVector[0], NormVector[1], NormVector[2]);
	//glTranslatef(-GongZhuanDian[0], -GongZhuanDian[1], -GongZhuanDian[2]);
	//glTranslatef(TranVector[0], TranVector[1], TranVector[2]);

	glRotatef(-90, 1, 0, 0);//之前的坐标轴设置错了，现在改正
	float args[4];
	glPushMatrix();
	//glTranslatef(1.5, 0, 0);
	glScalef(4, 0.2, 2);
	glTranslatef(0, 0, 0.5);
		DrawSolidCube(args, Block2Tex);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

#endif