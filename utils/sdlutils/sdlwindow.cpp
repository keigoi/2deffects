#include "utils/sdlutils/sdlwindow.h"
//
//
#include "utils/sdlutils/sdlbitmap.h"
#include "utils/sdlutils/sdleventpumper.h"
#include "utils/windowevents.h"  
//
//
#include <sdl.h>
#include <cassert>
#include <iostream>
using namespace std;

namespace syd {

unsigned long SdlWindow::videoModeOption_ = SDL_DOUBLEBUF;//SDL_HWSURFACE |SDL_FULLSCREEN:
bool SdlWindow::canSetVideoMode_ = true;
SdlWindow* SdlWindow::singleton_ = 0;


Window* SdlWindow::clone() const {
	return new SdlWindow();
}

Window& SdlWindow::create(const Rect &rect) {
	
	assert ( singleton_ == 0 );
	
	rect_ = rect;

	//SDL_SetVideoModeが以前に実行されていれば失敗する。
	if( canSetVideoMode_ == false ) {
		
		assert(false);

	} else {

		SDL_Surface* s = ::SDL_SetVideoMode(rect.width(), rect.height(), 32, videoModeOption_);

		if( s == NULL ) {
			cout << "SDL_SetVideoMode failed" << endl;
			cout << "  width:" << rect.width() << "  height:" << rect.height() << "  videomode:" 
				<< videoModeOption_ << endl;
			cout << "SDL_GetError():" << SDL_GetError() << endl;
			assert(false);
		} else {
			canSetVideoMode_ = false;
			auto_ptr<SdlBitmap> buf(new SdlBitmap(s));
			buffer_p = buf;

		}
	}

	singleton_ = this;

	return *this;

}

Window& SdlWindow::drawText(Point point, const char* str) {
	//cout << "SdlWindow::drawText yet not be supported" << endl;
	return *this;
}

Bitmap& SdlWindow::getBuffer() const {
	assert(buffer_p.get() != NULL);

	return *buffer_p;
}

bool SdlWindow::isAlive() const {
	return ( buffer_p.get() != NULL );
}

Rect SdlWindow::rect() const {
	return rect_;
}

Window& SdlWindow::refresh() {
	SDL_Surface *s=buffer_p->getSdlSurface();

	SDL_Flip( s );
/*	
	SDL_Rect rect;
	rect.x=0;
	rect.y=0;
	rect.w=s->w;
	rect.h=s->h;
	SDL_FillRect(s,&rect,0xFFFFFFFF);
	//cout << "SdlWindow::refresh";
*/
	return *this;
}


SdlWindow::SdlWindow()
 : buffer_p(0),eventPumper_(std::auto_ptr<SdlEventPumper>( new SdlEventPumper(*this) ) ) {

//	eventPumper_ = std::auto_ptr<SdlEventPumper>( new SdlEventPumper(*this) ) ;
	cout << "SdlWindow::SdlWindow" << endl;

}

SdlWindow::~SdlWindow() {
//	singleton_=0;	
	cout << "SdlWindow::~SdlWindow" << endl;
}

Window& SdlWindow::setRect(const Rect &rect) {
	rect_ = rect;
	return *this;
}

Window& SdlWindow::setTitle(const char* str) {
	SDL_WM_SetCaption( str, str );
	return *this;
}

Window& SdlWindow::show(bool showState) {
	cout << "SdlWindow::show yet not be supported" << endl;
	return *this;
}


void SdlWindow::invariant() const {
	//not implemented!!
}


Window& SdlWindow::setListener ( WindowListener* l) {
	
	eventPumper_->setListener(l);
	return *this;

}


Window& SdlWindow::setListener ( MouseListener* l) {

	eventPumper_->setListener(l);
	return *this;

}


Window& SdlWindow::setListener ( KeyListener* l) {

	eventPumper_->setListener(l);
	return *this;

}


void SdlWindow::destroy() {
	cout << "SdlWindow::destroy() not implemented" << endl;
}


void SdlWindow::usingFullScreen( bool flag ) {

	assert( singleton_ == 0 ); //create前ならok

	if( flag == true ) {
		videoModeOption_ |= SDL_FULLSCREEN;
	} else {
		videoModeOption_ &= ~SDL_FULLSCREEN;
	}

}


void SdlWindow::usingDoubleBuffer( bool flag ) {

	assert( singleton_ == 0 ); //create前ならok
	if( flag == true ) {
		videoModeOption_ |= SDL_DOUBLEBUF;
	} else {
		videoModeOption_ &= ~SDL_DOUBLEBUF;
	}

}


void SdlWindow::usingHardwareSurface( bool flag ) {

	assert( singleton_ == 0 ); //create前ならok
	if( flag == true ) {
		videoModeOption_ |= SDL_HWSURFACE;
	} else {
		videoModeOption_ &= ~SDL_HWSURFACE;
	}
}


EventPumper& SdlWindow::getEventPumper() const {

	return *eventPumper_;

}

} // namespace syd
