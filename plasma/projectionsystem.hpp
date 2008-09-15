#ifndef _PROJECTIONSYSTEM_HPP_
#define _PROJECTIONSYSTEM_HPP_

#include "geo.hpp"

namespace raytrace {


class ProjectionSystem {
	
	geo::vector3d plane_pos_;	// ���e�ʂ̒��S
	geo::vector3d u_dir_;		// ���e�ʏ��u����; �P�ʃx�N�g��
	geo::vector3d v_dir_;		// ���e�ʏ��v����; �P�ʃx�N�g��
	double camera_dist_;	// ���e�ʂ���J�����܂ł̋���

	geo::vector3d temp_camera_pos_;

protected:
	void reset_camera_pos_() {
		using namespace geo;
		geo::vector3d plane_normal = geo::normalize( geo::cross(u_dir_, v_dir_) ); 
		temp_camera_pos_ = geo::add( plane_pos_, geo::scalar(plane_normal, camera_dist_) ); // �J�����͏��uv���ʂ�\���猩��
	}

public:
	ProjectionSystem& plane_pos( const geo::vector3d& v ) { plane_pos_=v; reset_camera_pos_(); return *this; }
	ProjectionSystem& u_dir( const geo::vector3d& v ) { u_dir_=v; reset_camera_pos_(); return *this; }
	ProjectionSystem& v_dir( const geo::vector3d& v ) { v_dir_=v; reset_camera_pos_(); return *this; }
	ProjectionSystem& camera_dist( double d ) { 
		camera_dist_=d;
		reset_camera_pos_();
		return *this;
	}

public:
	const geo::vector3d& plane_pos() const { return plane_pos_; }
	const geo::vector3d& u_dir() const { return u_dir_; }
	const geo::vector3d& v_dir() const { return v_dir_; }
	double camera_dist() const { return camera_dist_; }
	const geo::vector3d camera_pos() const {
		return temp_camera_pos_;
	}
	const geo::vector3d camera_dir() const { return geo::normalize( geo::sub(plane_pos_,camera_pos()) ); }

public:
	const geo::HalfLine get_ray_line( double u, double v ) const {
		geo::vector3d pos_on_plane = 
			geo::add( plane_pos_, geo::add(geo::scalar(u_dir_,u),geo::scalar(v_dir_, v)) );

		return geo::HalfLine(temp_camera_pos_, geo::normalize(geo::sub(pos_on_plane,temp_camera_pos_)) );
	}

public:
	ProjectionSystem() : camera_dist_(0.0){ }

};

} //  namespace raytrace

#endif //!defined(_PROJECTIONSYSTEM_HPP_)
