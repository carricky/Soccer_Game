#ifndef BITMAP_H
#define BITMAP_H

#define BYTE(x) (*((BYTE_PTR)x))
#define WORD(x) (*((WORD_PTR)x))
#define DWORD(x) (*((DWORD_PTR)x))
#define BITMAP_ID 0x4D42

typedef unsigned char* BYTE_PTR;
typedef unsigned short* WORD_PTR;
typedef unsigned int* DWORD_PTR;

#pragma pack(push)
#pragma pack(2)

struct BITMAPFILEHEADER {
  unsigned char bfType_[2];
  unsigned char bfSize_[4];
  unsigned char bfReserved1_[2];
  unsigned char bfReserved2_[2];
  unsigned char bfOffBits_[4];
};

struct BITMAPINFOHEADER {
  unsigned char biSize_[4];
  unsigned char biWidth_[4];
  unsigned char biHeight_[4];
  unsigned char biPlanes_[2];
  unsigned char biBitCount_[2];
  unsigned char biCompression_[4];
  unsigned char biSizeImage_[4];
  unsigned char biXPelsPerMeter_[4];
  unsigned char biYPelsPerMeter_[4];
  unsigned char biClrUsed_[4];
  unsigned char biClrImportant_[4];
};

#pragma pack(pop)

#endif // BMP_HEADER_H
