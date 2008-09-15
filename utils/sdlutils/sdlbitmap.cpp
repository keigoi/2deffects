#include "utils/sdlutils/sdlbitmap.h"
//
//
#include <sdl.h>
#include <cassert>
#include <iostream>
using namespace std;



namespace syd {

SDL_Surface* SdlBitmap::mainSurface_p = 0;


unsigned SdlBitmap::width() const {
	return sdlSurface_p->w;
}

unsigned SdlBitmap::height() const {
	return sdlSurface_p->h;
}

Bitmap& SdlBitmap::lock() {

	assert(locked_==false);

	int res = SDL_LockSurface(sdlSurface_p);
	
	if( 0 != res ) {
		cout << "sdlsurface : cant lock surface." << endl;
		throw bitmap_exception();
	}

	locked_=true;

	return *this;

}

Bitmap& SdlBitmap::unlock() {

	locked_=false;

	SDL_UnlockSurface(sdlSurface_p);

	return *this;
}

/*color_t* SdlBitmap::getPtr() {
	assert(locked_==true);
	return reinterpret_cast<color_t*>( sdlSurface_p->pixels );
}*/


Bitmap* SdlBitmap::clone(unsigned w, unsigned h, unsigned tagsize ) const {
	cout << "SdlBitmap::clone operation yet not supported..." << endl;
	assert(false);
	SDL_Surface *p=SDL_CreateRGBSurface(SDL_SWSURFACE,
										w, h, 32,
										mainSurface_p->format->Rmask,
										mainSurface_p->format->Gmask,
										mainSurface_p->format->Bmask,
										mainSurface_p->format->Amask);

	if( p == 0 ) { 
		cout << "SDL_CreateRGBSurface() failed in SdlBitmap::clone() " << endl;
		assert(false);
	}
	
	return new SdlBitmap(p);

	return 0;	
}

SDL_Surface* SdlBitmap::getSdlSurface() const {
	return sdlSurface_p;
}

SdlBitmap::SdlBitmap(SDL_Surface* s) : Bitmap(s->format->BytesPerPixel), sdlSurface_p(s) 
{
	cout << "SdlBitmap::SdlBitmap" << endl;

	locked_ = false;

	if(mainSurface_p==0) {
		mainSurface_p = s;
	}

	if( s->format->BitsPerPixel != 32 ) {
		cout << "Sdl bitsperpixel not 32  in SdlBitmap Constructor" << endl;
		assert(false);
	}

	
//	lock();
//	unsigned char *p=(unsigned char*)s->pixels;
//	for(int i=0; i<s->w*s->h*4; i++) { p[i] =255; }
//	unlock();


	pointer_ = sdlSurface_p->pixels;
}


SdlBitmap::~SdlBitmap() {
	cout << "SdlBitmap::~SdlBitmap" << endl;
}
 
void SdlBitmap::invariant() const {
	// not implemented!!!
}

} //syd