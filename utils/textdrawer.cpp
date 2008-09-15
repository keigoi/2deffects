// interface
#include "utils/textdrawer.h"

// implement
#include "utils/bitmap.h"
#include "utils/graphics.h"
#include <cstring>
#include <cassert>

namespace syd {


TextDrawer::TextDrawer ( Bitmap& font_bmp, unsigned char_width, unsigned char_height ) 
 : char_width_( char_width ), char_height_( char_height ), font_bmp_( font_bmp ) {

	assert( font_bmp_.width() > char_width_ );
	assert( font_bmp_.height() > char_height );

}


TextDrawer::~TextDrawer() {

}


TextDrawer& TextDrawer::draw ( Bitmap& destbmp, unsigned x, unsigned y, const char* text ) {

	const char* outtxt = text;
	int outx = x;
	int len = strlen(text);

	if( y > destbmp.height() ) { 
		return *this; 
	}

	for( int p = 0; p < len; p++ ) {	

		if( x > destbmp.width() ) { break; }

		if( '!' <= outtxt[p] && outtxt[p] <= '~' ) {
			int srcx = ( outtxt[p] - '!' ) * char_width_ % font_bmp_.width();
			int srcy = (( outtxt[p] - '!' ) * char_width_ / font_bmp_.width()) * char_height_;

			bltparam bparam;
			bparam.width(char_width_).height(char_height_)
				.srcX(srcx).srcY(srcy)
				.destX(outx).destY(y);
			blt(destbmp, font_bmp_, bparam );
		
		}

		outx += char_width_;

	}

	return *this;

}


} //syd