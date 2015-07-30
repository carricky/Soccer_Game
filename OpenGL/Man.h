#ifndef _MAN_H
#define _MAN_H

#include <GL/glut.h>
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"
#include "Football.h"
#include "ThreeDOBJ.h"
extern bool isCamera;
using namespace std;
extern ThreeDOBJ *temp;
class Man
{
private:
	Football* football;
	Man* goalKeeper;
	RotationAngle rotation;
	float WalkAngle = 0;
	const float soClose = 0.75;
	bool isKeeper;
	bool isMoving;
	bool clockwise ;
public:

	Coordinates coordinates;
	Orientation orientation;

	Man::Man(float x, float y, float z);
	Man(Football *fb, Man *goalKeeper, float x, float y, float z);

	void draw(void);
	void update(float &manMoveX, float &manMoveY, float &manMoveZ);
	void moveBy(float distance, AxisType alongAxis);
	Coordinates getCoordinate();

	bool canMove(float moveDistance, AxisType type);
	bool isGoalKeeper()
	{
		return isKeeper;
	}
	Orientation getOrientation()
	{
		return orientation;
	}
};

Man::Man(Football* fb, Man *goalKeeper, float x, float y, float z)
{
	isKeeper = true;
	this->football = fb;
	this->goalKeeper = goalKeeper;
	coordinates.set(X_AXIS, x);
	coordinates.set(Y_AXIS, y);
	coordinates.set(Z_AXIS, z);
	orientation = Z_POSITIVE;
	clockwise = false;
	isMoving = false;

}
Man::Man(float x, float y, float z)
{
	isKeeper = false;
	coordinates.set(X_AXIS, x);
	coordinates.set(Y_AXIS, y);
	coordinates.set(Z_AXIS, z);
	orientation = Z_POSITIVE;
	clockwise = false;
	isMoving = false;
}

void Man::draw(void)
{

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//������ת���ƶ�
	glTranslatef(coordinates.get(X_AXIS), coordinates.get(Y_AXIS), coordinates.get(Z_AXIS));
	if (orientation == X_POSITIVE)
		glRotatef(90, 0, 1, 0);
	if (orientation == X_NEGATIVE)
		glRotatef(-90, 0, 1, 0);
	if (orientation == Z_NEGATIVE)
		glRotatef(180, 0, 1, 0);


	//glRotatef(rotation.getTheta(X_AXIS), 1, 0, 0);
	//glRotatef(rotation.getTheta(Y_AXIS), 0, 1, 0);
	//glRotatef(rotation.getTheta(Z_AXIS), 0, 0, 1);
	//glTranslatef(-GongZhuanDian[0], -GongZhuanDian[1], -GongZhuanDian[2]);
	//glTranslatef(TranVector[0], TranVector[1], TranVector[2]);

	float speed;
	speed = 110.0/10; //�����˶��ٶ�
	if (isMoving)  //ת������
		WalkAngle = WalkAngle + speed*clockwise - speed*(!clockwise);
	else
		WalkAngle = 0;
	if (WalkAngle>29 || WalkAngle < -29)//������ʮ��֮����ڶ�
		clockwise = !clockwise;


	//����
	//glPushMatrix();

	//glScalef(0.0625, 0.0625, 0.0625); //���ź�:���߿� 1x2x0.5 ����: 16x32x8 ͷ:8x8x8 ����:8x12x4 ��:4x12x4 ��:
	glScalef(0.100, 0.100, 0.100);
	glTranslatef(0, 16, 0);
	glBindTexture(GL_TEXTURE_2D, gTextures[HumanTex]);
	glColor3f(1.0f, 1.0f, 1.0f);
	//����
	glPushMatrix();
	glTranslatef(0, 10, 0);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0.25);
	glVertex3f(-4, 4, 4);
	glTexCoord2f(0.25, 1 - 0.25);
	glVertex3f(4, 4, 4);
	glTexCoord2f(0.25, 1 - 0.5);
	glVertex3f(4, -4, 4);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(-4, -4, 4);
	glEnd();

	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0);
	glVertex3f(-4, 4, -4);
	glTexCoord2f(0.25, 1 - 0);
	glVertex3f(4, 4, -4);
	glTexCoord2f(0.25, 1 - 0.25);
	glVertex3f(4, 4, 4);
	glTexCoord2f(0.125, 1 - 0.25);
	glVertex3f(-4, 4, 4);
	glEnd();

	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.375, 1 - 0.25);
	glVertex3f(4, -4, -4);
	glTexCoord2f(0.25, 1 - 0.25);
	glVertex3f(-4, -4, -4);
	glTexCoord2f(0.25, 1 - 0);
	glVertex3f(-4, -4, 4);
	glTexCoord2f(0.375, 1 - 0);
	glVertex3f(4, -4, 4);
	glEnd();

	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 1 - 0.5);
	glVertex3f(-4, -4, -4);
	glTexCoord2f(0.375, 1 - 0.5);
	glVertex3f(4, -4, -4);
	glTexCoord2f(0.375, 1 - 0.25);
	glVertex3f(4, 4, -4);
	glTexCoord2f(0.5, 1 - 0.25);
	glVertex3f(-4, 4, -4);
	glEnd();

	//�����
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1 - 0.25);
	glVertex3f(-4, 4, -4);
	glTexCoord2f(0.125, 1 - 0.25);
	glVertex3f(-4, 4, 4);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(-4, -4, 4);
	glTexCoord2f(0, 1 - 0.5);
	glVertex3f(-4, -4, -4);
	glEnd();

	//���Ҳ�
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 1 - 0.25);
	glVertex3f(4, 4, 4);
	glTexCoord2f(0.375, 1 - 0.25);
	glVertex3f(4, 4, -4);
	glTexCoord2f(0.375, 1 - 0.5);
	glVertex3f(4, -4, -4);
	glTexCoord2f(0.25, 1 - 0.5);
	glVertex3f(4, -4, 4);
	glEnd();
	glPopMatrix();
	//������
	//��������
	glBegin(GL_QUADS);
	glTexCoord2f(0.3125, 1 - 0.625);
	glVertex3f(-4, 6, 2);
	glTexCoord2f(0.4375, 1 - 0.625);
	glVertex3f(4, 6, 2);
	glTexCoord2f(0.4375, 1 - 1);
	glVertex3f(4, -6, 2);
	glTexCoord2f(0.3125, 1 - 1);
	glVertex3f(-4, -6, 2);
	glEnd();

	//���嶥��
	glBegin(GL_QUADS);
	glTexCoord2f(0.3125, 1 - 0.5);
	glVertex3f(-4, 6, -2);
	glTexCoord2f(0.4375, 1 - 0.5);
	glVertex3f(4, 6, -2);
	glTexCoord2f(0.4375, 1 - 0.625);
	glVertex3f(4, 6, 2);
	glTexCoord2f(0.3125, 1 - 0.625);
	glVertex3f(-4, 6, 2);
	glEnd();

	//�������
	glBegin(GL_QUADS);
	glTexCoord2f(0.5625, 1 - 0.625);
	glVertex3f(4, -6, -2);
	glTexCoord2f(0.4375, 1 - 0.625);
	glVertex3f(-4, -6, -2);
	glTexCoord2f(0.4375, 1 - 0.5);
	glVertex3f(-4, -6, 2);
	glTexCoord2f(0.5625, 1 - 0.5);
	glVertex3f(4, -6, 2);
	glEnd();

	//���屳��
	glBegin(GL_QUADS);
	glTexCoord2f(0.625, 1 - 1);
	glVertex3f(-4, -6, -2);
	glTexCoord2f(0.5, 1 - 1);
	glVertex3f(4, -6, -2);
	glTexCoord2f(0.5, 1 - 0.625);
	glVertex3f(4, 6, -2);
	glTexCoord2f(0.625, 1 - 0.625);
	glVertex3f(-4, 6, -2);
	glEnd();


	//��������
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 1 - 0.625);
	glVertex3f(-4, 6, -2);
	glTexCoord2f(0.3125, 1 - 0.625);
	glVertex3f(-4, 6, 2);
	glTexCoord2f(0.3125, 1 - 1);
	glVertex3f(-4, -6, 2);
	glTexCoord2f(0.25, 1 - 1);
	glVertex3f(-4, -6, -2);
	glEnd();

	//�����Ҳ�
	glBegin(GL_QUADS);
	glTexCoord2f(0.4375, 1 - 0.625);
	glVertex3f(4, 6, 2);
	glTexCoord2f(0.5, 1 - 0.625);
	glVertex3f(4, 6, -2);
	glTexCoord2f(0.5, 1 - 1);
	glVertex3f(4, -6, -2);
	glTexCoord2f(0.4375, 1 - 1);
	glVertex3f(4, -6, 2);
	glEnd();
	//������
	glPushMatrix();
	glTranslatef(6, 0, 0);
	glTranslatef(0, 6, 0);
	glRotatef(WalkAngle, 1, 0, 0);
	glRotatef(0, 1, 0, 0);
	glTranslatef(0, -6, 0);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.75, 1 - 1);
	glVertex3f(2, -6, 2);
	glTexCoord2f(0.6875, 1 - 1);
	glVertex3f(-2, -6, 2);
	glEnd();
	//�ֶ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.6875, 1 - 0.5);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.75, 1 - 0.5);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glEnd();
	//�ֵ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.75, 1 - 0.5);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.8125, 1 - 0.5);
	glVertex3f(2, -6, 2);
	glEnd();
	//�ֱ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.875, 1 - 1);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.8125, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.875, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.625, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.6875, 1 - 1);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.625, 1 - 1);
	glVertex3f(-2, -6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.8125, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.75, 1 - 1);
	glVertex3f(2, -6, 2);
	glEnd();
	glPopMatrix();

	//������
	glPushMatrix();
	glTranslatef(-6, 0, 0);
	glTranslatef(0, 6, 0);
	glRotatef(-WalkAngle, 1, 0, 0);
	glRotatef(0, 1, 0, 0);
	glTranslatef(0, -6, 0);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.6875, 1 - 1);
	glVertex3f(2, -6, 2);
	glTexCoord2f(0.75, 1 - 1);
	glVertex3f(-2, -6, 2);
	glEnd();
	//�ֶ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 1 - 0.5);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.6875, 1 - 0.5);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glEnd();
	//�ֵ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.8125, 1 - 0.5);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.75, 1 - 0.5);
	glVertex3f(2, -6, 2);
	glEnd();
	//�ֱ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.8125, 1 - 1);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.875, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.875, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.8125, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.75, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.75, 1 - 1);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.8125, 1 - 1);
	glVertex3f(-2, -6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.6875, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.625, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.625, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.6875, 1 - 1);
	glVertex3f(2, -6, 2);
	glEnd();
	glPopMatrix();
	//������
	glPushMatrix();
	glTranslatef(-2, -12, 0);
	glTranslatef(0, 6, 0);
	glRotatef(WalkAngle, 1, 0, 0);

	glTranslatef(0, -6, 0);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.125, 1 - 1);
	glVertex3f(2, -6, 2);
	glTexCoord2f(0.0625, 1 - 1);
	glVertex3f(-2, -6, 2);
	glEnd();
	//�ȶ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.0625, 1 - 0.5);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glEnd();
	//�ȵ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.1875, 1 - 0.5);
	glVertex3f(2, -6, 2);
	glEnd();
	//�ȱ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.25, 1 - 1);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.1875, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.25, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.0625, 1 - 1);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0, 1 - 1);
	glVertex3f(-2, -6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.1875, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.125, 1 - 1);
	glVertex3f(2, -6, 2);
	glEnd();
	glPopMatrix();

	//������
	glPushMatrix();
	glTranslatef(2, -12, 0);
	glTranslatef(0, 6, 0);
	glRotatef(-WalkAngle, 1, 0, 0);

	glTranslatef(0, -6, 0);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.0625, 1 - 1);
	glVertex3f(2, -6, 2);
	glTexCoord2f(0.125, 1 - 1);
	glVertex3f(-2, -6, 2);
	glEnd();
	//�ȶ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.0625, 1 - 0.5);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glEnd();
	//�ȵ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.1875, 1 - 0.5);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.125, 1 - 0.5);
	glVertex3f(2, -6, 2);
	glEnd();
	//�ȱ���
	glBegin(GL_QUADS);
	glTexCoord2f(0.1875, 1 - 1);
	glVertex3f(-2, -6, -2);
	glTexCoord2f(0.25, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.25, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.1875, 1 - 0.625);
	glVertex3f(-2, 6, -2);
	glTexCoord2f(0.125, 1 - 0.625);
	glVertex3f(-2, 6, 2);
	glTexCoord2f(0.125, 1 - 1);
	glVertex3f(-2, -6, 2);
	glTexCoord2f(0.1875, 1 - 1);
	glVertex3f(-2, -6, -2);
	glEnd();
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0625, 1 - 0.625);
	glVertex3f(2, 6, 2);
	glTexCoord2f(0, 1 - 0.625);
	glVertex3f(2, 6, -2);
	glTexCoord2f(0, 1 - 1);
	glVertex3f(2, -6, -2);
	glTexCoord2f(0.0625, 1 - 1);
	glVertex3f(2, -6, 2);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	isMoving = false;
}

bool Man::canMove(float moveDistance, AxisType type)
{
	
	float eye[3];
	eye[0] = coordinates.get(X_AXIS);
	eye[2] = coordinates.get(Z_AXIS);
	if (type == X_AXIS)
		eye[0] += moveDistance;
	else
		eye[2] += moveDistance;
	int trees[3][2] = { { 6, 3 }, { 7, 11 }, { -5, 7 } };
	int wall[2] = { WALL_X, WALL_Z };
	int goalPost[2][2] = { { -1.5, -20 }, { 1.5, -20 } };
	bool res = true;
	for (int i = 0; i < 3; i++)
	{
		if (abs(trees[i][0] - eye[0]) <= soClose&&abs(trees[i][1] - eye[2]) <= soClose)
			res = false;
	}
	for (int i = 0; i < 2; i++)
	{
		if (abs(goalPost[i][0] - eye[0]) <= soClose&&abs(goalPost[i][1] - eye[2]) <= soClose)
			res = false;
	}
	if (abs(goalKeeper->coordinates.get(X_AXIS) - eye[0]) <= soClose&&abs(goalKeeper->coordinates.get(Z_AXIS) - eye[2]) <= soClose)
		res = false;
	if (abs(wall[0] - eye[0]) <= soClose+1&&abs(wall[1] - eye[2]) <= soClose)
		res = false;
	if (eye[0] >= 25 || eye[0] <= -23 || eye[2] >= 25 || eye[2] <= -24)
		res = false;
	if (abs(football->coordinates.get(X_AXIS) - eye[0]) <= soClose && abs(football->coordinates.get(Z_AXIS) - eye[2]) <= soClose)
		res = false;
	if (temp != NULL)
	{
		PPP st = temp->Peripheral();
		double sz = temp->scale();
		st.first.first *= sz;
		st.first.second *= sz;
		st.second.first *= sz;
		st.second.second *= sz;
		if (abs((st.first.first - eye[0]) <= 0.5) && (abs(st.second.first - eye[2]+1) <= 4)) return false;
	}
	return res;
}
void Man::update(float &manMoveX, float &manMoveY, float &manMoveZ)
{
	
	if (abs(manMoveX - 0)>0.001)
	{
		if (canMove(manMoveX, X_AXIS))
		{
			isMoving = true;
			moveBy(manMoveX, X_AXIS);
			if (isCamera)
			{
				if (manMoveX > 0)
					orientation = X_POSITIVE;
				else
					orientation = X_NEGATIVE;
			}
		}
	}
	if (abs(manMoveZ - 0)>0.001)
	{
		if (canMove(manMoveZ, Z_AXIS))
		{
			isMoving = true;
			moveBy(manMoveZ, Z_AXIS);
			if (isCamera)
			{
				if (manMoveZ > 0)
					orientation = Z_POSITIVE;
				else
					orientation = Z_NEGATIVE;
			}
		}
	}
	manMoveX = 0;
	manMoveY = 0;
	manMoveZ = 0;
}

void Man::moveBy(float distance, AxisType alongAxis)
{
	coordinates.set(alongAxis, coordinates.get(alongAxis) + distance);
}
Coordinates Man::getCoordinate()
{
	return coordinates;
}
#endif