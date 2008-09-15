#ifndef __SYD__GEO__HPP__
#define __SYD__GEO__HPP__

#include <iosfwd>
#include <iomanip>
#include <cmath>

namespace geo {

class vector3d {
	friend double dot( const vector3d& lhs, const vector3d& rhs );
	friend const vector3d cross( const vector3d& lhs, const vector3d& rhs ) ;
	friend const vector3d add( const vector3d& lhs, const vector3d& rhs ) ;
	friend const vector3d sub( const vector3d& lhs, const vector3d& rhs ) ;
	friend const vector3d minus( const vector3d& v ) ;
	friend const vector3d scalar( const vector3d& lhs, double rhs ) ;
	friend double norm_square( const vector3d& v ) ;
	friend const vector3d normalize( const vector3d& v ) ;
	friend std::ostream& operator << (std::ostream& lhs, const vector3d& rhs) ;

private:
	double x_,y_,z_;

public:
	double x() const { return x_; }
	double y() const { return y_; }
	double z() const { return z_; }
public:
	vector3d& operator=( const vector3d& v ) {
		if(&v!=this) {
			x_=v.x_;
			y_=v.y_;
			z_=v.z_;
		}
		return *this;
	}
	vector3d( const vector3d& v ) {
		x_=v.x_;
		y_=v.y_;
		z_=v.z_;
	}
	vector3d() : x_(0.0),y_(0.0),z_(0.0) {}
	vector3d( double xx, double yy, double zz ) : x_(xx),y_(yy),z_(zz) {
	}
};


class vector2d {
	friend double dot( const vector2d& lhs, const vector2d& rhs );
	friend double cross( const vector2d& lhs, const vector2d& rhs ) ;
	friend const vector2d add( const vector2d& lhs, const vector2d& rhs ) ;
	friend const vector2d sub( const vector2d& lhs, const vector2d& rhs ) ;
	friend const vector2d minus( const vector2d& v ) ;
	friend const vector2d scalar( const vector2d& lhs, double rhs ) ;
	friend double norm_square( const vector2d& v ) ;
	friend const vector2d normalize( const vector2d& v ) ;
	friend std::ostream& operator << (std::ostream& lhs, const vector2d& rhs) ;

private:
	double x_,y_;
public:
	double x() const { return x_; }
	double y() const { return y_; }
public:
	vector2d() : x_(0.0),y_(0.0) {}
	vector2d( double xx, double yy ) : x_(xx), y_(yy) {}
};


inline double dot( const vector2d& lhs, const vector2d& rhs ) {
	return lhs.x_*rhs.x_ + lhs.y_*rhs.y_;
}

inline double cross( const vector2d& lhs, const vector2d& rhs ) {
	return lhs.x_*rhs.y_ - lhs.y_*rhs.x_;
}

inline const vector2d add( const vector2d& lhs, const vector2d& rhs ) {
	return vector2d( lhs.x_+rhs.x_, lhs.y_+rhs.y_ );
}

inline const vector2d sub( const vector2d& lhs, const vector2d& rhs ) {
	return vector2d( lhs.x_-rhs.x_, lhs.y_-rhs.y_ );
}

inline const vector2d minus( const vector2d& v ) {
	return vector2d(-v.x_, -v.y_ );
}

inline const vector2d scalar( const vector2d& lhs, double rhs ) {
	return vector2d( lhs.x_*rhs, lhs.y_*rhs );
}

inline double norm_square( const vector2d& v ) {
	return  v.x_*v.x_ + v.y_*v.y_;
}

inline double norm( const vector2d& v ) {
	using namespace std;
	return sqrt( norm_square(v) );
}

inline const vector2d normalize( const vector2d& v ) {
	return scalar(v, 1/norm(v) );		
}

inline std::ostream& operator << (std::ostream& lhs, const vector2d& rhs) {
	using namespace std;
	return lhs << setw(9) << rhs.x_ << setw(9) << rhs.y_;
}


inline double dot( const vector3d& lhs, const vector3d& rhs ) {
	return lhs.x_*rhs.x_ + lhs.y_*rhs.y_ + lhs.z_*rhs.z_;
}

inline const vector3d cross( const vector3d& lhs, const vector3d& rhs ) {
	return vector3d( lhs.y_*rhs.z_ - lhs.z_*rhs.y_,
					 lhs.z_*rhs.x_ - lhs.x_*rhs.z_,
					 lhs.x_*rhs.y_ - lhs.y_*rhs.x_ );
}

inline const vector3d add( const vector3d& lhs, const vector3d& rhs ) {
	return vector3d( lhs.x_+rhs.x_, lhs.y_+rhs.y_, lhs.z_+rhs.z_ );
}

inline const vector3d sub( const vector3d& lhs, const vector3d& rhs ) {
	return vector3d( lhs.x_-rhs.x_, lhs.y_-rhs.y_, lhs.z_-rhs.z_ );
}

inline const vector3d minus( const vector3d& v ) {
	return vector3d(-v.x_, -v.y_, -v.z_);
}

inline const vector3d scalar( const vector3d& lhs, double rhs ) {
	return vector3d( lhs.x_*rhs, lhs.y_*rhs, lhs.z_*rhs );
}

inline double norm_square( const vector3d& v ) {
	return v.x_*v.x_ + v.y_*v.y_ + v.z_*v.z_;
}


inline double norm( const vector3d& v ) {
	using namespace std;
	return sqrt( norm_square(v) );
}

inline const vector3d normalize( const vector3d& v ) {
	return scalar(v, 1/norm(v) );		
}

inline std::ostream& operator << (std::ostream& lhs, const vector3d& rhs) {
	using namespace std;
	return lhs << setw(9) << rhs.x_ << setw(9) << rhs.y_ << setw(9) << rhs.z_;
}


class IntersectionInfo {
	bool valid_;
	vector3d normal_;
	double distance_;

public:
	const vector3d& normal() const { return normal_; }  // 交点での面の法線
	double distance() const { return distance_; } // 直線の始点から交点までの距離
	bool valid() const { return valid_; } // 交差してるかどうか

public:
	IntersectionInfo( const vector3d& normal, double distance ) 
		: normal_(normal), distance_(distance), valid_(true) {}
	IntersectionInfo() : distance_(0.0), valid_(false) {}

public:
	IntersectionInfo( const IntersectionInfo& info ) 
		: valid_(info.valid_), normal_(info.normal_), distance_(info.distance_) {}
	IntersectionInfo& operator=( const IntersectionInfo& info ) {
		if(&info!=this) {
			valid_ = info.valid_;
			normal_ = info.normal_;
			distance_ = info.distance_;
		}
		return *this;

	}

};

extern const IntersectionInfo no_intersection;




class HalfLine { // 半直線
	vector3d origin_;
	vector3d direction_;
public:
	const vector3d& origin() const { return origin_; }
	const vector3d& direction() const { return direction_; }

	const vector3d get_point( double distance ) const {
		return add( origin_, scalar(direction_, distance) );
	}
public:
	HalfLine( const vector3d& origin, const vector3d& direction )
		: origin_(origin), direction_(normalize(direction)) {}
	HalfLine() {}
};
 

//
class Shape {  // 半直線との交差判定が可能なあらゆる種類の３Ｄ幾何形状を表す純粋仮想基底クラス。

public:
	virtual const IntersectionInfo intersect( const HalfLine& line ) const =0;
	virtual Shape* clone() const =0;

public:
	virtual ~Shape() {};
};

class Triangle : public Shape { // p0, p1, p2 が右回りに見える方向が表

	vector3d p_[3];

public:
	const vector3d* const three_points() const { return this->p_; }
	const vector3d normal() const
		{ return normalize( cross(sub(p_[2],p_[0]), sub(p_[1],p_[0])) ); }
	const IntersectionInfo intersect( const HalfLine& line ) const;
	
	Shape* clone() const {
		return new Triangle(*this);
	}

	Triangle& turnover() {
		vector3d temp = p_[0];
		p_[0]=p_[1];
		p_[1]=temp;
		return *this;
	}

	const vector3d barycenter() const {
		return scalar( add( add(p_[0],p_[1]),p_[2] ),0.333333333333333);
	}
public:
	Triangle() {}
	Triangle( const vector3d& p0, const vector3d& p1, const vector3d& p2 ) {
		p_[0] = p0;
		p_[1] = p1;
		p_[2] = p2;
	}
	Triangle( const Triangle& tri ) {
		p_[0] = tri.p_[0];
		p_[1] = tri.p_[1];
		p_[2] = tri.p_[2];
	}
	const Triangle& operator=( const Triangle& tri ) {
		if( &tri != this ) {
			p_[0] = tri.p_[0];
			p_[1] = tri.p_[1];
			p_[2] = tri.p_[2];
		}
 		return *this;
	}
};


class Sphere : public Shape {
	vector3d center_;
	double r_;

public:
	const vector3d& center() const { return center_; }
	double radius() const { return r_; }
	const IntersectionInfo intersect( const HalfLine& line ) const;

	Shape* clone() const {
		return new Sphere(*this);
	}

public:
	Sphere() : r_(0.0) {}
	Sphere( const Sphere& s ) {
		center_ = s.center_;
		r_ = s.r_;
	}

	Sphere( const vector3d& c, double r ) :center_(c),r_(r) {}
};


} // namespace geo


#endif // (!defined(__SYD__GEO__HPP__))
