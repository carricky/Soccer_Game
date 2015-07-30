#ifndef _CAMERA_H
#define _CAMERA_H

#include <iostream>
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"
#include "FootBall_Field.h"
#include "ThreeDOBJ.h"

extern ThreeDOBJ *temp = NULL;
extern bool isCamera;
using namespace std;

//This class keeps track of the human player's point of view and display.

class Camera
{
private:
	FootBallField* field;

	//Indices of stationery objects in the display list
	GLuint displayListIndex;
	GLuint sunDisplayListIndex;
	GLuint skyDisplayListIndex;
	GLuint groundDisplayListIndex;

	float eye[3];
	float center[3];
	const float pace = 0.25;
	const float anglePace = 10;
	const float soClose = 1;
	float rotate = 0;

public:
	Camera(FootBallField *fb);
	Camera(Coordinates coord, RotationAngle angle);
	PDD place()
	{
		return PDD(eye[0], eye[2]);
	}
	void update(FootBallField *fb);

	void moveForward();
	void moveBackward();
	void slideLeft();
	void slideRight();

	void initPrimitive();						//Goalkeeper's Image
	void render(FootBallField footballField);
	void render();

	void rotateClockwise();
	void rotateReverse();
	void lookUp();
	void lookDown();

	void reset();

	friend ostream& operator<<(ostream& out, Camera &camera);

	GLfloat* getEye(){ return eye; }
	GLfloat* getCenter(){ return center; }

	bool canMove();
	float getRadian();
	float getRadian2();
};

Camera::Camera(FootBallField *fb)
{
	this->field = fb;

	eye[0] = eye[2] = 0;
	eye[1] = 0;
	center[0] = center[1] = 0;
	center[2] = -1;
}

Camera::Camera(Coordinates coord, RotationAngle angle)
{
}

void Camera::lookUp()
{
	center[1] += pace;
}

void Camera::lookDown()
{
	center[1] -= pace;
}

void Camera::rotateReverse()
{
	rotate -= anglePace;
	if (rotate < 0)
		rotate += 360;

}

void Camera::rotateClockwise()
{
	rotate += anglePace;

	if (rotate > 360)
		rotate -= 360;
}

float Camera::getRadian()
{
	float now = 90 - rotate;
	float radian = now*PI / 180;

	return radian;
}
float Camera::getRadian2()
{
	float now = 180 - rotate;
	float radian = now*PI / 180;

	return radian;
}
void Camera::moveForward()
{
	float radian = getRadian();
	float addX = cos(radian);
	float addZ = -sin(radian);
	eye[0] += pace*addX;
	eye[2] += pace*addZ;

	if (!canMove())
	{
		eye[0] -= pace*addX;
		eye[2] -= pace*addZ;
	}
}

void Camera::moveBackward()
{
	float radian = getRadian();
	float addX = cos(radian);
	float addZ = -sin(radian);
	eye[0] -= pace*addX;
	eye[2] -= pace*addZ;
	if (!canMove())
	{
		eye[0] += pace*addX;
		eye[2] += pace*addZ;
	}
}

void Camera::slideLeft()
{
	float radian = getRadian();
	float addX = sin(radian);
	float addZ = cos(radian);
	eye[0] -= pace*addX;
	eye[2] -= pace*addZ;
	if (!canMove())
	{
		eye[0] += pace*addX;
		eye[2] += pace*addZ;
	}
}

void Camera::slideRight()
{
	float radian = getRadian();
	float addX = sin(radian);
	float addZ = cos(radian);
	eye[0] += pace*addX;
	eye[2] += pace*addZ;
	if (!canMove())
	{
		eye[0] -= pace*addX;
		eye[2] -= pace*addZ;
	}
}

bool Camera::canMove()
{
	cout << eye[0] << " " << eye[1] << " " << eye[2] << endl;
	int trees[3][2] = { { 6, 3 }, { 7,11 }, { -5, 7 } };
	int man[2] = { 0, -10 };
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
	if (abs(field->man->coordinates.get(X_AXIS) - eye[0]) <= soClose&&abs(field->man->coordinates.get(Z_AXIS) - eye[2]) <= soClose)
		res = false;
	if (abs(field->player->coordinates.get(X_AXIS) - eye[0]) <= soClose&&abs(field->player->coordinates.get(Z_AXIS) - eye[2]) <= soClose)
		res = false;
	if (abs(wall[0] - eye[0]) <= soClose&&abs(wall[1] - eye[2]) <= soClose)
		res = false;
	if (eye[0] >= 25 || eye[0] <= -24 || eye[2] >= 25 || eye[2] <= -24)
		res = false;
	if (abs(field->getFootball()->coordinates.get(X_AXIS) - eye[0]) <= soClose && abs(field->getFootball()->coordinates.get(Z_AXIS) - eye[2]) <= soClose)
		res = false;
	if (temp != NULL)
	{
		PPP st = temp->Peripheral();
		double sz = temp->scale();
		st.first.first *= sz;
		st.first.second *= sz;
		st.second.first *= sz;
		st.second.second *= sz;
		if ((st.first.first - 1 <= eye[0] && st.first.second + 1 >= eye[0]) && (st.second.first - 1 <= eye[2] && st.second.second + 1 >= eye[2])) return false;
	}

	return res;
}

void Camera::initPrimitive()
{
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	//glEnable(GL_LIGHTING); //enable the lighting
	//glEnable(GL_LIGHT0); //enable our Diffuse Light
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH); //set the shader to smooth shader


	//Creating a cube for testing
	displayListIndex = glGenLists(1); //set the cube list to Generate a List
	glNewList(displayListIndex, GL_COMPILE); //compile the new list
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(0.5, 20, 20); //draw the cube
	glPopMatrix();
	glEndList(); //end the list	
}

void Camera::render(FootBallField footballField)
{

	float now = 90 - rotate;
	float radian = now*PI / 180;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	glLoadIdentity();
	if (isCamera)
	{
		gluLookAt(eye[0], eye[1], eye[2],
			eye[0] + cos(radian), center[1], eye[2] - sin(radian),
			0, 1, 0);
	}
	else
	{
		int eyeOfPlayer[3] = { 0 };
		int face[3] = { 0 };
		Orientation orient = (footballField.player)->orientation;
		eyeOfPlayer[0] = (footballField.player)->getCoordinate().get(X_AXIS);
		eyeOfPlayer[2] = (footballField.player)->getCoordinate().get(Z_AXIS);
		if (orient == X_POSITIVE)
		{
			face[0] = 1;
		}
		if (orient == X_NEGATIVE)
		{
			face[0] = -1;
		}
		if (orient == Z_POSITIVE)
		{
			face[2] = 1;
		}
		if (orient == Z_NEGATIVE)
		{
			face[2] = -1;
		}
		eyeOfPlayer[0] += face[0];
		eyeOfPlayer[2] += face[2];
		gluLookAt(eyeOfPlayer[0], eyeOfPlayer[1], eyeOfPlayer[2],
			eyeOfPlayer[0] + face[0], eyeOfPlayer[1] + face[1], eyeOfPlayer[2] + face[2],
			0, 1, 0);

	}
	footballField.drawStationeryObjects();			//Render the sun, sky and ground which appear to be stationery wrt the camera

	//glTranslatef(0, 0, -distanceFromCam);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glCallList(displayListIndex);




}

void Camera::update(FootBallField *fb)
{
	this->field = fb;
}

void Camera::render()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	//glCallList(displayListIndex);



}

void Camera::reset()
{
}


#endif