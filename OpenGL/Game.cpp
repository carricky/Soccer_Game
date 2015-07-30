#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include "stdafx.h"
#include <GL/glut.h>
#include "Camera.h"
#include "FootBall_Field.h"
#include "ScoreBoard.h"
#include "GraphicString.h"
#include "Man.h"
#include<GL/glext.h>
#include "Drawer.h"
#include "ThreeDOBJ.h"
#include "ThetaSlider.h"

using namespace std;

#define FULL_SCREEN 1
extern bool isCamera;
int namelist = 0;
FootBallField fbField;
Camera camera(&fbField);
char* vs[6];
extern ThreeDOBJ *temp;
bool isIntro = true;
GraphicString *graphicString;
GraphicString *gstring;

char FilePath[1024];
char FileName[1024];
typedef pair<double, double> PDD;
typedef pair<PDD, PDD> PPP;


void init (void) 
{
	camera.initPrimitive();

	string pool = "We Present\nFootball Shoot\nPress 'o' to continue...\
		 ";
	graphicString = new GraphicString(pool);
	
	gstring=new GraphicString(tool);
	Coordinates c(-100, 0, 0);
	
	graphicString->append(c);
	Coordinates d(-500,-700,-20);
	gstring->append(d);

	InitTexture();

}


void enable (void) 
{
    glEnable(GL_DEPTH_TEST); //enable the depth testing
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH); //set the shader to smooth shader
}


bool SnapScreen(int width, int height, const char *file)
{
	byte *image;          //接受图像数据
	FILE *fp;            //文件指针
	BITMAPFILEHEADER FileHeader;    //接受位图文件头
	BITMAPINFOHEADER InfoHeader;    //接受位图信息头

	FileHeader.bfType = BITMAP_ID;                                                  //ID设置为位图的id号
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);      //实际图像数据的位置在文件头和信息头之后
	FileHeader.bfReserved1 = 0;                                                    //必须设置为0
	FileHeader.bfReserved2 = 0;                                                    //必须设置为0
	FileHeader.bfSize = height*width * 24 + FileHeader.bfOffBits;                      //BMP图像文件大小
	InfoHeader.biXPelsPerMeter = 0;                                              //水平分辨率，这里暂时设为0就是
	InfoHeader.biYPelsPerMeter = 0;                                              //垂直分辨率，这里暂时设为0就是
	InfoHeader.biClrUsed = 0;                                                    //图像使用的颜色，这里暂时设为0就是
	InfoHeader.biClrImportant = 0;                                                //重要的颜色数，这里暂时设为0就是                        //垂直分辨率，这里暂时设为0就是
	InfoHeader.biPlanes = 1;                //必须设置为1
	InfoHeader.biCompression = BI_RGB;                                              //设置为BI_RGB时,表示图像并没有彩色表
	InfoHeader.biBitCount = 24;                                                    //图像的位数
	InfoHeader.biSize = sizeof(BITMAPINFOHEADER);                                  //结构体的大小	
	InfoHeader.biHeight = height;
	InfoHeader.biWidth = width;
	InfoHeader.biSizeImage = height*width * 4;

	image = (byte *)malloc(sizeof(byte)*InfoHeader.biSizeImage);
	if (image == NULL)
	{
		free(image);
		printf("Exception: No enough space!\n");
		return false;
	}
	//像素格式设置4字节对齐
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//接收出像素的数据
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);

	fp = fopen(file, "wb");
	if (fp == NULL)
	{
		printf("Exception: Fail to open file!\n");
		return false;
	}
	fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(image, InfoHeader.biSizeImage, 1, fp);
	free(image);
	fclose(fp);
	return true;
}
void displayIntro()
{	
	static Coordinates coordinates;

	camera.render();
	glPushMatrix();
	
		glTranslatef(-25, 25, -75);
		glTranslatef(coordinates.get(X_AXIS), coordinates.get(Y_AXIS), coordinates.get(Z_AXIS));
		glScalef(0.05, 0.05, 0.05);
		graphicString->draw();
	gstring->draw();		
	glPopMatrix();

	static float toAppend = 0;
	if(toAppend >= 10)
	{
		graphicString->append();
		gstring->append();
		toAppend = 0;
	}
	toAppend += 1;

	static float projectionAngle = 100.0;

	float alphaRadian = (projectionAngle / 180.0) * 3.1415;		
	float verticalDisp = 75 * sin(alphaRadian);
	float horizontalDisp = cos(alphaRadian);
	projectionAngle +=.1;

	coordinates.set(Y_AXIS, verticalDisp / 10.0);	
	coordinates.set(X_AXIS, horizontalDisp);
	gstring->draw();
}

void display (void) 
{
    glClearColor (1.0,1.0,1.0,0.0); 
	glClearColor (0.0,0.0,0.0,0.0); 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	enable();

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	if(isIntro)
	{
		displayIntro();	
		/*if(graphicString->isPoolNewLine())
			camera.reset();
		camera.slideRight();
		camera.slideRight();
		camera.slideRight();
		camera.rotateAbout(X_AXIS, 0.05);
		camera.rotateAbout(Y_AXIS, -0.05);*/
	}
	else
	{
		camera.render(fbField);
		fbField.drawMovingObjects();
		
		glColor3f(1, 1, 1);

	}

	

	glutSwapBuffers(); // Flush the OpenGL buffers to the window 

	fbField.update();
}

void reshape (int width, int height) 
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window  
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.1, 1000.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes  
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly  

}

void createFootballTrajectory(Orientation orientation)
{
	RotationAngle footballAngularVelocity(-5, -2, -1);
	cout << orientation << endl;
	fbField.getVelocitySlider()->stopMotion();
	fbField.getAngleSlider()->stopMotion();
	fbField.getThetaSlider()->stopMotion();
	fbField.getFootball()->setInMotion(footballAngularVelocity, fbField.getVelocitySlider()->getInitialVelocity(), 2, 
		fbField.getAngleSlider()->getInitialAngle(), fbField.getThetaSlider()->getInitialAngle(), 3, 0.1, orientation);

}

void init3DObj(PDD current_eye)
{
	if (temp != NULL)
	{
		delete temp;		
		puts("");
	}

INPUTFILEPATH:
	printf("Wavefront OBJ file path:\n../3D Objects/");
	scanf("%s", FileName);
	sprintf(FilePath, "../3D Objects/");
	//sprintf(FilePath, "../");
	strcat(FilePath, FileName);
	try
	{
		temp = new ThreeDOBJ(FilePath);
		PPP st = temp->Peripheral();
		double sz = temp->scale();
		st.first.first *= sz;
		st.first.second *= sz;
		st.second.first *= sz;
		st.second.second *= sz;
		if ((st.first.first - 1 <= current_eye.first && st.first.second + 1 >= current_eye.first) && (st.second.first - 1 <= current_eye.second && st.second.second + 1 >= current_eye.second))
		{
			puts("load obj fail, canmera is in the obj");
			delete(temp);
			temp = NULL;
		}
	}
	catch (exception e)
	{
		delete temp;
		puts("File opening failure...\n");
		goto INPUTFILEPATH;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'q':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.lookUp();
		}
		break;
	case 'z':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.lookDown();
		}
		break;
	case 'e':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.rotateReverse();
		}
		break;
	case 'c':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.rotateClockwise();
		}
		break;
	case 'C':
		isCamera = !isCamera;
		break;
	case 'w':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.moveForward();
		}
		break;
	case 's':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.moveBackward();
		}
		break;
	case 'd':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.slideRight();
		}
		break;
	case 'a':
		if (isCamera)
		{
			camera.update(&fbField);
			camera.slideLeft();
		}
		break;
	case ' ':
		if (fbField.kickBall() == 1)
			createFootballTrajectory(fbField.player->orientation);
		break;
	case ',':
		fbField.getVelocitySlider()->toggleMotion();
		break;
	case '.':
		fbField.getAngleSlider()->toggleMotion();
		break;
	case '/':
		fbField.getThetaSlider()->toggleMotion();
		break;
	case 'r':
		fbField.getFootball()->reset();
		break;
	case 'o':
		isIntro = false;
		camera.reset();
		break;

		//light pos
	case '1':
		light_pos[2] -= 0.5;
		break;
	case '4':
		light_pos[2] += 0.5;
		break;
	case '2':
		light_pos[0] -= 0.5;
		break;
	case '3':
		light_pos[0] += 0.5;
		break;
	case 'b':
		night = 1 - night;
		break;
	case 'L':
		if (temp != NULL) temp->change(0, 20);
		break;
	case 'J':
		if (temp != NULL) temp->change(0, -20);
		break;
	case 'K':
		if (temp != NULL) temp->change(1, 20);
		break;
	case 'I':
		if (temp != NULL) temp->change(1, -20);
		break;
	case 'U':
		if (temp != NULL) temp->change(2, -0.001);
		break;
	case 'O':
		if (temp != NULL) temp->change(2, 0.001);
		break;
	case 'j':
		if (isCamera)
		{
			manMoveX -= 0.3;
		}
		else
		{
			Orientation orient = fbField.player->orientation;
			if (orient == Z_POSITIVE)
			{
				fbField.player->orientation = X_POSITIVE;
			}
			else if (orient == Z_NEGATIVE)
			{
				fbField.player->orientation = X_NEGATIVE;
			}
			else if (orient == X_POSITIVE)
			{
				fbField.player->orientation = Z_NEGATIVE;
			}
			else if (orient == X_NEGATIVE)
			{
				fbField.player->orientation = Z_POSITIVE;
			}
		}
		break;
	case '[':
		fbField.md = -0.5;
		break;
	case ']':
		fbField.md = 0.5;
		break;
	case';':
		fbField.changemanCTRL();
		break;
	case 'P':
	{
				char str[300] = "";
				char s1[300] = "";
				char s2[300] = "";
				SYSTEMTIME st = { 0 };
				GetLocalTime(&st);
				sprintf(s2, "Shot%d_%d_%d_%d_%d_%d.bmp",
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond);
				printf("save BMP file path:\n../PrintScreen/\n");
				sprintf(str, "../PrintScreen/");
				strcat(str, s1);
				strcat(str, s2);
				int  W = GetSystemMetrics(SM_CXSCREEN);
				int  H = GetSystemMetrics(SM_CYSCREEN);
				SnapScreen(W, H, str); //need to adjust to current screen
				cout << str << endl;
				cout << "done!" << endl;
				break;
	}
	case 'i':
		if (isCamera)
		{
			manMoveZ -= 0.3;
		}
		else
		{
			Orientation orient = fbField.player->orientation;
			if (orient == Z_POSITIVE)
			{
				manMoveZ += 0.3;
			}
			else if (orient == Z_NEGATIVE)
			{
				manMoveZ -= 0.3;
			}
			else if (orient == X_POSITIVE)
			{
				manMoveX += 0.3;
			}
			else if (orient == X_NEGATIVE)
			{
				manMoveX -= 0.3;
			}
		}
		break;
	case 'k':
		if (isCamera)
		{
			manMoveZ += 0.3;
		}
		else
		{
			Orientation orient = fbField.player->orientation;
			if (orient == Z_POSITIVE)
			{
				manMoveZ -= 0.3;
			}
			else if (orient == Z_NEGATIVE)
			{
				manMoveZ += 0.3;
			}
			else if (orient == X_POSITIVE)
			{
				manMoveX -= 0.3;
			}
			else if (orient == X_NEGATIVE)
			{
				manMoveX += 0.3;
			}
		}
		break;	
	case 'l':
		if (isCamera)
		{
			manMoveX += 0.3;
		}
		else
		{

			Orientation orient = fbField.player->orientation;
			if (orient == Z_POSITIVE)
			{
				fbField.player->orientation = X_NEGATIVE;
			}
			else if (orient == Z_NEGATIVE)
			{
				fbField.player->orientation = X_POSITIVE;
			}
			else if (orient == X_POSITIVE)
			{
				fbField.player->orientation = Z_POSITIVE;
			}
			else if (orient == X_NEGATIVE)
			{
				fbField.player->orientation = Z_NEGATIVE;
			}
		}
		break;
	case GLUT_KEY_F1:
		windFlag = 1 - windFlag;
		break;
	case '=':
		if (shoot == 0)
			fbField.getVelocitySlider()->initialVelocity = 50.0f;
		else
			fbField.getVelocitySlider()->initialVelocity = 10.0f;
		shoot = 1 - shoot;
		break;


	case '8':

		//it is used to write some object into .obj files
		delete(temp);
		temp = NULL;
		break;

	case '9':
		init3DObj(camera.place());
		break;
	case '0':
	{
				if (temp != NULL && temp->isTiger())
				{
					namelist = (namelist + 1) % 6;
					delete(temp);
					temp = NULL;
					sprintf(FilePath, "../3D Objects/");
					//sprintf(FilePath, "../");
					strcat(FilePath, vs[namelist]);
					try
					{
						temp = new ThreeDOBJ(FilePath);
						PPP st = temp->Peripheral();
						cout << st.first.first << " " << st.first.second << " " << st.second.first << " " << st.second.second << endl;
					}
					catch (exception e)
					{
						delete temp;
						puts("File opening failure...\n");
					}
				}
				break;
	}

	case 27:
		exit(0);
	}
}


int main(int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);	
	vs[0] = ("tiger.obj");
	vs[1] = ("tiger_handup.obj");
	vs[2] = ("tiger_headdown.obj");
	vs[3] = ("tiger_headright.obj");
	vs[4] = ("tiger_headup.obj");
	vs[5] = ("tiger_mouseopen.obj");
	if(FULL_SCREEN)
	{
		glutGameModeString("1920x1080:32@75"); //the settings for fullscreen mode
		glutEnterGameMode(); //set glut to fullscreen using the settings in the line above
	}
	else
	{
		glutInitWindowSize (500, 500);
		glutInitWindowPosition (100, 100);
		glutCreateWindow ("Football Shoot");
	}


	init();

    glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
    glutMainLoop ();

}