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
	FILE *filePtr;	// 文件指针
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	unsigned char	*bitmapImage;		// bitmap图像数据
	int	imageIdx = 0;		// 图像位置索引
	unsigned char	tempRGB;	// 交换变量

	// 以“二进制+读”模式打开文件filename 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL) return NULL;
	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

void TexLoad(int i, char *filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap信息头
	unsigned char*   bitmapData;                                       // 纹理数据

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, gTextures[i]);
	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap层次(通常为，表示最上层) 
		GL_RGB,	//我们希望该纹理有红、绿、蓝数据
		bitmapInfoHeader.biWidth, //纹理宽带，必须是n，若有边框+2 
		bitmapInfoHeader.biHeight, //纹理高度，必须是n，若有边框+2 
		0, //边框(0=无边框, 1=有边框) 
		GL_RGB,	//bitmap数据的格式
		GL_UNSIGNED_BYTE, //每个颜色数据的类型
		bitmapData);	//bitmap数据指针  
}

void InitTexture()
{
	glGenTextures(TexturesNum, gTextures);                                         // 第一参数是需要生成标示符的个数, 第二参数是返回标示符的数组
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

//这里的参数每一个函数都有四个float，但是只有cylinder用到了四个，统一起来是为了函数指针的统一，放进链表的时候格式一致
//画立方体， args第一个参数有效，边长，有连续6个纹理对应六个面，tex是第一个纹理在texture[]里面的下标
void DrawSolidCube(float args[4], GLint tex)
{
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]); //底面
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

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]); //顶面
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

	//glBindTexture(GL_TEXTURE_2D, gTextures[tex ]); //四个侧面
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

//画圆柱/棱柱/棱台， args[]四个参数都有效，第一个是上顶面圆半径，然后是高度，然后是下底面半径，然后是底面的边数
//如果要画圆柱的话，边数至少设20
//纹理只有两个，第一个是底面的纹理，第二个是侧面的纹理
void DrawCylinder(float args[4], int tex)
{
	float radiusTop = args[0];
	float height = args[1];
	float radiusBottom = args[2];
	int stacks = args[3]; //default 20

	float angle, angleNext;
	if (radiusBottom == 0)
		radiusBottom = radiusTop;
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]); //底面和顶面
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

	glBindTexture(GL_TEXTURE_2D, gTextures[tex + 1]); //侧面
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

//画圆锥， args[]两个参数有效，第一个是圆半径，然后是高度
//纹理只有两个，第一个是底面的纹理，第二个是侧面的纹理
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
	int weiduAll = 50; //可更改，如果绘制效率慢的话，但应该没问题，经度条数
	int jingduAll = 50; //可更改，如果绘制效率慢的话，但应该没问题，纬度条数
	float SR = args[0]; //sphereRadius，半径
	glBindTexture(GL_TEXTURE_2D, gTextures[tex]);

	glBegin(GL_QUADS);
	for (int weidu = 0; weidu<weiduAll; weidu++) //从球顶到球底
	for (int jingdu = 0; jingdu<jingduAll; jingdu++)
	{ //0到2pi
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