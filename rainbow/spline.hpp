#ifndef __SYD__SPLINE__HPP__
#define __SYD__SPLINE__HPP__

#include <vector>

namespace syd {

template <class T>
class CompByKey { // 変換不可能なファンクタ、Effective STL No.23,40 参照
	typedef typename T::key_type key_type;
public:
	bool operator()( const T& lhs, const T& rhs ) const {  // ソート用
		return keyLess( lhs.key(), rhs.key() );
	}

	bool operator()( const T& lhs, const key_type rhs ) const { // サーチ用１
		return keyLess( lhs.key(), rhs );
	}

	bool operator()( const key_type& lhs, const T& rhs ) const { // サーチ用２
		return keyLess( lhs, rhs.key() );
	}

private:
	bool keyLess( const key_type& lhs, const key_type& rhs ) const {
		return lhs < rhs;
	}
};


struct XY_pair {
	double x,y;

	typedef double key_type;
	double key() const { return x; }

	XY_pair() {}
	XY_pair(double x_, double y_) : x(x_), y(y_) {}
};


class Interpolation {
public:
	virtual double get( double x ) const =0;
	virtual double range_max() const =0;
	virtual double range_min() const =0;

	virtual Interpolation& normalize() =0;
	virtual Interpolation& operator<<( const XY_pair& src ) =0;
	virtual Interpolation& clear() =0;
};


class Linear : public Interpolation { // Linear Interpolation

	std::vector< XY_pair > table_;
	bool is_dirty_;
	double max_,min_;

public:
	double get( double x ) const;
	double range_max() const;
	double range_min() const;

public:
	Interpolation& normalize();
	Interpolation& operator<<( const XY_pair& src );
	Interpolation& clear();


public:
	Linear();
};




class Spline : public Interpolation { // Spline Interpolation

	struct S {
		double x,y,y2;

		typedef double key_type;
		double key() const { return x; }

		S(){}
		S( double x_, double y_, double y2_ ) : x(x_), y(y_), y2(y2_) {}
	};

	std::vector< S > data_;
	bool is_dirty_;
	double max_,min_; 

public:
	double range_max() const;
	double range_min() const;
	double get( double x ) const;

public:
	Interpolation& operator<<( const XY_pair& src );
	Interpolation& normalize();
	Interpolation& clear();
	
public:
	Spline();

};

} // namespace syd

#undef for

#endif //__SYD__SPLINE__HPP__
