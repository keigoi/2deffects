#ifndef _PLANES_HPP_
#define _PLANES_HPP_

#include <boost/scoped_ptr.hpp>
#include <vector>

#include "geo.hpp"
#include "color.hpp"


namespace raytrace {



class PlaneTraits {
	double ambient_refrectance_;
	double diffuse_refrectance_;
	double specular_refrectance_;
	color_t color_;
public:
	double ambient_refrectance() const { return ambient_refrectance_; }
	double diffuse_refrectance() const { return diffuse_refrectance_; }
	double specular_refrectance() const { return specular_refrectance_; }
	const color_t& color() const { return color_; }

public:
	PlaneTraits& ambient_refrectance( double val ) { ambient_refrectance_ = val; return *this; }
	PlaneTraits& diffuse_refrectance( double val ) { diffuse_refrectance_ = val; return *this; }
	PlaneTraits& specular_refrectance( double val ) { specular_refrectance_ = val; return *this; }
	PlaneTraits& color( const color_t& val ) { color_ = val; return *this; }

public:
	PlaneTraits() : ambient_refrectance_(0.0), diffuse_refrectance_(0.0),specular_refrectance_(0.0) {}
	PlaneTraits( double amb, double dif, double spc, color_t col ) 
		: ambient_refrectance_(amb), diffuse_refrectance_(dif), specular_refrectance_(spc), color_(col) {}
};
	

class GeneralPlane : public geo::Shape {
	boost::scoped_ptr<geo::Shape> shape_;
	PlaneTraits traits_;

public:
	const PlaneTraits& traits() const { return traits_; }
	const geo::Shape& shape() const { return *shape_; }
	const geo::IntersectionInfo intersect( const geo::HalfLine& line ) const {
		if(shape_.get()!=0) {
			return shape_->intersect(line);
		}
		return geo::no_intersection;
	}

public:
	GeneralPlane& traits( const PlaneTraits& t ) { traits_ = t; return *this; }

public:
	geo::Shape* clone() const {
		return new GeneralPlane( *shape_, traits_ );
	}

public:
	GeneralPlane() : shape_(0) {}
	
	GeneralPlane( const geo::Shape& s, const PlaneTraits& t ) 
		: shape_(s.clone()), traits_(t) {}

	GeneralPlane( const GeneralPlane& p ) 
		: shape_(p.shape_->clone()), traits_(p.traits_) {
	}

	GeneralPlane& operator=( const GeneralPlane& rhs ) {
		if(&rhs!=this){
			shape_.reset(rhs.shape_->clone());
			traits_=rhs.traits_;
		}
		return *this;
	}
};



class World {

	typedef std::vector<GeneralPlane> PlaneContainer;
	PlaneContainer planes_;

public:
	const std::pair<geo::IntersectionInfo, const GeneralPlane*>
	get_intersection( const geo::HalfLine& line ) const {
		
		using namespace geo;

		const GeneralPlane* pplane;
		IntersectionInfo info(no_intersection);
		
		PlaneContainer::const_iterator it=planes_.begin();

		double distance=999999;
		for(; it != planes_.end(); it++) {
			IntersectionInfo temp = it->intersect( line );
			if(temp.valid()) {
				if(temp.distance() < distance) { // ここに表裏判定の部分も入れる必要がある。
					info=temp;
					distance = temp.distance();
					pplane = &(*it);
				}
			}
		}

		return std::pair<IntersectionInfo, const GeneralPlane*>(info, pplane);
	}

public:
	World& set_plane( const GeneralPlane& plane ) {
		planes_.push_back( plane );
		return *this;
	}
};

} // namespace raytrace

#endif //!defined(_PLANES_HPP_)
