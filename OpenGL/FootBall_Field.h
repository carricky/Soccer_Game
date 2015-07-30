#ifndef _FOOTBALL_FIELD_H
#define _FOOTBALL_FIELD_H

#include "Football.h"
#include "GoalKeeper.h"
#include "ScoreBoard.h"
#include "HorizontalSlider.h"
#include "VerticalSlider.h"
#include "Man.h"
#include "Wall.h"
#include "Tree.h"
#include "ThreeDOBJ.h"
#include "ThetaSlider.h"
string tool="\nDeveloped by -\n ZJU team";
//This class keeps track of all the objects in the Game World, including the Football, and textures of the field, etc.
const int SUN_PARAM = 1;
int countD = 0;
const float SUN_SHINE_PARAM[] = { 0.35, 0.37, 0.39, 0.41, 0.43, 0.44, 0.42, 0.39, 0.37 };
const double pi = acos(-1);
#define RADIAN(x) (PI*(x)/180)

class FootBallField
{
private:
	Football * football;

	Wall * wall;
	Tree * tree;
	Tree * tree2;
	Tree * tree3;	
	int trees[10][2];
	ScoreBoard *scoreBoard;
	HorizontalSlider *initialVelocitySlider;
	VerticalSlider *initialAngleSlider;
	ThetaSlider *initialThetaSlider;
	bool manCTRL;
	//Indices of stationery objects in the display list
	GLuint sunDisplayListIndex;
	GLuint moonDisplayListIndex;
	GLuint skyDisplayListIndex;
	GLuint groundDisplayListIndex;
	GLuint goalPostDisplayListIndex;

	GLuint boundaryDisplayListIndex;
	//Methods for Stationery Objects with respect to the Camera
	void initGround(void);
	void initBoundary(void);
	void initSky(void);
	void initSun(void);
	void initMoon(void);
	void drawGroundAndSky(void);
	void drawGround(void);
	void drawSun(void);	
	void BallBounce();
	//Methods for Moving Objects with respect to the Camera	
	void initGoalPost(void);
	void drawGoalPost(void);
	void drawMarkings(void);	


public:
	Man * player;
	Man * man;
	FootBallField();
	void drawStationeryObjects(void);
	void drawMovingObjects();
	void drawLoadedObj(const ThreeDOBJ &a);
	double md;
	//Required to update relative motion of any object in the football field
	void update();

	Football *getFootball();
	HorizontalSlider *getVelocitySlider();
	VerticalSlider *getAngleSlider();
	ThetaSlider *getThetaSlider();
	bool getmanCTRL()
	{
		return manCTRL;
	}
	void changemanCTRL()
	{
		manCTRL = 1 - manCTRL;
	}
	int kickBall();
	~FootBallField();
};
inline double length(PDD a)
{
	return sqrt((a.first*a.first) + (a.second*a.second));
}
void FootBallField::BallBounce()
{
	vector<PPP> vs;
	double x, y;//current place;
	for (int i = 0; i < vs.size(); i++)
	{
		PDD a;// the direction of the ball		
		double x1 = vs[i].first.first;
		double x2 = vs[i].second.first;
		double y1 = vs[i].first.second;
		double y2 = vs[i].second.second;
		double A = y2 - y1;
		double B = x1 - x2;
		double C = -y2 * (x1 - x2) + x2*(y1 - y2);
		double res = A * x + B * y + C;
		if (fabs(res) > 0.1) continue;
		PDD b = PDD(x2 - x1, y2 - y1); // the mirror
		double theta = acos((a.first*b.first + a.second*b.second) / length(a) / length(b));
		double hand = a.first * b.second - a.second * b.first; //nega right, posi left
		if ((theta * 2 < pi && hand < 0) || (theta * 2 >= pi && hand >= 0)) // clockwise rotate
		{
			if (theta * 2 > pi) theta = pi - theta;
			theta *= 2;
			double tmpx = a.first;
			double tmpy = a.second;
			a.first = cos(theta) * tmpx + sin(theta) * tmpy;
			a.second = -sin(theta) * tmpx + cos(theta) * tmpy;
		}
		else
		{
			if (theta * 2 > pi) theta = pi - theta;
			theta *= 2;
			double tmpx = a.first;
			double tmpy = a.second;
			a.first = cos(theta) * tmpx - sin(theta) * tmpy;
			a.second = sin(theta) * tmpx + cos(theta) * tmpy;
		}
		break;	
	}
}
FootBallField::FootBallField()
{
	trees[0][0] = 2;
	trees[0][1] = 3;
	trees[1][0] = 7;
	trees[1][1] = 11;
	trees[2][0] = -5;
	trees[2][1] = 7;
	this->manCTRL = 0;
	
	this->md = 0;
	this->scoreBoard = new ScoreBoard(2.0, 5.0);
	this->football = new Football(INITIAL_BALL_X, INITIAL_BALL_Y, INITIAL_BALL_Z, scoreBoard);	
	this->man = new Man(0, -2, -19);
	this->player = new Man(football, man, 0, -2, 5);
	this->wall = new Wall(WALL_X, -1, WALL_Z);
	this->tree = new Tree(6, -1, 3);
	this->tree2 = new Tree(trees[1][0], -1, trees[1][1]);
	this->tree3 = new Tree(trees[2][0], -1, trees[2][1]);

	this->initialVelocitySlider = new HorizontalSlider(2);
	this->initialAngleSlider = new VerticalSlider();
	this->initialThetaSlider = new ThetaSlider();
	//this->goalkeeper = new GoalKeeper();

	sunDisplayListIndex = -1;
	moonDisplayListIndex = -1;
	skyDisplayListIndex = -1;
	groundDisplayListIndex= -1;
	goalPostDisplayListIndex = -1;

	boundaryDisplayListIndex = -1;
}
void FootBallField::drawStationeryObjects()
{

	//drawGroundAndSky();
	scoreBoard->draw();
	//scoreBoard->drawScore("SCORE: ", 100,100,100);

	//drawMarkings();
	//drawGoalPost();
	//goalPost.draw();
	//scoreBoard->draw();
	//goalKeeper.draw();

	initialVelocitySlider->draw();
	initialAngleSlider->draw();
	initialThetaSlider->draw();


}

void FootBallField::drawMovingObjects()
{
	//glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, (1,1,1,1.0));
	drawSun();
	football->draw();
	drawGoalPost();	
	
	if (!manCTRL)
	{
		if ((man->getCoordinate().get(X_AXIS) <= 8) && (man->getCoordinate().get(X_AXIS) >= -6))
			man->moveBy(moveDistance, X_AXIS);
		else
		{
			moveDistance = -moveDistance;
			man->moveBy(moveDistance, X_AXIS);
			cout << man->getCoordinate().get(X_AXIS);
		}
	}
	else
	{
		if ((man->getCoordinate().get(X_AXIS) + this->md <= 8) && (man->getCoordinate().get(X_AXIS) + this->md>= -6))
			man->moveBy(this->md, X_AXIS);
		this->md = 0;
	}
	

	man->draw();

	player->update(manMoveX, manMoveY, manMoveZ);
	player->draw();
	wall->draw();
	tree->draw();
	tree2->draw();
	tree3->draw();
	drawGroundAndSky();
	extern ThreeDOBJ *temp;
	if (temp != NULL)
	{		
		temp->drawObject();		
	}
}

void FootBallField::update()
{
	football->update(man->getCoordinate().get(X_AXIS), man->getCoordinate().get(Z_AXIS));
	initialVelocitySlider->update();
	initialAngleSlider->update();
	initialThetaSlider->update();
}

Football *FootBallField::getFootball()
{
	return football;
}


HorizontalSlider *FootBallField::getVelocitySlider()
{
	return initialVelocitySlider;
}

VerticalSlider *FootBallField::getAngleSlider()
{
	return initialAngleSlider;
}
ThetaSlider *FootBallField::getThetaSlider()
{
	return initialThetaSlider;
}



void FootBallField::drawMarkings(void)
{
	glColor3f(0,0,0);
	
	glBegin(GL_LINE_LOOP);
	glVertex3f(-1.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, 0.0f, 0.0f); 
	glVertex3f(1.5f, -2.0f, 0.0f); 
	glVertex3f(-1.5f, -2.0f, 0.0f);
	glEnd();
	glColor3f(1,1,1);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(0.0,-1,0);
	glEnd();

}

void FootBallField::initGoalPost(void)
{
	//高度为1， 长度3， 厚度0.5
	glPointSize(4.0);
	glLineWidth(3.0);
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	goalPostDisplayListIndex = glGenLists(1);
	glNewList(goalPostDisplayListIndex,GL_COMPILE); //compile the new list
		glPushMatrix();
			glPushMatrix();
			//glEnable(GL_TEXTURE_2D);
				//side post
				float args[4] = { 0.04, 1, 0.04, 20 };
				glRotatef(-90, 1, 0, 0);
				glTranslatef(-1.5f, 0.0f, 0.5f);
				glColor3f(1.0f, 1.0f, 1.0f);
				DrawCylinder(args, TreeTex);
				glTranslatef(3.0f, 0.0f, 0.0f);
				DrawCylinder(args, TreeTex);

				//top post
				float args2[4] = { 0.04, 3.05f, 0.04, 20 };
				glTranslatef(-1.5f, 0.0f, 0.5f);
				glRotatef(-90, 0, 1, 0);
				DrawCylinder(args2, TreeTex);

				//side skew post
				glRotatef(90, 0, 1, 0);
				glRotatef(26, 1, 0, 0);
				float args3[4] = { 0.02, 1.2f, 0.02, 20 };
				glTranslatef(-1.5f, 0.0f, -0.6f);
				DrawCylinder(args3, TreeTex);
				glTranslatef(3.0f, 0.0f, 0.0f);
				DrawCylinder(args3, TreeTex);

			//glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			glColor3f(1.0f,1.0f,1.0f);
			glLineWidth(1.0);
			
			//draw the net
			for(float i = 1.0, j=-1.5; i>0 && j<=1.5; i-=0.05,j+=0.15)
			{
				glBegin(GL_LINES);
				glVertex3f(1.5f,i, (i-1.0)/2);
				glVertex3f(-1.5f,i, (i-1.0)/2);
				glVertex3f(j,0.0f,-0.5f);
				glVertex3f(j,1.0f,0.0f);
				glEnd();
			}
			glRotatef(-90, 1, 0, 0);
			
			for (float i = 0.0; i <=0.5; i += 0.025)
			{
				glBegin(GL_LINES);
				glVertex3f(1.5f, 0, i*2);
				glVertex3f(1.5f, 0.5-i, i*2);
				glVertex3f(-1.5f, 0, i * 2);
				glVertex3f(-1.5f, 0.5-i, i * 2);
				glEnd();
			}

			for (float i = 0.0; i <= 0.5; i += 0.05)
			{
				glBegin(GL_LINES);
				glVertex3f(1.5f, i, 0);
				glVertex3f(1.5f, i, 1-i*2);
				glVertex3f(-1.5f, i, 0);
				glVertex3f(-1.5f, i, 1 - i * 2);
				glEnd();
			}

		glPopMatrix();
    glEndList(); //end the list	
	glPopAttrib();
}

void FootBallField::drawGoalPost(void)
{
	//These two statements have to be included here...cannot put them in the Constructor because of the SHITTY state machine
	if(goalPostDisplayListIndex == -1)
		initGoalPost();
		
	glPushMatrix();
		glTranslated(1.5, -2, -20); 
		glScalef(5, 5, 5);
		glCallList(goalPostDisplayListIndex);
	glPopMatrix();

}

void FootBallField::initSun()
{

	extern GLfloat light_pos[];
	//Putting the sun into the display list
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	sunDisplayListIndex = glGenLists(1);
	glNewList(sunDisplayListIndex, GL_COMPILE); //compile the new list
	/*	glPushMatrix();
	glColor3f(1, 1, 0);
	glutSolidSphere(0.5,65,15);
	glPopMatrix();*/

	float mat_diffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	float mat_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	float args[4] = { SUN_PARAM ,0,0,0};
	//DrawSolidSphere(args, SunTex);
	//glutSolidSphere(SUN_PARAM, 10, 10);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i<10; i++)
	for (int j = 0; j<10; j++)
	{
		GLfloat transX, transY, transZ;
		GLfloat angleX, angleY;
		glPushMatrix();
		angleX = 81 - (i * 18);
		angleY = 18 + 36 * j;

		GLfloat tmp = SUN_PARAM*cos(RADIAN(angleX));
		transX = tmp*sin(RADIAN(angleY));
		transZ = tmp*cos(RADIAN(angleY));
		transY = sqrt(SUN_PARAM*SUN_PARAM - transX*transX - transZ*transZ);
		if (i>4)
			transY = -transY;
		glTranslatef(transX, transY, transZ);
		glTranslatef(-5, 5, 0);
		glRotatef(angleY, 0, 1, 0);
		glRotatef((90 - angleX), 1, 0, 0);
		glRotatef(-90, 1, 0, 0);
		glColor3f(1.0, 1.0, 0.0);
		glutSolidCone(sin(RADIAN(90 - angleX))*1.1*SUN_SHINE_PARAM[(countD / 13) % 9] * SUN_PARAM, SUN_SHINE_PARAM[(countD / 13) % 9] * 0.8*SUN_PARAM, 10, 10);

		glPopMatrix();
	}
	glEndList(); //end the list	
	glPopAttrib();
}

void FootBallField::initMoon()
{
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialf(GL_FRONT_AND_BACK, GL_EMISSION, (1.0f,1.0f,1.0f));
	extern GLfloat light_pos[];
	//Putting the sun into the display list
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	moonDisplayListIndex = glGenLists(1);
	glNewList(moonDisplayListIndex, GL_COMPILE); //compile the new list
	/*	glPushMatrix();
	glColor3f(1, 1, 0);
	glutSolidSphere(0.5,65,15);
	glPopMatrix();*/
	
	float mat_diffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	float mat_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	float self_emit[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, self_emit);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
	glTranslatef(0, 5, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotatef(120, 1, 0, 0);
	float args[4] = { SUN_PARAM, 0, 0, 0 };
	DrawSolidSphere(args, MoonTex);
	//glutSolidSphere(SUN_PARAM, 10, 10);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glEndList(); //end the list	
	glPopAttrib();
}

void FootBallField::drawSun()
{
	//These two statements have to be included here...cannot put them in the Constructor because of the SHITTY state machine
	//if (sunDisplayListIndex == -1)
		initSun();
		if (moonDisplayListIndex == -1)
			initMoon();
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_position[] = { -10.0, 8.0, -10.0, 0.0 };
	//GLfloat light_color[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };

	glEnable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT1, GL_AMBIENT, whiteWeak);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0f);
	glEnable(GL_LIGHTING);
	if (night == 0)
	{
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		
	}

	glPushMatrix();
	//glTranslatef(0, 0, -5);
	glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
	//glTranslatef(-10, 8, -10);
	glCallList(night == 0?sunDisplayListIndex:moonDisplayListIndex);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	countD += 1;
}

void FootBallField::initSky()
{
	//Putting the sky in a display list
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	skyDisplayListIndex = glGenLists(1);
	glNewList(skyDisplayListIndex, GL_COMPILE); //compile the new list
		glPushMatrix();
		float args[4] = { 500 };
		//glRotatef(-100, 1, 0, 0);
		DrawSolidSphere(args, EarthTex);
		glPopMatrix();
	glEndList(); //end the list	
	glPopAttrib();
}

void FootBallField::initGround()
{
	//Putting the ground in a display list
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	groundDisplayListIndex = glGenLists(1);
	glNewList(groundDisplayListIndex,GL_COMPILE); //compile the new list
		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		//glRotatef(-90, 1, 0, 0);
		glScalef(1, 0.2, 1);
		float args[4];
		//SolidCube(10);
		DrawSolidCube(args, GrassTex);
		glPopMatrix();
    glEndList(); //end the list	
	glPopAttrib();
}
void FootBallField::initBoundary()
{
	//Putting the ground in a display list
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	boundaryDisplayListIndex = glGenLists(1);
	glNewList(boundaryDisplayListIndex, GL_COMPILE); //compile the new list
	glPushMatrix();
	//glRotatef(-90, 1, 0, 0);
	//glRotatef(-90, 1, 0, 0);
	glScalef(1, 0.2, 1);
	float args[4];
	//SolidCube(10);
	DrawSolidCube(args, BlockTex);
	glPopMatrix();
	glEndList(); //end the list	
	glPopAttrib();
}

void FootBallField::drawGroundAndSky()
{
	if ((groundDisplayListIndex == -1) || (skyDisplayListIndex == -1))
	{
		initSky();
		initGround();
		initBoundary();
	}

	//draw sky
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	float args[4] = { 500 };
	glRotatef(-90, 1, 0, 0);
	glCallList(skyDisplayListIndex);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//draw ground
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glTranslatef(-5, -5, 0);
	glTranslatef(-25, -25, -2);
	for (int i = 0; i < 50; i++)
	{

		for (int j = 0; j < 50; j++)
		{
			//glRotatef(-90, 1, 0, 0);
			glTranslatef(1, 0, 0);
			glCallList(groundDisplayListIndex);
			if (i == 0)
			{
				glTranslatef(0, -0.5, 2);
				glScalef(1, 1, 5);
				glCallList(boundaryDisplayListIndex);
				glScalef(1, 1, 0.2);
				glTranslatef(0, 0.5, -2);
			}
			if (i == 49)
			{
				glTranslatef(0, 0.5, 2);
				glScalef(1, 1, 5);
				glCallList(boundaryDisplayListIndex);
				glScalef(1, 1, 0.2);
				glTranslatef(0, -0.5, -2);
			}
			if (j == 0)
			{
				glTranslatef(-0.5, 0, 2);
				glRotatef(90, 0, 0, 1);
				glScalef(1, 1, 5);
				glCallList(boundaryDisplayListIndex);
				glScalef(1, 1, 0.2);
				glRotatef(-90, 0, 0, 1);
				glTranslatef(0.5, 0, -2);
			}
			if (j == 49)
			{
				glTranslatef(0.5, 0, 2);
				glRotatef(90, 0, 0, 1);
				glScalef(1, 1, 5);
				glCallList(boundaryDisplayListIndex);
				glScalef(1, 1, 0.2);
				glRotatef(-90, 0, 0, 1);
				glTranslatef(-0.5, 0, -2);
			}
			

		}
		glTranslatef(-50, 1, 0);
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

int FootBallField::kickBall()
{
	if (player->orientation == Z_NEGATIVE)
	{
		if (abs(football->coordinates.get(X_AXIS) - player->coordinates.get(X_AXIS)) < 1 &&
			abs(football->coordinates.get(Z_AXIS) - player->coordinates.get(Z_AXIS) + 1) < 1)
			return 1;
	}
	else 
	if (player->orientation == Z_POSITIVE)
	{
		if (abs(football->coordinates.get(X_AXIS) - player->coordinates.get(X_AXIS)) < 1 &&
			abs(football->coordinates.get(Z_AXIS) - player->coordinates.get(Z_AXIS) - 1) < 1)
			return 1;
	}
	else
	if (player->orientation == X_NEGATIVE)
	{
		cout << abs(football->coordinates.get(X_AXIS) - player->coordinates.get(X_AXIS) + 1) << endl;
		cout << abs(football->coordinates.get(Z_AXIS) - player->coordinates.get(Z_AXIS)) << endl;
		if (abs(football->coordinates.get(X_AXIS) - player->coordinates.get(X_AXIS) + 1) <= 1 &&
			abs(football->coordinates.get(Z_AXIS) - player->coordinates.get(Z_AXIS)) <= 1)
		{
			return 1;
		}
	}
	else
	if (player->orientation == X_POSITIVE)
	{
		if (abs(football->coordinates.get(X_AXIS) - player->coordinates.get(X_AXIS)-1) <= 1 &&
			abs(football->coordinates.get(Z_AXIS) - player->coordinates.get(Z_AXIS) - 1) <= 1)
			return 1;
	}
	else
		return 0;
}

FootBallField::~FootBallField()
{
	//delete scoreBoard;
}
#endif