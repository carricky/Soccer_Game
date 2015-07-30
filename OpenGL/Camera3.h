#ifndef _CAMERA_H
#define _CAMERA_H

#include <iostream>
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"
#include "FootBall_Field.h"

using namespace std;

//This class keeps track of the human player's point of view and display.

class Camera
{
private:
	Coordinates coordinates;
	RotationAngle rotation;
	float distanceFromCam;

	//Indices of stationery objects in the display list
	GLuint displayListIndex;
	GLuint sunDisplayListIndex;
	GLuint skyDisplayListIndex;
	GLuint groundDisplayListIndex;
	GLfloat eye[3] = { 0, -1, 0 };
	GLfloat center[3] = { 0, 0, -1};
	const GLfloat pace = 0.5;
	const GLfloat anglePace = 10;
	
public:
	Camera();
	Camera(Coordinates coord, RotationAngle angle);

	void moveForward();
	void moveBackward();
	void slideLeft();
	void slideRight();


	void initPrimitive();						//Goalkeeper's Image
	void render(FootBallField footballField);
	void render();

	void reset();

	void rotateClockwise();
	void rotateReverse();
	void lookUp();
	void lookDown();

	GLfloat rotate = 0;
	friend ostream& operator<<(ostream& out, Camera &camera);

	GLfloat* getEye(){return eye; }
	GLfloat* getCenter(){ return center; }
	
};

Camera::Camera()
{
	Coordinates coord;
	coordinates = coord;

	RotationAngle angle;
	rotation = angle;

	distanceFromCam = 5;
}

Camera::Camera(Coordinates coord, RotationAngle angle)
{
	coordinates = coord;
	rotation = angle;

	distanceFromCam = 10;
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

void Camera::moveForward()
{
	eye[2] -= pace;
}

void Camera::moveBackward()
{
	eye[2] += pace;
}

void Camera::slideLeft()
{
	eye[0] -= pace;
}

void Camera::slideRight()
{
	eye[0] += pace;
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
	glNewList(displayListIndex,GL_COMPILE); //compile the new list
		glColor3f(0.5, 0.5, 0.5);
		glutSolidSphere(0.5, 20, 20); //draw the cube
		glPopMatrix();
    glEndList(); //end the list	
}

void Camera::render(FootBallField footballField)
{	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	glLoadIdentity();

	footballField.drawStationeryObjects();			//Render the sun, sky and ground which appear to be stationery wrt the camera

	glTranslatef(0, 0, -distanceFromCam);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glCallList(displayListIndex);
   
}

void Camera::render()
{
	glTranslatef(0, 0, -distanceFromCam);

	glColor3f(1.0f, 0.0f, 0.0f);
	//glCallList(displayListIndex);


	

void Camera::reset()
{
	Coordinates c;
	coordinates = c;
	RotationAngle r;
	rotation = r;
}

ostream & operator<<(ostream& out, Camera &camera)
{
    cout << "Coordinates = " << camera.coordinates << "\tAngle = " << camera.rotation;
    return out;
}

#endif