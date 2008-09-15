#ifndef _COLOR_HPP_
#define _COLOR_HPP_

namespace raytrace {

class color_t {
	double r_;
	double g_;
	double b_;

public:
	double r() const{ return r_; }
	double g() const{ return g_; }
	double b() const{ return b_; }

	color_t& r( double c ) { r_ = c; return *this; }
	color_t& g( double c ) { g_ = c; return *this; }
	color_t& b( double c ) { b_ = c; return *this; }

public:
	color_t() : r_(0.0),g_(0.0),b_(0.0) {}
	color_t( double rr, double gg, double bb ) 
		: r_(rr), g_(gg), b_(bb) {}
};


} // namespace raytrace

#endif //!defined(_COLOR_HPP_)
