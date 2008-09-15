#include "utils/sdlutils/sdleventpumper.h"

#include <sdl.h>
#include <cassert>
#include "utils/windowevents.h"

namespace syd {

SdlEventPumper* SdlEventPumper::singleton_ = 0;

SdlEventPumper::SdlEventPumper( Window& window ) : window_(window), quitRequested_(false) {

	windowListener_=0;
	keyListener_=0;
	mouseListener_=0;

	assert( singleton_ == 0 );

	singleton_ = this;

};


SdlEventPumper::~SdlEventPumper() {

	singleton_ = 0;
}

#define safe_call(x,y,z) \
	if(x) x->y(z);else;

void SdlEventPumper::pumpEvents() {

	SDL_Event event;

	Window& self=window_;
	
	SDL_PumpEvents();

	if( SDL_PollEvent(&event) ) {
		
		switch( event.type ) {

		case SDL_ACTIVEEVENT:

			switch( event.active.state ) {
			
			case SDL_APPMOUSEFOCUS:
				// no implements
				break;

			case SDL_APPINPUTFOCUS:

				// no implements
				break;

			case SDL_APPACTIVE:

				if( event.active.gain == 1 ) {
					safe_call ( windowListener_ , windowDeiconified , self );
				} else {
					safe_call ( windowListener_ , windowIconified , self );
				}
				break;

			default:
				break;

			}

			break;

		case SDL_KEYDOWN:
			if( event.key.keysym.sym == SDLK_ESCAPE ) {
				SDL_Event event;
				event.type = SDL_QUIT;
				SDL_PushEvent( &event );
			}
			break;

		case SDL_KEYUP:
			break;

		case SDL_MOUSEMOTION:
			break;

		case SDL_MOUSEBUTTONDOWN:
			//self.pmouse_listener_->mousePressed(
			break;

		case SDL_MOUSEBUTTONUP:
			break;

		case SDL_JOYAXISMOTION:
			break;

		case SDL_JOYBALLMOTION:
			break;

		case SDL_JOYHATMOTION:
			break;

		case SDL_JOYBUTTONDOWN:
			break;

		case SDL_JOYBUTTONUP:
			break;

		case SDL_QUIT:
			safe_call ( windowListener_ , windowClosing , self );
			quitRequested_ = true;
			break;

		case SDL_SYSWMEVENT:
			break;

		case SDL_VIDEORESIZE:
			safe_call( windowListener_ , windowResized, self );
			break;

		case SDL_VIDEOEXPOSE:
			break;

		case SDL_USEREVENT:
			break;

		default:
			break;

		}

	}

}

#undef safe_call


bool SdlEventPumper::toExit() const {
	return quitRequested_;
}


SdlEventPumper& SdlEventPumper::setListener( WindowListener* l ) {

	windowListener_=l;
	return *this;
}

SdlEventPumper& SdlEventPumper::setListener( MouseListener* l ) {

	mouseListener_=l;
	return *this;
}

SdlEventPumper& SdlEventPumper::setListener( KeyListener* l) {

	keyListener_=l;
	return *this;
}


};