#include <boost/cstdint.hpp>

#include "utils/bitmap.h"
#include "raytrace.hpp"
#include "raytracecore.hpp"

namespace raytrace {

namespace {

	inline double saturate(double v) {
		
		if(v>1.0)return 1.0;
		if(v<0.0)return 0.0;
		return v;
	}

	typedef struct color_tag_rgba {
		color_tag_rgba() : b(0), g(0), r(0), a(0) {};
		color_tag_rgba( boost::uint8_t r_, boost::uint8_t g_, boost::uint8_t b_, boost::uint8_t a_)
			: b(b_), g(g_), r(r_), a(a_) {};
		boost::uint8_t b;
		boost::uint8_t g;
		boost::uint8_t r;
		boost::uint8_t a;
	} COLOR;

} // noname namespace

syd::Bitmap& bmp=*(syd::Bitmap*)(0); //!?? 何かのテストか。



//syd::Bitmap& 
syd::Bitmap&
raytrace( const ProjectionSystem& proj,
					   const syd::Rect& rect,
					   const RaytraceCore& core,
					   unsigned depth,
					   syd::Bitmap& bmp ) {
	
	std::cout << "rendering start..." << std::endl;

	COLOR* ptr = bmp.getPtr(ptr);
	int w=bmp.width(),
	    h=bmp.height();


	double xscale = (double)rect.width() / bmp.width(),
		   yscale = (double)rect.height() / bmp.height();


	for( int y=0; y<h; y++ ) {
		for( int x=0; x<w; x++ ) {

			double u = rect.topLeft().x() + xscale * x,
				   v = rect.topLeft().y() + yscale * y;

			geo::HalfLine eyeline = proj.get_ray_line(u,v);

			color_t col = core.raytrace(eyeline, depth);

			ptr->r=saturate(col.r())*255;
			ptr->g=saturate(col.g())*255;
			ptr->b=saturate(col.b())*255;

			ptr++;
		}

		std::cout << "\b\b\b\b" << std::setw(3) << (int)( (double)y/h*100 ) << "%";
	}
	std::cout << "\b\b\b\b" << "100%" << "drawn." << std::endl;
		
	return bmp;

}


} // namespace raytrace
