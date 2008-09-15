#ifndef _RAWGRAPHICS_H_
#define _RAWGRAPHICS_H_

#include <iostream>
#include <cassert>
#include "utils/types.h"
#include "utils/color.h"

namespace syd {


struct new_plain_bmp32 {
	int w,h;
	color_t *new_pixels;
};

new_plain_bmp32 load_plain_bmp32(std::istream& infile);


namespace {

	inline bool clip(uint16_t& owidth,
					uint16_t& oheight,
					uint16_t& ox,
					uint16_t& oy,
					uint16_t& twidth,
					uint16_t& theight,
					uint16_t& iwidth,
					uint16_t& iheight,
					uint16_t& ix,
					uint16_t& iy) {

		//clipping
		if(ox > owidth -1 || oy > oheight) { return false; }
		if(ix > iwidth -1 || iy > iheight) { return false; }

		if( ox + twidth > owidth -1 ) { twidth = owidth - ox; }
		if( ix + twidth > iwidth -1 ) { twidth = iwidth - ix; }
		if( oy + theight> oheight -1) { theight= oheight- oy; }
		if( iy + theight> iheight -1) { theight= iheight- iy; }

		return true;
	}

	template<typename T>
	void swap(T& lhs, T& rhs) {
		T tmp=lhs;
		lhs=rhs;
		rhs=tmp;
	}

	template<typename T>
	T abs(T t) {
		if(t<0)return -t;
		else return t;
	}

	inline bool line_param_validate(
						   const uint16_t& width,
						   const uint16_t& height,
						   uint16_t& startx,
						   uint16_t& starty,
						   uint16_t& endx,
						   uint16_t& endy ) {

		
		if(starty>endy) {
			swap(startx, endx);
			swap(starty, endy);
		}

		if(startx>=width || starty>=height  || endx>=width || endy>=height) {
			return false; //たいしたクリッピング機能ではない
		}

		return true;
	}
}

inline void line( color_t* pixels,
				  uint16_t width,
				  uint16_t height,
				  color_t c,
				  uint16_t startx,
				  uint16_t starty,
				  uint16_t endx,
				  uint16_t endy ) {


	bool clip_valid = line_param_validate(width, height, startx, starty, endx, endy);

	if( clip_valid = false ) {
		assert(false);
		return; // not debug then return
	}

	color_t* out=pixels;
	out+=startx + starty*width;

	int dw=endx-startx;
	int dh=endy-starty; assert(dh>=0);// must be >0

	if(abs(dw)>dh) {
		//x step
		float a=abs((float)dh/(float)dw);
		if(dw>0) {
			float y=0.0;
			for(int x=0; x<dw; x++) {
				int iy=((int)y)*width;
				out[x+iy]=c;
				y+=a;
			}
		} else {
			// dw <= 0
			float y=0.0;
			for(int x=0; x>dw; x--) {
				int iy=((int)y)*width;
				out[x+iy]=c;
				y+=a;
			}
		}
	} else {
		float a=dw/(float)dh;
		int size=abs(width*dh);
		float x=0.0;
		for(int y=0; y<size; y+=width) {
			int ix=x;
			out[ix+y]=c;
			x+=a;
		}
	}
}


inline color_t* fill( color_t* pixels,
					  uint16_t owidth,
					  uint16_t oheight,
					  color_t c ) {
	
	int size=owidth*oheight;
	for(int i=0; i<size; i++) {
		pixels[i]=c; 
	}
	return pixels;
}

inline void blt(color_t* outPtr,
		  uint16_t owidth,
		  uint16_t oheight,
		  uint16_t ox,
		  uint16_t oy,
		  uint16_t twidth,
		  uint16_t theight,
		const color_t* inPtr,
		  uint16_t iwidth,
		  uint16_t iheight,
		  uint16_t ix,
		  uint16_t iy) {
	//clipping
	bool clipped =  clip(owidth, oheight, ox, oy, twidth, theight, iwidth, iheight, ix, iy);

	if( clipped == false ) {
		assert(false);
		return;  // for debug;
	}

	uint16_t ogap = owidth - twidth;
	uint16_t igap = iwidth - twidth;

	color_t* out = outPtr + ox + oy*owidth;
	const color_t* in = inPtr + ix + iy*iwidth;

	for(uint16_t y=theight; y!=0; y--) {
		for(uint16_t x=twidth; x!=0; x--) { //さりげなくダウンカウンタ

			*out=*in;
			out++;
			in++;

		}
		out+=ogap;
		in+=igap;
	}
}


inline void blt2(color_t* outPtr,
		  uint16_t owidth,
		  uint16_t oheight,
		  uint16_t ox,
		  uint16_t oy,
		  uint16_t twidth,
		  uint16_t theight,
		const color_t* inPtr,
		  uint16_t iwidth,
		  uint16_t iheight,
		  uint16_t ix,
		  uint16_t iy) {
	//clipping
	if(clip(owidth, oheight, ox, oy, twidth, theight, iwidth, iheight, ix, iy) ==false)return;

	uint16_t ogap = owidth - twidth;
	uint16_t igap = iwidth - twidth;

	color_t* out = outPtr + ox + oy*owidth;
	const color_t* in = inPtr + ix + iy*iwidth;

	for(uint16_t y=theight; y!=0; y--) {
		for(uint16_t x=twidth; x!=0; x--) { //さりげなくダウンカウンタ
			out->r += in->r;
			out->g += in->g;
			out->b += in->b;
			out++;
			in++;

		}
		out+=ogap;
		in+=igap;
	}
}

inline void hblur2(color_t* outPtr,
		  uint16_t owidth,
		  uint16_t oheight,
		  uint16_t ox,
		  uint16_t oy,
		  uint16_t twidth,
		  uint16_t theight,
		  const color_t* inPtr,
		  uint16_t iwidth,
		  uint16_t iheight,
		  uint16_t ix,
		  uint16_t iy) {

	//clipping
	if(clip(owidth, oheight, ox, oy, twidth, theight, iwidth, iheight, ix, iy) ==false)return;

	uint16_t ogap = owidth - twidth;
	uint16_t igap = iwidth - twidth;

	color_t* out = outPtr + ox+2 + oy*owidth ;
	const color_t* in = inPtr + ix+2 + iy*iwidth ;

	for(uint16_t y=theight; y!=0; y--) {
		for(uint16_t x=twidth-4; x!=0; x--) { //さりげなくダウンカウンタ

//ホワイトアウト
//			out->r = 255-(((255-(in-1)->r) + (255-(in +1)->r))/2);
//			out->g = 255-(((255-(in-1)->g) + (255-(in +1)->g))/2);
//			out->b = 255-(((255-(in-1)->b) + (255-(in +1)->b))/2);
//ブラックアウト
//			out->r = ((in-1)->r + (in +1)->r)/2;
//			out->g = ((in-1)->g + (in +1)->g)/2;
//			out->b = ((in-1)->b + (in +1)->b)/2;
//ホワイトアウト
			out->r = ~( (~((in-1)->r) + ~((in +1)->r)) )/2;
			out->g = ~( (~((in-1)->g) + ~((in +1)->g)) )/2;
			out->b = ~( (~((in-1)->b) + ~((in +1)->b)) )/2;
			out++;
			in++;

		}
		out+=ogap+4;
		in+=igap+4;
	}
}

inline void hblur4(color_t* outPtr,
		  uint16_t owidth,
		  uint16_t oheight,
		  uint16_t ox,
		  uint16_t oy,
		  uint16_t twidth,
		  uint16_t theight,
		  const color_t* inPtr,
		  uint16_t iwidth,
		  uint16_t iheight,
		  uint16_t ix,
		  uint16_t iy) {

	if(clip(owidth, oheight, ox, oy, twidth, theight, iwidth, iheight, ix, iy) ==false)return;

	uint16_t ogap = owidth - twidth;
	uint16_t igap = iwidth - twidth;

	color_t* out = outPtr + ox+2 + oy*owidth ;
	const color_t* in = inPtr + ix+2 + iy*iwidth ;

	for(uint16_t y=theight; y!=0; y--) {
		for(uint16_t x=twidth-4; x!=0; x--) { //さりげなくダウンカウンタ

#if 1
			out->r = ~( (~((in-1)->r) + ~((in +1)->r)) + ~(in-2)->r + ~(in +2)->r)/4;
			out->g = ~( (~((in-1)->g) + ~((in +1)->g)) + ~(in-2)->g + ~(in +2)->g)/4;
			out->b = ~( (~((in-1)->b) + ~((in +1)->b)) + ~(in-2)->b + ~(in +2)->b)/4;
#else
			out->r = ((in-1)->r + (in +1)->r + (in-2)->r + (in +2)->r )/4;
			out->g = ((in-1)->g + (in +1)->g + (in-2)->g + (in +2)->g )/4;
			out->b = ((in-1)->b + (in +1)->b + (in-2)->b + (in +2)->b )/4;
#endif
			out++;
			in++;

		}
		out+=ogap+4;
		in+=igap+4;
	}
}

}; //syd

#endif //_RAWGRAPHICS_H_
 