#ifndef _FOOTBALL_H
#define _FOOTBALL_H

#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <Windows.h>
#include "Drawer.h"
#include "Coordinates.h"
#include "RotationAngle.h"
#include "Constants.h"
#include "ScoreBoard.h"
#include "Wind.h"
#pragma comment(lib, "winmm.lib")

using namespace std;
//This class keeps track of the Football's movements and detects collisions and controls the physics of the ball

class Football
{
private:

	Coordinates currentOrigin;
	RotationAngle rotation;
	bool isMoving;

	Coordinates velocity;
	RotationAngle angularVelocity;

	float initialVelocity;
	float rateOfDecreaseOfVelocity;
	float projectionAngle;
	float leftrightAngle;
	float rateOfDecreaseOfProjAngle;
	float rateOfChangeOfTime;
	Orientation orientation;
	int bounceCount;
	ScoreBoard *scoreBoard;
	Wind *wind;
	
	float time;

	void rotateBy(float angle, AxisType aboutAxis);

public:
	Coordinates coordinates;
	Football(float x, float y, float z, ScoreBoard *s);
	void draw(void);
	void setInMotion(RotationAngle omega, float v0, float dv, float alpha, float theta, float da, float dt, Orientation orientation);
	void stopMotion();

	void update(double xx, double yy);
	void reset();

	void moveBy(float distance, AxisType alongAxis);

};

Football::Football(float x, float y, float z, ScoreBoard *s)
{
	coordinates.set(X_AXIS, x);
	coordinates.set(Y_AXIS, y);
	coordinates.set(Z_AXIS, z);
	currentOrigin = Coordinates(coordinates);
	isMoving = false;
	scoreBoard = s;
	bounceCount = 0;
	time = 0;
	wind = new Wind(10, 0.02, true);
}

void Football::setInMotion(RotationAngle omega, float v0, float dv, float alpha, float theta, float da, float dt, Orientation orientation)
{
	isMoving = true;
	angularVelocity = omega;

	initialVelocity = v0;
	rateOfDecreaseOfVelocity = dv;
	projectionAngle = alpha;

	leftrightAngle = theta;

	rateOfDecreaseOfProjAngle = da;
	rateOfChangeOfTime = dt;
	this->orientation = orientation;
}

void Football::stopMotion()
{
	isMoving = false;
}

void Football::moveBy(float distance, AxisType alongAxis)
{
	coordinates.set(alongAxis, coordinates.get(alongAxis) + distance);
}

void Football::rotateBy(float angle, AxisType aboutAxis)
{
	rotation.setTheta(aboutAxis, rotation.getTheta(aboutAxis) + angle);
}

void Football::update(double xx, double yy)
{
	if (coordinates.get(X_AXIS) >= 25)
		orientation = X_NEGATIVE;
	else if (coordinates.get(X_AXIS) <= -23)
		orientation = X_POSITIVE;
	else if (coordinates.get(Z_AXIS) >= 25)
		orientation = Z_NEGATIVE;
	else if (coordinates.get(Z_AXIS) <= -24)
		orientation = Z_POSITIVE;
	
	if (initialVelocity < 0 || projectionAngle < 0)
	{
		currentOrigin = Coordinates(coordinates);
		isMoving = false;
		bounceCount = 0;
	}
	if(isMoving && initialVelocity > 0)
	{
		if (coordinates.get(Y_AXIS) < INITIAL_BALL_Y)
		{
			
			initialVelocity -= 2;
			if ((4 - bounceCount)>0)
				projectionAngle -= (4 -  bounceCount);
			else
				projectionAngle -= 1;
			currentOrigin.set(Z_AXIS, coordinates.get(Z_AXIS));
			cout << "Initial V = " << initialVelocity << "\tAlpha = " << projectionAngle << "\tTheta = " << leftrightAngle << endl;
			cout << "Current Origin = " << currentOrigin << endl;
			time = 0;
			//currentOrigin.set(Y_AXIS, MINIMUM_HEIGHT);
			coordinates.set(Y_AXIS, INITIAL_BALL_Y);
			bounceCount++;
			cout << "Bounce!!!!" << endl;
			return;
		}

		if(coordinates.get(Z_AXIS) <= MAXIMUM_DEPTH)
		{
			isMoving = false;
			
			LPCWSTR str;


			//Check if ball is within the bounding box of goalpost
			if (coordinates.get(X_AXIS) <= 8.0 && coordinates.get(X_AXIS) >= -6 && coordinates.get(Y_AXIS) <= 3 )
			{
				str = TEXT("Media/APPLAUSE.WAV");
				PlaySound(str, NULL, SND_ASYNC);
				scoreBoard->setNumOfGoals(scoreBoard->getNumOfGoals() + 1);
			}
			else
			{
				str = TEXT("Media/BOMB.WAV");
				PlaySound(str, NULL, SND_ASYNC);
			}

			coordinates.set(X_AXIS, INITIAL_BALL_X);
			coordinates.set(Y_AXIS, INITIAL_BALL_Y);
			coordinates.set(Z_AXIS, INITIAL_BALL_Z);

			currentOrigin.set(X_AXIS, INITIAL_BALL_X);
			currentOrigin.set(Y_AXIS, INITIAL_BALL_Y);
			currentOrigin.set(Z_AXIS, INITIAL_BALL_Z);

			time = 0;
			wind->reset();
			return;
		}
		double xs = coordinates.get(X_AXIS);
		double ys = coordinates.get(Z_AXIS);
		if ((xs - xx)*(xs - xx) + (yy - ys)*(yy - ys) < 2)
		{
			orientation = (Orientation)(13 - (int)orientation);
			leftrightAngle = -leftrightAngle;
		}
		float alphaRadian = projectionAngle / 180.0 * acos(-1);
		float betaRadian = leftrightAngle / 180.0 * acos(-1);				
		float verticalDisp = initialVelocity * time * sin(alphaRadian) - 0.5 * 9.8 * time * time; //z axis
		float horizontalDisp = (initialVelocity * time * cos(alphaRadian)); //facing axis
		
		cout << "x = " << coordinates.get(X_AXIS) << "y = " << coordinates.get(Y_AXIS) << "z = " << coordinates.get(Z_AXIS) << endl;

		
		if (orientation == Z_POSITIVE)
		{
			coordinates.set(X_AXIS, coordinates.get(X_AXIS) + (windFlag == 1?wind->getSpeed():0) - (horizontalDisp / 100.0) * sin(betaRadian));
			coordinates.set(Y_AXIS, verticalDisp / 100.0 - 1);
			coordinates.set(Z_AXIS, coordinates.get(Z_AXIS) + (horizontalDisp / 100.0) * cos(betaRadian));
		}
		if (orientation == Z_NEGATIVE)
		{
			coordinates.set(X_AXIS, coordinates.get(X_AXIS) + (windFlag == 1?wind->getSpeed():0) + (horizontalDisp / 100.0) * sin(betaRadian));
			coordinates.set(Y_AXIS, verticalDisp / 100.0 - 1);
			coordinates.set(Z_AXIS, coordinates.get(Z_AXIS) - (horizontalDisp / 100.0) * cos(betaRadian));
		}
		if (orientation == X_NEGATIVE)
		{
			cout << "Fuck" << endl;
			coordinates.set(X_AXIS, coordinates.get(X_AXIS) + (windFlag == 1 ? wind->getSpeed() : 0) - (horizontalDisp / 100.0) * cos(betaRadian));
			coordinates.set(Y_AXIS, verticalDisp / 100.0 - 1);
			coordinates.set(Z_AXIS, coordinates.get(Z_AXIS) - (horizontalDisp / 100.0) * sin(betaRadian));
		}
		if (orientation == X_POSITIVE)
		{
			coordinates.set(X_AXIS, coordinates.get(X_AXIS) + (windFlag == 1 ? wind->getSpeed() : 0) + (horizontalDisp / 100.0) * cos(betaRadian));
			coordinates.set(Y_AXIS, verticalDisp / 100.0 - 1);
			coordinates.set(Z_AXIS, coordinates.get(Z_AXIS) + (horizontalDisp / 100.0) * sin(betaRadian));
		}


		rotateBy(angularVelocity.getTheta(X_AXIS), X_AXIS);
		rotateBy(angularVelocity.getTheta(Y_AXIS), Y_AXIS);
		rotateBy(angularVelocity.getTheta(Z_AXIS), Z_AXIS);

		time += 0.2;
		
		wind->update();

		//cout << "Time = " << time << endl;
	}
}

void Football::reset()
{
	coordinates.set(X_AXIS, INITIAL_BALL_X);
	coordinates.set(Y_AXIS, INITIAL_BALL_Y);
	coordinates.set(Z_AXIS, INITIAL_BALL_Z);
	bounceCount = 0;
	currentOrigin = Coordinates(coordinates);
	time = 0;
	isMoving = false;
	wind->reset();
}


void Football::draw()
{
	glColor3f(1.0,1.0,0.0);
	//glTranslatef(0,-1,0.0);
	glPushMatrix();
		glTranslatef(coordinates.get(X_AXIS), coordinates.get(Y_AXIS), coordinates.get(Z_AXIS));

		glRotatef(rotation.getTheta(X_AXIS), 1, 0, 0);
		glRotatef(rotation.getTheta(Y_AXIS), 0, 1, 0);
		glRotatef(rotation.getTheta(Z_AXIS), 0, 0, 1);

		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		float args[4] = {0.3,0,0,0};
		DrawSolidSphere(args, BallTex);
		//glutSolidSphere(0.59, 12, 12);
		//glColor3f(0, 0, 0);
		//glutWireSphere(0.6, 12, 12);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

#endif