#ifndef _SYD_GRAPHICS_H_
#define _SYD_GRAPHICS_H_
#include "utils/color.h"
#include "utils/types.h"

#include <memory>

namespace syd {

class bltparam {
	uint16_t destX_, destY_,
			 srcX_, srcY_,
			 width_, height_;

public:
	bltparam() {
		destX_=destY_=
		srcX_=srcY_=
		width_=height_=0;
	}

public:
	uint16_t destX() const { return destX_; }
	uint16_t destY() const { return destY_; }
	uint16_t srcX() const { return srcX_; }
	uint16_t srcY() const { return srcY_; }
	uint16_t width() const { return width_; }
	uint16_t height() const { return height_; }

public:
	bltparam& destX(uint16_t x) { destX_=x; return *this; }
	bltparam& destY(uint16_t y) { destY_=y; return *this; }
	bltparam& srcX(uint16_t x) { srcX_=x; return *this; }
	bltparam& srcY(uint16_t y) { srcY_=y; return *this; }
	bltparam& width(uint16_t w) { width_=w; return *this; }
	bltparam& height(uint16_t h) { height_=h; return *this; }
};

class lineparam {
	uint16_t startx_,starty_,endx_,endy_;

public:
	lineparam() {
		startx_=starty_=
		endx_=endy_=0;
	}

	lineparam( uint16_t startx, uint16_t starty,
		uint16_t endx, uint16_t endy )
		 : startx_( startx ), starty_( starty ),
		 endx_( endx ) , endy_( endy ) {}

public:
	uint16_t startX() const { return startx_; }
	uint16_t startY() const { return starty_; }
	uint16_t endX() const { return endx_; }
	uint16_t endY() const { return endy_; }

public:

	lineparam& start(uint16_t x, uint16_t y) { startx_=x; starty_=y; return *this; }
	lineparam& end(uint16_t x, uint16_t y) { endx_=x; endy_=y; return *this; }
};

class Bitmap;

Bitmap& blt(Bitmap& out, Bitmap& in, const bltparam& binfo);
Bitmap& blt2(Bitmap& out, Bitmap& in, const bltparam& binfo);
Bitmap& hblur2(Bitmap& out, Bitmap& in, const bltparam& binfo);
Bitmap& hblur4(Bitmap& out, Bitmap& in, const bltparam& binfo);
Bitmap& line(Bitmap& out, color_t& col, const lineparam& param);

std::auto_ptr<Bitmap> load_bmp(const char* filename);

}; //syd
#endif //_SYD_GRAPHICS_H_
