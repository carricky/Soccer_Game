#ifndef _THETA_SLIDER_H
#define _THETA_SLIDER_H

#include <sstream>
#include <string>
#include <iostream>
#include <GL/glut.h>
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"

using namespace std;

class ThetaSlider
{
private:
	float initialAngle;
	bool isMoving;
	bool isIncreasing;

	void drawStrokeString(float value);

public:
	ThetaSlider();

	void draw();
	void update();
	void setInMotion();
	void stopMotion();
	void toggleMotion();

	float getInitialAngle();
};

ThetaSlider::ThetaSlider()
{
	initialAngle = 0;
	isMoving = false;
	isIncreasing = true;
}

void ThetaSlider::setInMotion()
{
	isMoving = true;
}

void ThetaSlider::stopMotion()
{
	isMoving = false;
}

void ThetaSlider::toggleMotion()
{
	isMoving = !isMoving;
}

void ThetaSlider::update()
{
	if (isMoving)
	{
		if (isIncreasing)
			initialAngle += 1;
		else
			initialAngle -= 1;

		if (initialAngle >= 55)
			isIncreasing = false;
		else
		if (initialAngle <= -55)
			isIncreasing = true;
	}
}

void ThetaSlider::drawStrokeString(float value)
{
	stringstream ss;
	ss << value;
	string valueString = "b = " + ss.str();

	for (int i = 0; i<valueString.length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, valueString[i]);
}

void ThetaSlider::draw()
{
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 100, 0);
	glTranslatef(-100, initialAngle / MAXIMUM_ANGLE * 10 - 100, -100);

	glColor3f(1, 1, 0);
	glTranslatef(2, 20, 0);
	glScalef(0.05, 0.05, 0.05);

	drawStrokeString(initialAngle);


	glPopMatrix();
}


float ThetaSlider::getInitialAngle()
{
	return initialAngle;
}



#endif