#include "utils/rawgraphics.h"
#include <iostream>

namespace syd{

using namespace std;

typedef struct tagBITMAPFILEHEADER { 
  uint16_t    bfType; 
  uint32_t    bfSize; 
  uint16_t    bfReserved1; 
  uint16_t    bfReserved2; 
  uint32_t    bfOffBits; 
  inline void load_from_istream(istream& infile) {
    infile.read(reinterpret_cast<char*>(&bfType), 2);
	infile.read(reinterpret_cast<char*>(&bfSize), 4);
	infile.read(reinterpret_cast<char*>(&bfReserved1), 2);
	infile.read(reinterpret_cast<char*>(&bfReserved2), 2);
	infile.read(reinterpret_cast<char*>(&bfOffBits), 4);
  }

} BITMAPFILEHEADER, *PBITMAPFILEHEADER; 


typedef struct tagBITMAPINFOHEADER{
  enum {REAL_SIZE=0x28};
  uint32_t  biSize; 
  int32_t   biWidth; 
  int32_t   biHeight; 
  uint16_t  biPlanes; 
  uint16_t  biBitCount; 
  uint32_t  biCompression; 
  uint32_t  biSizeImage; 
  int32_t   biXPelsPerMeter; 
  int32_t   biYPelsPerMeter; 
  uint32_t  biClrUsed; 
  uint32_t  biClrImportant; 
  inline void load_from_istream(istream &infile) {
		infile.read(reinterpret_cast<char*>(&biSize), 4);
		infile.read(reinterpret_cast<char*>(&biWidth), 4);
		infile.read(reinterpret_cast<char*>(&biHeight), 4);
		infile.read(reinterpret_cast<char*>(&biPlanes), 2);
		infile.read(reinterpret_cast<char*>(&biBitCount), 2);
		infile.read(reinterpret_cast<char*>(&biCompression), 4);
		infile.read(reinterpret_cast<char*>(&biSizeImage), 4);
		infile.read(reinterpret_cast<char*>(&biXPelsPerMeter), 4);
		infile.read(reinterpret_cast<char*>(&biYPelsPerMeter), 4);
		infile.read(reinterpret_cast<char*>(&biClrUsed), 4);
		infile.read(reinterpret_cast<char*>(&biClrImportant), 4);
  }
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

typedef struct tagRGBQUAD {
  uint8_t    rgbBlue; 
  uint8_t    rgbGreen; 
  uint8_t    rgbRed; 
  uint8_t    rgbReserved; 
} RGBQUAD; 


typedef struct tagBITMAPINFO { 
  BITMAPINFOHEADER bmiHeader; 
  RGBQUAD          bmiColors[1]; 
} BITMAPINFO, *PBITMAPINFO; 

typedef struct tagBITMAPCOREHEADER {
  enum {REAL_SIZE=0xC};
  uint32_t    bcSize; 
  uint16_t    bcWidth; 
  uint16_t    bcHeight; 
  uint16_t    bcPlanes; 
  uint16_t    bcBitCount; 
  inline void load_from_istream(istream& infile) {
  		infile.read(reinterpret_cast<char*>(&bcSize),4);
		infile.read(reinterpret_cast<char*>(&bcWidth),2);
		infile.read(reinterpret_cast<char*>(&bcHeight),2);
		infile.read(reinterpret_cast<char*>(&bcPlanes),2);
		infile.read(reinterpret_cast<char*>(&bcBitCount),2);
  }
} BITMAPCOREHEADER, *PBITMAPCOREHEADER; 

typedef struct tagRGBTRIPLE { 
  uint8_t rgbtBlue; 
  uint8_t rgbtGreen; 
  uint8_t rgbtRed; 
} RGBTRIPLE; 

typedef struct _BITMAPCOREINFO { 
  BITMAPCOREHEADER  bmciHeader; 
  RGBTRIPLE         bmciColors[1]; 
} BITMAPCOREINFO, *PBITMAPCOREINFO; 


inline void load_24(uint32_t* out, const uint8_t* in, uint16_t width, uint16_t height, int gap) {
	RGBQUAD* outptr      =reinterpret_cast<RGBQUAD*>(out);
	const uint8_t* inptr =in + (width*3+gap) * (height-1) ; //画像の左下隅

	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {

			outptr->rgbBlue=*inptr;
			inptr++;
			outptr->rgbGreen=*inptr;
			inptr++;
			outptr->rgbRed=*inptr;
			inptr++;
		
			outptr->rgbReserved=0;

			outptr++;
		}
		inptr += gap;
		inptr-=(width*3+gap)*2;
	}
	
}

new_plain_bmp32 load_plain_bmp32(std::istream& infile) {
	new_plain_bmp32 retval;
	retval.new_pixels =0;
	retval.w = retval.h =0;

	BITMAPFILEHEADER bmpFH;

	bmpFH.load_from_istream(infile);


	if(bmpFH.bfType != 0x4D42) { return retval; }  // 4D='M' 42='B'
	if(bmpFH.bfSize == 0) { return retval; }
	//if(bmpFH.bfReserved1 != 0) { return retval; }
	//if(bmpFH.bfReserved2 != 0) { return retval; }


	//BITMAPINFOかBITMAPCOREINFOか判別
	uint32_t tagSize;
	infile.read(reinterpret_cast<char*>(&tagSize), 4);
	infile.seekg(-4, ios::cur);

	uint16_t width, height, bitCount, gap;

	//widthとheightを戻り値へ格納
	if(tagSize==BITMAPINFOHEADER::REAL_SIZE) { 

		BITMAPINFO bmpI;
		bmpI.bmiHeader.load_from_istream(infile);

		width     =bmpI.bmiHeader.biWidth;
		height    =bmpI.bmiHeader.biHeight;
		bitCount  =bmpI.bmiHeader.biBitCount;

	} else if(tagSize==BITMAPCOREHEADER::REAL_SIZE) {

		BITMAPCOREINFO bmpCI;

		bmpCI.bmciHeader.load_from_istream(infile);

		width     =bmpCI.bmciHeader.bcWidth;
		height    =bmpCI.bmciHeader.bcHeight;
		bitCount  =bmpCI.bmciHeader.bcBitCount;

	} else { return retval; }

	if((width*3) % 4 == 0) {
		gap = 0;
	} else {
		gap = 4 - (width*3)%4;
	}

	//データ異常チェック
	if( width*height > 0x00400000 ) {return retval; } //64MBytes以上はエラー
	if( !(bitCount ==24) ) { return retval; } //24bitColorのみ

	//データ本体へのシーク
	infile.seekg(bmpFH.bfOffBits, ios::beg);

	uint32_t* retbuf;
	{
		//データ本体のコピー先バッファ
		uint8_t *buf = new uint8_t[ (width*(bitCount/8)+gap) * height];
		//コピー
		infile.read(reinterpret_cast<char*>(buf), width * height * (bitCount/8) + gap );
		//24->32bit 変換 かつ 上下ひっくり返りを修正
		retbuf = new uint32_t[width * height];
		load_24(retbuf, buf, width, height, gap);
		delete[]buf;
	}
	retval.new_pixels = reinterpret_cast<color_t*>(retbuf);
	retval.w =width;
	retval.h =height;

	//delete[]retval.new_pixels;は呼び出し側の責任
	return retval;
}




} //syd