#ifndef _SDLBITMAP_H_
#define _SDLBITMAP_H_
//
#include "utils/bitmap.h"
//

struct SDL_Surface;

namespace syd {

class SdlBitmap : public Bitmap {
	
	SDL_Surface* sdlSurface_p;

	static SDL_Surface* mainSurface_p;

public:
	SDL_Surface* getSdlSurface() const;

public:
	virtual unsigned width() const;
	virtual unsigned height() const;

public:
	virtual Bitmap& lock();
	virtual Bitmap& unlock();
//	virtual color_t* getPtr();

public:
	virtual Bitmap* clone(unsigned w, unsigned h, unsigned tagsize) const;

public:
	SdlBitmap(SDL_Surface* s);
	virtual ~SdlBitmap();

public:
	virtual void invariant() const; 	// not implemented!!!
};

} //syd

#endif //_SDLBITMAP_H_
