#include <algorithm>
#include <boost/scoped_array.hpp>

#include "spline.hpp"

#define for if(false);else for

namespace syd {

using namespace std;
using namespace boost;


double Linear::get( double x ) const {
	if(is_dirty_) {
		throw "LinerInterpolation : data not normalized.";
	}

	vector<XY_pair>::const_iterator it = 
		upper_bound(table_.begin(), table_.end(), x, CompByKey<XY_pair>());

	if( table_.begin() <= it && it < table_.end() ) {
	
		const XY_pair& hi = *it;
		const XY_pair& lo = *(it-1);

		double dif = (hi.y - lo.y) / (hi.x - lo.x);
		return lo.y + dif*(x - lo.x);
		
	} else {
		throw "LinearInterpolation: given x is out of range.";
	}

}

double Linear::range_max() const {
	return max_;
}

double Linear::range_min() const {
	return min_;
}

Interpolation& Linear::normalize() {
	sort(table_.begin(), table_.end(), CompByKey<XY_pair>());
	max_ = table_.rbegin()->x;
	min_ = table_.begin()->x;
	is_dirty_=false;
	return *this;
}

Interpolation& Linear::operator<<( const XY_pair& src ) {
	table_.push_back(src);
	is_dirty_=true;
	return *this;
}

Interpolation& Linear::clear() {
	table_.clear();
	is_dirty_=true;
	return *this;
}

Linear::Linear() : is_dirty_(true) {
}

double Spline::range_max() const {
	return max_;
}

double Spline::range_min() const {
	return min_;
}

double Spline::get( double x ) const {
	if( is_dirty_ ) {
		throw "Spline: Interpolation data not normalized.";
	}
	
	vector<S>::const_iterator upper= 
		std::upper_bound( data_.begin(), data_.end(), x, CompByKey<S>() );

	if( data_.begin()+1 <= upper && upper < data_.end() ) {
		const S &hi = *upper;
		const S &lo = *(upper-1);

		double h = hi.x - lo.x;
		if( h == 0.0 ) {
			throw "Spline: Bad table data.";
		}

		double a = (hi.x - x) / h,
		  b = (x - lo.x) / h;
		
		// Spline Interpolation!!
		return a*lo.y + b*hi.y
				+ ((a*a*a-a)*lo.y2 + (b*b*b-b)*hi.y2) * h*h / 6.0;
	
	} else {
		throw "Spline: out of range.";
	}
}


Interpolation& Spline::operator<<( const XY_pair& src ) {
	is_dirty_=true;
	data_.push_back( S( src.x, src.y, 0 ) );
	return *this;
}


Interpolation& Spline::normalize() {
	if( data_.empty() ) return *this;

	std::sort( data_.begin(), data_.end(), CompByKey<S>() );
	int size = data_.size();
	
	boost::scoped_array<double> u( new double[size-1] );

	data_[0].y2 = 0.0;
	data_[size-1].y2 = 0.0;
	u[0]=0;
	u[size-2]=0;

	int i=1;
	for( vector<S>::iterator it = data_.begin()+1;
		 it != data_.end()-2;
		 it++ ) {

		S const& back  =*(it-1);
		S      & now   =*it;
		S const& front =*(it+1);
		
		double sig = (now.x-back.x) / (front.x-back.x);
		double p = sig*back.y2+2.0;
		now.y2= (sig-1.0)/p;
		u[i] = (front.y-now.y)/(front.x-now.x) - (now.y-back.y)/(now.x-back.x);
		u[i] = (6.0*u[i]/(front.x-back.x)-sig*u[i-1])/p;

		i++;
	}

	i=size-2; 
	for( vector<S>::iterator it = data_.end()-1;
		  it > data_.begin();
		  it--) {
		S      & now   =*it;		// 上とは逆！！(reverse_iteratorだから・・・
		S const& front =*(it-1);

		now.y2 = now.y2*front.y2+u[i];

		i--;
	}

	max_=data_.rbegin()->x;
	min_=data_.begin()->x;

	is_dirty_=false;
	return *this;
}


Interpolation& Spline::clear() {
	data_.clear();
	is_dirty_=true;
	return *this;
}

Spline::Spline() : is_dirty_(true) {
}


} // namespace syd

