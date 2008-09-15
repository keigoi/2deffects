#ifndef _PHOTOENV_HPP_
#define _PHOTOENV_HPP_

#include <boost/scoped_ptr.hpp>

#include "geo.hpp"
#include "color.hpp"
#include "spline.hpp"


namespace raytrace {


const double PI=3.1415926358979;

class Lambda2RgbTable {
public:
	enum {min = 360, max=780};

private:
	raytrace::color_t rgbtable_[max-min+1];

public:
	raytrace::color_t get_rgb( int lambda ) {
		if( min<=lambda &&  lambda <= max) {
			return rgbtable_[lambda-min];
		} else {
			return raytrace::color_t(0,0,0);
		}

	}

public:
	Lambda2RgbTable( std::istream &i ) {
		
		int l;
		while(!i.eof()&&i.good()) {
			i >> l;
			if( min <= l && l <= max ) {
				char c;
				double r,g,b;
				i >>c>>r>>c>>g>>c>>b;
				rgbtable_[l-min].r(r/255).g(g/255).b(b/255);
			}
		}
	
	}

};


class Refraction2LambdaTable {

	syd::Spline spline_;

public:
	double get_lambda( double refraction_index ) {
		if( spline_.range_min() < refraction_index && refraction_index < spline_.range_max() ) {
	
			double x = spline_.get(refraction_index);
			if( 300 < x && x < 800 ) {
				return x;
			}
		}

		// otherwise
		return 800;	
	}

	Refraction2LambdaTable( std::istream &i ) {

		while( !i.eof() && i.good() ) {
			char c;
			double refraction_index, lambda;

			i >> refraction_index >> c >> lambda;
			spline_ << syd::XY_pair(refraction_index, lambda);
		}

		spline_.normalize();

	}
};
	
class Angle2RefractionTable {

	syd::Spline spline_;
	
public:
	Angle2RefractionTable() {

		for( double nn=10; nn<20; nn+=0.5 ) {
			double n=nn/10;
			double angle =
				4*asin(sqrt( (4/(n*n)-1)/3 )) -2*asin(sqrt( (4-n*n)/3 ));
		
			spline_ << syd::XY_pair( angle, n );
		}
		spline_.normalize();
	}

	double get_refractive_index( double angle ) {
		if(spline_.range_min() <= angle && angle < spline_.range_max() ) {
			return spline_.get(angle);
		} else {
			return 0;
		}
	}

};
	


class Light {
public:
	virtual const geo::vector3d ray_dir_at( const geo::vector3d& p ) const =0;
	virtual double intensity_at( const geo::vector3d& p) const =0;

public:
	virtual Light& intensity( double i ) =0;

	virtual ~Light(){}
};


class ParallelLight : public Light {
public:
	geo::vector3d ray_dir_;
	double intensity_;

public:
	virtual const geo::vector3d ray_dir_at( const geo::vector3d& p ) const {
		return ray_dir_; //‚Ç‚±‚Å‚à“¯‚¶•ûŒü(•½sŒõŒ¹
	}
	virtual double intensity_at( const geo::vector3d& p ) const {
		return intensity_;
	}

public:
	virtual Light& intensity( double i ) { intensity_ = i; return *this; }
	ParallelLight& ray_dir( const geo::vector3d& v ) { ray_dir_ = geo::normalize(v); return *this; }

	ParallelLight( const geo::vector3d& ray_dir, double intensity) :ray_dir_(geo::normalize(ray_dir)), intensity_(intensity) {}

};


class PointLight : public Light {
public:
	geo::vector3d center_;
	double intensity_;

public:
	virtual const geo::vector3d ray_dir_at( const geo::vector3d& p ) const {
		return geo::normalize( geo::sub( p, center_ ) );
	}
	virtual double intensity_at( const geo::vector3d& p ) const {
		double f = 1.0/( geo::norm_square(geo::sub(p,center_)));
		return intensity_*f;
	}

public:
	virtual Light& intensity( double i ) { intensity_ = i; return *this; }
	virtual PointLight& center( const geo::vector3d& p ) { center_ = p; return *this; }

public:
	PointLight( const geo::vector3d& c, double inten ) : center_(c), intensity_(inten) {}
};



class PhotoEnvironment {
	boost::scoped_ptr<Angle2RefractionTable> a2rtable_;
	boost::scoped_ptr<Refraction2LambdaTable> r2ltable_;
	boost::scoped_ptr<Lambda2RgbTable> l2rgbtable_;
	boost::scoped_ptr<Light> light_;
	bool rainbow_;
	double rainbow_intensity_;
	double ambient_;
	color_t skycolor_;


public:
	double ambient() const {
		return ambient_;
	}

	const Light& light() const {
		return *light_;
	}

	color_t get_skycolor( const geo::vector3d& eyeline_direction ) const {
		
		color_t ret;

		if(rainbow_) {
			using namespace geo;
			double dot_l_r = dot(eyeline_direction, light_->ray_dir_at(vector3d(0,0,0)));
			double radian = acos(dot_l_r);
			
			double refract = a2rtable_->get_refractive_index(radian);
			double lambda = r2ltable_->get_lambda(refract);
			ret = l2rgbtable_->get_rgb(lambda);

			ret.r(ret.r()*rainbow_intensity_ + skycolor_.r() );
			ret.g(ret.g()*rainbow_intensity_ + skycolor_.g() );
			ret.b(ret.b()*rainbow_intensity_ + skycolor_.b() );
		}
		return ret;
	}

public:
	PhotoEnvironment& skycolor( const color_t& col ) {
		skycolor_=col;
		return *this;
	}

public:
	PhotoEnvironment& light( Light* light ) {
		light_.reset(light);
		return *this;
	}

	PhotoEnvironment& rainbow_factor( Angle2RefractionTable* a2r, Refraction2LambdaTable* r2l, Lambda2RgbTable* l2rgb, double intensity ) {
		a2rtable_.reset(a2r);
		r2ltable_.reset(r2l);
		l2rgbtable_.reset(l2rgb);
		rainbow_intensity_ = intensity;
		return *this;
	}

	PhotoEnvironment& ambient( double amb ) {
		ambient_=amb;
		return *this;
	}

	PhotoEnvironment& rainbow( bool flag ) {
		rainbow_=flag;
		return *this;
	}

public:
	PhotoEnvironment() : rainbow_(false), ambient_(0.0), rainbow_intensity_(0.0) {
	}

};

} // namespace raytrace


#endif //!defined(_PHOTOENV_HPP_)
