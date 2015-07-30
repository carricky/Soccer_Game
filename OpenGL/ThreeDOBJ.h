#ifndef _THREEDOBJ_H
#define _THREEDOBJ_H

#include "opencv\cv.h"
#include <map>
#include<set>
#include<vector>
#include "opencv\highgui.h"
using namespace cv;
using namespace std;
#define MAXLINESIZE 1024
#define MAXFILENAMESIZE 256
#define MAXFILEPATHSIZE 512

char gao(char c)
{
	if (c >= 'a') return c;
	else return c  + 'a' - 'A';
}
typedef pair<double, double> PDD;
typedef pair<PDD, PDD> PPP;
struct Vertexs
{
	float x;
	float y;
	float z;
};

//laplacian coordinates(same as vertex coordinates)
typedef Vertexs LaplacianVal;

//texture coordinates
struct TexCoord
{
	float u;
	float v;
};
class ThreeDOBJ
{
public:
	ThreeDOBJ()
	{
		vertexAmt = texCoordAmt = faceAmt = groupAmt = 0;
	}
	~ThreeDOBJ()
	{
		delete[] vertex;
		delete[] texCoord;
		delete[] vRenderIndex;
		delete[] tRenderIndex;		
		delete[] gStartRIndex;
	}
	ThreeDOBJ(const char* path);
	void drawObject();
	PPP Peripheral()
	{
		return PPP(PDD(Peri.first.first + move[0], Peri.first.second + move[0]), PDD(Peri.second.first + move[1], Peri.second.second + move[1]));
	}
	double scale()
	{
		return move[2];
	}
	int get()
	{
		return vertexAmt;
	}
	void change(int x, double y)
	{
		if (x == 2 && move[x] + y < 0) return;
		move[x] += y;
	}
	bool isTiger() 
	{
		return istiger;
	}
private:
	PPP Peri;
	int vertexAmt;		//total vertex amount
	int texCoordAmt;	//total texture coordinate amount
	int faceAmt;		//total face amount
	int groupAmt;		//total group amount
	double move[3];
	bool istiger;
	bool wire;
	Vertexs *vertex;		//vertex coordinates
	TexCoord *texCoord;	//texture coordinates
	int *vRenderIndex;	//vertex rendering indices
	int *tRenderIndex;	//texture rendering indices
	int *gStartRIndex;	//range of rendering indices of groups 
	char mtlFileName[MAXFILENAMESIZE];	//mtl file name
	Mat texImg;	//texture image
	void load(const char *path);
	char ObjFilePath[MAXFILENAMESIZE];	
};
void ThreeDOBJ::drawObject(void)
{
	glPushMatrix();
	glScaled(move[2], move[2], move[2]);
	glTranslatef(move[0], 0, move[1]);
	//draw wire or textured object
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double zoom = 600;
	bool showSF = false;
	bool isMark = true;
	int curGNum = 0;
	wire = false;
	glBindTexture(GL_TEXTURE_2D, gTextures[TigerTex]);	
	glBegin(GL_TRIANGLES);
	{
		for (int groupI = 0; groupI<groupAmt; groupI++)
		{
			//if this group is chosen, then make it twinkle
			if (groupI == curGNum && isMark )
			{

				//glTexCoord2f(0.0f, 0.0f);
				

				for (int i = gStartRIndex[groupI]; i< gStartRIndex[groupI + 1]; i++)
				{
					glTexCoord2fv((float*)(texCoord + tRenderIndex[i]));
					glVertex3fv((float*)(vertex + vRenderIndex[i]));


				}
				
			}
			else
			{
				//glTexCoord2f(0.0f, 0.0f);
				for (int i = gStartRIndex[groupI];	i< gStartRIndex[groupI + 1]; i++)
				{

						glTexCoord2fv((float*)(texCoord + tRenderIndex[i]));



					glVertex3fv((float*)(vertex + vRenderIndex[i]));
				}
			}
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}
ThreeDOBJ::ThreeDOBJ(const char *path)
{
	load(path);
}
void ThreeDOBJ::load(const char *path)
{
	FILE* objFile = fopen(path, "r");	
	char lineBuf[MAXLINESIZE];

	//first pass: count v,vt,f,g, get mtl file name
	char tempFileName[MAXFILENAMESIZE] = "";
	vertexAmt = texCoordAmt = faceAmt = groupAmt = 0;
	move[0] = move[1] = 0;
	move[2] = 0.01;
	while (!feof(objFile))
	{
		fgets(lineBuf, MAXLINESIZE, objFile);
		switch (lineBuf[0])
		{
		case 'v':
			switch (lineBuf[1])
			{
			case ' ':
				vertexAmt++;
				break;

			case 't':
				texCoordAmt++;
				break;

			default:
				break;
			}
			break;

		case 'f':
			faceAmt++;
			break;

		case 'g':
			groupAmt++;
			break;

		case 'm':
			sscanf(lineBuf, "mtllib %s", tempFileName);
			break;

		default:
			break;
		}
	}
	if (gao(path[14]) == 't' && gao(path[15]) == 'i' && gao(path[16]) == 'g' && gao(path[17]) == 'e' && gao(path[18]) == 'r') istiger = true; else istiger = false;
	//if it has mtl file, then save the filename
	char tempFilePath[MAXFILEPATHSIZE];
	if (tempFileName[0] != '\0')
	{
		strcpy(mtlFileName, tempFileName);
		strcpy(tempFilePath, path);
		sprintf(strrchr(tempFilePath, '/') + 1, "%s", tempFileName);
	}
	else
	{
		mtlFileName[0] = '\0';
	}
	vertex = new Vertexs[vertexAmt + 1];
	texCoord = new TexCoord[texCoordAmt + 1];
	vRenderIndex = new int[faceAmt * 3];
	tRenderIndex = new int[faceAmt * 3];
	gStartRIndex = new int[groupAmt + 1];

	//second pass: get v,vt,f,g data
	rewind(objFile);
	fgets(lineBuf, MAXLINESIZE, objFile);
	while (lineBuf[0] != 'v')
	{
		fgets(lineBuf, MAXLINESIZE, objFile);
	}

	sscanf(lineBuf, "v %f %f %f\n", &vertex[1].x, &vertex[1].y, &vertex[1].z);
	Peri = PPP(PDD(vertex[1].x, vertex[1].y), PDD(vertex[1].y, vertex[1].y));
	for (int i = 2; i <= vertexAmt; i++)
	{
		fscanf(objFile, "v %f %f %f\n", &vertex[i].x, &vertex[i].y, &vertex[i].z);
		if (vertex[i].x < Peri.first.first) Peri.first.first = vertex[i].x;
		if (vertex[i].x > Peri.first.second) Peri.first.second = vertex[i].x;
		if (vertex[i].y < Peri.second.first) Peri.second.first = vertex[i].y;
		if (vertex[i].y > Peri.second.second) Peri.second.second = vertex[i].y;
	}

	for (int i = 1; i <= texCoordAmt; i++)
	{
		fscanf(objFile, "vt %f %f\n", &texCoord[i].u, &texCoord[i].v);
	}

	int ri, temp;
	for (int faceI = 0, groupI = 0; faceI<faceAmt;)
	{
		fgets(lineBuf, MAXLINESIZE, objFile);
		switch (lineBuf[0])
		{
		case 'f':
			ri = faceI * 3;
			sscanf(lineBuf, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				vRenderIndex + ri, tRenderIndex + ri, &temp,
				vRenderIndex + ri + 1, tRenderIndex + ri + 1, &temp,
				vRenderIndex + ri + 2, tRenderIndex + ri + 2, &temp);
			faceI++;
			break;

		case 'g':
			gStartRIndex[groupI++] = faceI * 3;
			break;

		default:
			break;
		}
	}
	gStartRIndex[groupAmt] = faceAmt * 3;
	fclose(objFile);
	printf("Succeeded loading %d vertices and %d triangles.\n", vertexAmt, faceAmt);	
	cout << groupAmt << " " << texCoordAmt << endl;
	//read mtl file:
	FILE* mtlFile = fopen(tempFilePath, "r");
	if (mtlFile == NULL)
	{
		//if no mtl file found, return
		puts("mtl file not found!");
		return;
	}

	//if mtl file exists, then get texture file name
	char tempStr[16];
	while (!feof(mtlFile))
	{
		fgets(lineBuf, MAXLINESIZE, mtlFile);
		sscanf(lineBuf, "%s", tempStr);
		switch (tempStr[0])
		{
		case 'm':
			sscanf(lineBuf, "%s %s", tempStr, tempFileName);
			break;

		default:
			break;
		}
	}
	sprintf(strrchr(tempFilePath, '/') + 1, "%s", tempFileName);

	//load texture image:
	Mat tempImg = imread(tempFilePath);
	if (tempImg.empty())
	{
		//if no texture image found, return
		puts("texture image not found.");
		return;
	}

	//imread will read image upside down, so we mend it handly:
	texImg = Mat(tempImg.rows, tempImg.cols, tempImg.type());
	for (int rowI = 0; rowI<tempImg.rows; rowI++)
	{
		for (int colI = 0; colI<tempImg.cols; colI++)
		{
			texImg.at<cv::Vec3b>(rowI, colI) = tempImg.at<cv::Vec3b>(tempImg.rows - rowI - 1, colI);
		}
	}
}
#endif