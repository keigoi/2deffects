#ifndef _SYD_COLOR_H_
#define _SYD_COLOR_H_
#include "utils/types.h"

namespace syd {

typedef struct color_tag_rgba {
	color_tag_rgba() : b(0), g(0), r(0), a(0) {};
	color_tag_rgba( uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
		: b(b_), g(g_), r(r_), a(a_) {};
//	color_tag_rgba(const color_tag_rgba& c) : b(c.b), g(c.g), r(c.r), a(c.a) {}
//	color_tag_rgba operator=(const color_tag_rgba& c) {
//		if( this==&c )return *this;
//		b=c.b;
//		g=c.g;
//		r=c.r;
//		a=c.a;
//	}

	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} color_t;

} //syd

#endif //_SYD_COLOR_H_
