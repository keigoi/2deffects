#include <iostream>
#include <cassert>

#include "utils/winutils/dibsection.h"

#include <windows.h>

using namespace std;

namespace syd {

Bitmap& DibSection::lock () {

	invariant ();
	
	if( locked_ == true ) {
		cout << "cant lock dibsection" << endl;
		throw bitmap_exception();
	}
	
	locked_ = true;

	return *this;

}


Bitmap& DibSection::unlock () {

	invariant ();
	
	locked_=false;

	return *this;

}


/*color_t* DibSection::getPtr () {

	invariant ();

	assert ( locked_==true );

	return ptr_;

}*/


HBITMAP DibSection::getHBitmap () const {

	invariant ();

	return hbitmap_;

}


unsigned DibSection::width () const {
	
	invariant();

	return width_;

}


unsigned DibSection::height () const {

	invariant();

	return height_;
}


Bitmap* DibSection::clone ( unsigned w, unsigned h, unsigned tagsize ) const {

	return new DibSection ( w, h, tagsize, turnover_ );

}


inline static void setBitmapInfoHeader ( BITMAPINFOHEADER& bih, int w, int h , int tagsize) {
    
	bih.biSize				= sizeof ( BITMAPINFOHEADER );
	bih.biWidth  			= w;
    bih.biHeight 			= h;
    bih.biPlanes 			= 1;

	bih.biBitCount 			= tagsize*8;
    bih.biCompression 		= BI_RGB;
    bih.biSizeImage 		= 0;
    bih.biXPelsPerMeter		= 0;
    bih.biYPelsPerMeter		= 0;
    bih.biClrUsed			= 0;
    bih.biClrImportant		= 0;

}


DibSection::DibSection ( unsigned w, unsigned h, unsigned tagsize, bool turnover)
 : Bitmap(tagsize) {
	
	locked_ = false;
	width_ = w;
	height_ = h;
	turnover_ = turnover;

	void* bmp;
    BITMAPINFO bitmapInfo;

	if(turnover_) {
		setBitmapInfoHeader ( bitmapInfo.bmiHeader, width_, height_, tagsize_ );
	} else {
	    setBitmapInfoHeader ( bitmapInfo.bmiHeader, width_, -height_,  tagsize_ );
	}

	HBITMAP hBmp = ::CreateDIBSection ( 0,
										&bitmapInfo,
										DIB_RGB_COLORS,
    									&bmp,
										NULL,
										NULL );

	assert ( hBmp != 0 );

	hbitmap_ = hBmp;

	

//	lock ();
//	unsigned char *p=(unsigned char*)bmp;
//	for(int i=0; i<w*h*4; i++) { p[i] =255; }
//	unlock ();

	cout << "DibSection::DibSection()" << endl;

	pointer_ = bmp;

	invariant();
}


DibSection::~DibSection () {

	::DeleteObject ( hbitmap_ );

	cout << "DibSection::~DibSection" << endl;
}


// スピード的に高くつく
void DibSection::invariant() const {
	
	DWORD res = ::GetObjectType ( hbitmap_ );
	assert ( res != 0 );

//	HGDIOBJ obj = ::GetCurrentObject ( hDC_, OBJ_BITMAP );
//	assert ( hbitmap_ == (HBITMAP) obj );

	assert ( 0 < width_   &&  width_  < 8193 );
	assert ( 0 < height_  &&  height_ < 8193 );

//	assert ( pointer_ != NULL );  // commented out.

}

} //namespace syd