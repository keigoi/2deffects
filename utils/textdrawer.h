#ifndef _TEXTDRAWER_H_
#define _TEXTDRAWER_H_

namespace syd {

class Bitmap;
class PlainBmp;

class TextDrawer {

	unsigned char_width_, char_height_;
	Bitmap& font_bmp_;

public:
	TextDrawer ( Bitmap& font_bmp, unsigned char_width, unsigned char_height );
	~TextDrawer();

public:
	TextDrawer& draw ( Bitmap& destbmp, unsigned x, unsigned y, const char* text );

};


} //syd


#endif //!defined(_TEXTDRAWER_H_)