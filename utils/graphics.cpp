#include "utils/graphics.h"
#include "utils/rawgraphics.h"
#include "utils/bitmap.h"
#include "utils/plainbmp.h"

#include <utility>
#include <fstream>

#include <memory>

using namespace std;

namespace syd {

Bitmap& blt(Bitmap& out, Bitmap& in, const bltparam& binfo) {

	color_t *d; //dummy
	//クリッピングはbltがやってくれる
	blt(out.getPtr(d), out.width(), out.height(), 
		binfo.destX(), binfo.destY(),
		binfo.width(), binfo.height(),
		in.getPtr(d), in.width(), in.height(),
		binfo.srcX(), binfo.srcY() );

	return out;
}

Bitmap& blt2(Bitmap& out, Bitmap& in, const bltparam& binfo) {

	color_t *d; //dummy
	//クリッピングはbltがやってくれる
	blt2(out.getPtr(d), out.width(), out.height(), 
		binfo.destX(), binfo.destY(),
		binfo.width(), binfo.height(),
		in.getPtr(d), in.width(), in.height(),
		binfo.srcX(), binfo.srcY() );

	return out;
}

Bitmap& hblur2(Bitmap& out, Bitmap& in, const bltparam& binfo) {

	color_t *d; //dummy
	//クリッピングはbltがやってくれる
	hblur2(out.getPtr(d), out.width(), out.height(), 
		binfo.destX(), binfo.destY(),
		binfo.width(), binfo.height(),
		in.getPtr(d), in.width(), in.height(),
		binfo.srcX(), binfo.srcY() );

	return out;
}

Bitmap& hblur4(Bitmap& out, Bitmap& in, const bltparam& binfo) {

	color_t *d; //dummy
		hblur4(out.getPtr(d), out.width(), out.height(), 
			binfo.destX(), binfo.destY(),
			binfo.width(), binfo.height(),
			in.getPtr(d), in.width(), in.height(),
			binfo.srcX(), binfo.srcY() );
	return out;
}

Bitmap& line(Bitmap& out, color_t& c, const lineparam& param) {
	color_t *d; //dummy
	line(out.getPtr(d), out.width(), out.height(), c, param.startX(), param.startY(), param.endX(), param.endY()); 
	return out;
}


std::auto_ptr<Bitmap> load_bmp(const char* filename) {

	auto_ptr<Bitmap> retval;

	ifstream f(filename, ios::binary);
	new_plain_bmp32 bmp = load_plain_bmp32(f);
	if( bmp.new_pixels != 0 ) {
		retval.reset(new PlainBmp(bmp.new_pixels, bmp.w, bmp.h, 4));
	}

	return retval;
}

} //syd