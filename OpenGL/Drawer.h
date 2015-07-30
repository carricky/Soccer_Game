#ifndef _DRAWER_H
#define _DRAWER_H

#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include "Constants.h"

#define PI 3.1415926
#define BITMAP_ID 0x4D42

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char	*bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

	// �ԡ�������+����ģʽ���ļ�filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}

void TexLoad(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char*   bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, gTextures[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��  
}

void InitTexture()
{
	glGenTextures(TexturesNum, gTextures);                                         // ��һ��������Ҫ���ɱ�ʾ���ĸ���, �ڶ������Ƿ��ر�ʾ��������
	TexLoad(HumanTex, "textures/man.bmp");
	TexLoad(BlockTex, "textures/block.bmp");
	TexLoad(TreeTex, "textures/tree_top.bmp");
	TexLoad(TreeTex + 1, "textures/tree_side.bmp");

	TexLoad(LeafTex, "textures/leaf.bmp");
	TexLoad(LeafTex + 1, "textures/leaf.bmp");
	TexLoad(EarthTex, "textures/sky.bmp");
	TexLoad(GrassTex, "textures/grass3.bmp");
	TexLoad(BallTex, "textures/ball.bmp"); 
	TexLoad(SunTex, "textures/sun.bmp");
	TexLoad(MoonTex, "textures/moon.bmp");
	TexLoad(MoonTex, "textures/moon.bmp");
	TexLoad(TigerTex, "textures/tiger-atlas2.bmp");
	TexLoad(Block2Tex, "textures/block3.bmp");
}

//����Ĳ���ÿһ�����������ĸ�float������ֻ��cylinder�õ����ĸ���ͳһ������Ϊ�˺���ָ���ͳһ���Ž������ʱ���ʽһ��
//�������壬 args��һ��������Ч���߳���������6�������Ӧ�����棬tex�ǵ�һ��������texture[]������±�
void DrawSolidCube(float args[4], GLint tex)
{
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]); //����
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0, 1);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2i(1, 1);
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2i(1, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]); //����
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2i(0, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2i(1, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1, 0);
	glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]); //�ĸ�����
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2i(0, 1);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2i(1, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1, 0);
	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0, 1);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2i(1, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2i(1, 0);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2i(0, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2i(1, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1, 0);
	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0, 1);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2i(1, 1);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2i(1, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
}

//��Բ��/����/��̨�� args[]�ĸ���������Ч����һ�����϶���Բ�뾶��Ȼ���Ǹ߶ȣ�Ȼ�����µ���뾶��Ȼ���ǵ���ı���
//���Ҫ��Բ���Ļ�������������20
//����ֻ����������һ���ǵ���������ڶ����ǲ��������
void DrawCylinder(float args[4], int tex)
{
	float radiusTop = args[0];
	float height = args[1];
	float radiusBottom = args[2];
	int stacks = args[3]; //default 20

	float angle, angleNext;
	if (radiusBottom == 0)
		radiusBottom = radiusTop;
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]); //����Ͷ���
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < stacks; i++)
	{
		angle = 360.0 / stacks*i / 180 * PI;
		angleNext = 360.0 / stacks*(i + 1) / 180 * PI;
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, height / 2);
		glTexCoord2f(0.5 + 0.5*cos(angle), 0.5 + 0.5*sin(angle));
		glVertex3f(radiusTop*cos(angle), radiusTop*sin(angle), height / 2);
		glTexCoord2f(0.5 + 0.5*cos(angleNext), 0.5 + 0.5*sin(angleNext));
		glVertex3f(radiusTop*cos(angleNext), radiusTop*sin(angleNext), height / 2);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, -height / 2);
		glTexCoord2f(0.5 + 0.5*cos(angle), 0.5 + 0.5*sin(angle));
		glVertex3f(radiusBottom*cos(angle), radiusBottom*sin(angle), -height / 2);
		glTexCoord2f(0.5 + 0.5*cos(angleNext), 0.5 + 0.5*sin(angleNext));
		glVertex3f(radiusBottom*cos(angleNext), radiusBottom*sin(angleNext), -height / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, gTextures[tex + 1]); //����
	glBegin(GL_QUADS);
	for (int i = 0; i < stacks; i++)
	{
		angle = 360.0 / stacks*i / 180 * PI;
		angleNext = 360.0 / stacks*(i + 1) / 180 * PI;
		glTexCoord2f(float(i) / stacks, 1);
		glVertex3f(radiusTop*cos(angle), radiusTop*sin(angle), height / 2);
		glTexCoord2f(float(i + 1) / stacks, 1);
		glVertex3f(radiusTop*cos(angleNext), radiusTop*sin(angleNext), height / 2);
		glTexCoord2f(float(i + 1) / stacks, 0);
		glVertex3f(radiusBottom*cos(angleNext), radiusBottom*sin(angleNext), -height / 2);
		glTexCoord2f(float(i) / stacks, 0);
		glVertex3f(radiusBottom*cos(angle), radiusBottom*sin(angle), -height / 2);
	}
	glEnd();
}

//��Բ׶�� args[]����������Ч����һ����Բ�뾶��Ȼ���Ǹ߶�
//����ֻ����������һ���ǵ���������ڶ����ǲ��������
void DrawSolidCone(float args[4], int tex)
{
	float radius = args[0];
	float height = args[1];
	int stacks = 20;
	float angle, angleNext;
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < stacks; i++)
	{
		angle = 360.0 / stacks*i / 180 * PI;
		angleNext = 360.0 / stacks*(i + 1) / 180 * PI;
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);
		glTexCoord2f(0.5 + 0.5*cos(angle), 0.5 + 0.5*sin(angle));
		glVertex3f(radius*cos(angle), radius*sin(angle), 0);
		glTexCoord2f(0.5 + 0.5*cos(angleNext), 0.5 + 0.5*sin(angleNext));
		glVertex3f(radius*cos(angleNext), radius*sin(angleNext), 0);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, gTextures[tex + 1]);

	glBegin(GL_QUADS);
	for (int i = 0; i < stacks; i++)
	{
		angle = 360.0 / stacks*i / 180 * PI;
		angleNext = 360.0 / stacks*(i + 1) / 180 * PI;
		glTexCoord2f(float(i) / stacks, 1);
		glVertex3f(0, 0, height);
		glTexCoord2f(float(i + 1) / stacks, 1);
		glVertex3f(0, 0, height);
		glTexCoord2f(float(i + 1) / stacks, 0);
		glVertex3f(radius*cos(angleNext), radius*sin(angleNext), 0);
		glTexCoord2f(float(i) / stacks, 0);
		glVertex3f(radius*cos(angle), radius*sin(angle), 0);
	}
	glEnd();
}

void DrawSolidSphere(float args[4], int tex)
{
	int weiduAll = 50; //�ɸ��ģ��������Ч�����Ļ�����Ӧ��û���⣬��������
	int jingduAll = 50; //�ɸ��ģ��������Ч�����Ļ�����Ӧ��û���⣬γ������
	float SR = args[0]; //sphereRadius���뾶
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]);

	glBegin(GL_QUADS);
	for (int weidu = 0; weidu<weiduAll; weidu++) //���򶥵����
	for (int jingdu = 0; jingdu<jingduAll; jingdu++)
	{ //0��2pi
		//calculate the vertexs of quad
		float x[4], y[4], z, zNext, R, RNext, angle, angleNext;
		z = SR - weidu*2.0*SR / weiduAll;
		zNext = SR - (weidu + 1)*2.0*SR / weiduAll;
		R = sqrt(SR*SR - z*z);
		RNext = sqrt(SR*SR - zNext*zNext);
		angle = 360.0 / jingduAll*jingdu / 180 * PI;
		angleNext = 360.0 / jingduAll*(jingdu + 1) / 180 * PI;

		x[0] = R*cos(angle);
		y[0] = R*sin(angle);
		x[1] = R*cos(angleNext);
		y[1] = R*sin(angleNext);
		x[2] = RNext*cos(angleNext);
		y[2] = RNext*sin(angleNext);
		x[3] = RNext*cos(angle);
		y[3] = RNext*sin(angle);

		//draw quad
		//set points
		glTexCoord2f(float(jingdu) / jingduAll, 1 - float(weidu) / weiduAll);
		glVertex3f(x[0], y[0], z);
		glTexCoord2f(float(jingdu + 1) / jingduAll, 1 - float(weidu) / weiduAll);
		glVertex3f(x[1], y[1], z);
		glTexCoord2f(float(jingdu + 1) / jingduAll, 1 - float(weidu + 1) / weiduAll);
		glVertex3f(x[2], y[2], zNext);
		glTexCoord2f(float(jingdu) / jingduAll, 1 - float(weidu + 1) / weiduAll);
		glVertex3f(x[3], y[3], zNext);
	}
	glEnd();
}
#endif