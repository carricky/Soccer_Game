#include "util.h"
#include "bitmap.h"

#include "../core/globaldata.h"
#include <string>

using namespace zagflws;
using namespace std;

const double global::math::PI {3.14159265358979323846};

float global::util::slowDown {3};
float global::util::xSpeed;
float global::util::ySpeed;

GLuint global::util::loop;
GLuint global::util::col;
GLuint global::util::delay;

global::util::Particle global::util::particle[MAX_PARTICLES];

bool global::util::LoadData() {
  // TODO
  return true;
}

bool global::util::SaveData() {
  // TODO
  return true;
}

void global::util::GetCurrentImg(int w, int h, const string& fileprefix) {
	GLubyte * pPixelData;
	int PixeldataLen, i;
	i = w*3;
	while (i%4 !=0) ++i;
	PixeldataLen = i*h;
	pPixelData = (GLubyte *)malloc(PixeldataLen);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
	FILE *filePtr;	// 文件指针
	string filename = "../user/" + fileprefix + ".bmp";
	filePtr = fopen(filename.c_str(), "wb+"); 

	// 读入bitmap信息头
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap文件头
	WORD(bitmapFileHeader.bfType_) = BITMAP_ID;
	DWORD(bitmapFileHeader.bfOffBits_) = 54; //默认开头到数据区54字节
	DWORD(bitmapFileHeader.bfSize_) = 54+w*h*3;
	WORD(bitmapFileHeader.bfReserved1_) = 0;
	WORD(bitmapFileHeader.bfReserved2_) = 0;
	
	BITMAPINFOHEADER bitmapInfoHeader;
	DWORD(bitmapInfoHeader.biSize_) = 40;
	DWORD(bitmapInfoHeader.biWidth_) = w;
	DWORD(bitmapInfoHeader.biHeight_) = h;
	WORD(bitmapInfoHeader.biPlanes_) = 0;
	WORD(bitmapInfoHeader.biBitCount_) = 24;
	DWORD(bitmapInfoHeader.biCompression_) = 0;
	DWORD(bitmapInfoHeader.biSizeImage_) = w*h*3;
	DWORD(bitmapInfoHeader.biXPelsPerMeter_) = 2849; //我也不知道要设什么，分辨率什么的
	DWORD(bitmapInfoHeader.biYPelsPerMeter_) = 2849; //我也不知道要设什么，分辨率什么的
	DWORD(bitmapInfoHeader.biClrUsed_) = 0;
	DWORD(bitmapInfoHeader.biClrImportant_) = 0;

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fwrite(pPixelData, 1, PixeldataLen, filePtr);
	
	// 关闭bitmap图像文件
	fclose(filePtr); 
}


void global::math::to_3d_vector(float v[3]) {
  v[0] *= math::cos(global::world::gRotateX);
  v[1] *= -math::cos(global::world::gRotateY);
  v[2] *= math::sin(global::world::gRotateX) * math::sin(global::world::gRotateY);
}
