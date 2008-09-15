#include "utils/plainbmp.h"
#include <cassert>
#include <iostream>
using namespace std;

namespace syd {


unsigned PlainBmp::width() const {

	invariant();

	return width_;
}


unsigned PlainBmp::height() const {

	invariant();

	return height_;
}


Bitmap& PlainBmp::lock() {

	invariant();
	
	if( locked_ == true ) {
		//cout << "cant lock PlainBmp" << endl;
		throw bitmap_exception();
	}

	locked_=true;
	return *this;

}


Bitmap& PlainBmp::unlock() {

	invariant();

	locked_=false;
	return *this;
}


void PlainBmp::attach(void* ptr, unsigned w, unsigned h) {
	pointer_ = ptr;
	width_=w;
	height_=h;

	invariant();
}


Bitmap* PlainBmp::clone(unsigned w, unsigned h, size_t tagsize) const {
	return new PlainBmp(w,h,tagsize);	
}


PlainBmp::PlainBmp(unsigned w, unsigned h, size_t tagsize) : 
								   width_(w), height_(h),
								   Bitmap(tagsize)
								   
{
	assert(sizeof(char)==1);

	locked_=false;
	pointer_=new char[w*h*tagsize];
	//cout << "PlainBmp::PlainBmp" << endl;
	invariant();
}


PlainBmp::PlainBmp(void* ptr, unsigned w, unsigned h, size_t tagsize) : 
								   width_(w), height_(h),
								   Bitmap(tagsize)
{
	locked_=false;
	pointer_=ptr;
	//cout << "PlainBmp::PlainBmp" << endl;
	invariant();
}


PlainBmp::~PlainBmp() {
	//cout << "PlainBmp::~PlainBmp" << endl;
	if(pointer_!=0) {
		delete[] pointer_;
		pointer_ = 0;
	}
}
 

void PlainBmp::invariant() const {

	assert ( pointer_ != 0 );
	assert ( 0 < width_   &&  width_  < 8193 );
	assert ( 0 < height_  &&  height_ < 8193 );

}

} //syd