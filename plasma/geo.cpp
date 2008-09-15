#include "geo.hpp"

namespace geo {

const geo::IntersectionInfo no_intersection;

inline double ABS(double x) { //fabsを使うべき
	return (x>=0.0)?x:-x;
}


const IntersectionInfo Triangle::intersect( const HalfLine& rayline ) const {

	using namespace std;

	const Triangle& tri=*this;
	const vector3d& origin = rayline.origin();
	const vector3d& dir = rayline.direction();
	vector3d tri_normal = tri.normal();

	// t : 直線の始点から三角形までの距離 -1<t<1 に 正規化 
	// t>0で半直線は三角形を含む無限平面と交差する
	double dot_n_d = dot( tri_normal, dir ),
		   t = dot( tri_normal, sub(tri.three_points()[0], origin) ) / dot_n_d;
	
	vector3d intersection( add(origin,scalar(dir,t)) ); // 交点を保持させる

	if( t > 0.0001 ) { // 0.0001。こうするとうまくいく - don't touch!
		const vector3d* const point = tri.three_points();
		vector2d v0,v1,v2;

		if( ABS(tri_normal.x()) < ABS(tri_normal.y()) ) {
			if( ABS(tri_normal.y()) < ABS(tri_normal.z()) ) {
				// (x<y<z) z軸に垂直な面(xy)に投影して考える
				v0 = vector2d( point[0].x()-intersection.x(), point[0].y()-intersection.y() );
				v1 = vector2d( point[1].x()-intersection.x(), point[1].y()-intersection.y() );
				v2 = vector2d( point[2].x()-intersection.x(), point[2].y()-intersection.y() );
			} else { 
				// (x,z<y) y軸に垂直な面(zx)に投影して考える
				v0 = vector2d( point[0].z()-intersection.z(), point[0].x()-intersection.x() );
				v1 = vector2d( point[1].z()-intersection.z(), point[1].x()-intersection.x() );
				v2 = vector2d( point[2].z()-intersection.z(), point[2].x()-intersection.x() );
			}
		} else if( ABS(tri_normal.x()) < ABS(tri_normal.z()) ) {
			// (y<x<z) z軸に垂直な面(xy)に投影して考える
			v0 = vector2d( point[0].x()-intersection.x(), point[0].y()-intersection.y() );
			v1 = vector2d( point[1].x()-intersection.x(), point[1].y()-intersection.y() );
			v2 = vector2d( point[2].x()-intersection.x(), point[2].y()-intersection.y() );
		} else {
			// (y,z<x) x軸に垂直な面(yz)に投影して考える
			v0 = vector2d( point[0].y()-intersection.y(), point[0].z()-intersection.z() );
			v1 = vector2d( point[1].y()-intersection.y(), point[1].z()-intersection.z() );
			v2 = vector2d( point[2].y()-intersection.y(), point[2].z()-intersection.z() );
		}

		// アホなコード 確か表裏判定だった気がする?? なんだっけ
		if( cross(v0,v1) < 0.0 ) {
			if( cross(v1,v2) < 0.0 ) {
				if( cross(v2,v0) < 0.0 ) {
					return IntersectionInfo(tri_normal, t);
				}
			}
		}

		//結局ウラも見える
		if( cross(v0,v1) > 0.0 ) {
			if( cross(v1,v2) > 0.0 ) {
				if( cross(v2,v0) > 0.0 ) {
					return IntersectionInfo(tri_normal, t);
				}
			}
		}

		return no_intersection;
		
	} else { // t <= 0.0
		return no_intersection;
	}

}


const IntersectionInfo Sphere::intersect( const HalfLine& rayline ) const {

	using namespace std;

	const Sphere& sphere = *this;
	//double dir_norm = norm( dir );
	const vector3d& origin = rayline.origin();
	const vector3d& dir = rayline.direction();
	const vector3d& sphere_center = sphere.center();
	const double sphere_center_norm = norm( sphere_center );
	const double origin_norm = norm( origin );
	const double r = sphere.radius();

	const double //A = dir_norm * dir_norm, // A=1!!!!!
		Bdiv2 = dot( dir, sub(origin,sphere_center) );

	if( Bdiv2>0.0 ) {
		return no_intersection;
	}
	const double C = sphere_center_norm*sphere_center_norm + origin_norm*origin_norm
						- 2*dot(sphere_center,origin) - r*r,
				Ddiv4 = Bdiv2*Bdiv2 - C; // Bdiv2*Bdiv2 - A*C;

	if( Ddiv4 < 0.0 ) {
		return no_intersection; // 交差しない
	} else {
		double sqrt_Ddiv4 = sqrt( Ddiv4 );
		double t_lesser = ( -Bdiv2 - sqrt_Ddiv4 ); // / A;

		if( t_lesser > 0.0 ) {
			vector3d normal = 
				scalar( sub( add(origin,scalar(dir,t_lesser)), sphere_center), 1.0/r );
			return IntersectionInfo(normal, t_lesser); // 外側が見えた
		} else {
			return no_intersection;
			// 内側が見えるかも!?
			/*double t_greater = ( -Bdiv2 + sqrt_Ddiv4 ); // / A;

			if( t_greater > 0.0 ) {
				return true;
			} else {
				return false;
			}*/
		}

	}

}


} // namespace geo

