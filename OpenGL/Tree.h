#ifndef _TREE_H
#define _TREE_H

#include <GL/glut.h>
#include "Drawer.h"
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"

class Tree
{
private:
	Coordinates coordinates;
	RotationAngle rotation;

public:
	Tree(float x, float y, float z);
	void draw(void);

};

Tree::Tree(float x, float y, float z)
{
	coordinates.set(X_AXIS, x);
	coordinates.set(Y_AXIS, y);
	coordinates.set(Z_AXIS, z);

}

void Tree::draw()
{//树高3.4，最长半径1，底面在xy，z轴是中心轴
	//圆锥树
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//物体自转和移动

	glTranslatef(coordinates.get(X_AXIS), coordinates.get(Y_AXIS), coordinates.get(Z_AXIS));
	//glTranslatef(GongZhuanDian[0], GongZhuanDian[1], GongZhuanDian[2]);
	//glRotatef(SelfRotAngle, NormVector[0], NormVector[1], NormVector[2]);
	//glTranslatef(-GongZhuanDian[0], -GongZhuanDian[1], -GongZhuanDian[2]);
	//glTranslatef(TranVector[0], TranVector[1], TranVector[2]);
	glRotatef(-90, 1, 0, 0);//之前的坐标轴设置错了，现在改正
	float args[4] = { 1, 2, 0, 0 }; // 半径和高
	//画3个圆锥
	glPushMatrix();
	glTranslatef(0, 0, 2.2);
	glScalef(0.6, 0.6, 0.6);
	DrawSolidCone(args, LeafTex);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1.6);
	glScalef(0.8, 0.8, 0.8);
	DrawSolidCone(args, LeafTex);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1);
	DrawSolidCone(args, LeafTex);
	glPopMatrix();
	//画树干
	float argswood[4] = { 0.2, 1, 0.2, 10 };
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	DrawCylinder(argswood, TreeTex);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

#endif