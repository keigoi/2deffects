//#include <boost/cstdint.hpp>

#include "raytracecore.hpp"
#include "utils/color.h"

namespace raytrace {


color_t RaytraceCore::raytrace( const geo::HalfLine &eyeline, unsigned depth ) const{
		
	using namespace geo;
	using namespace geo;

	// �����ƌ�������ł��߂����̂̎擾
	std::pair<IntersectionInfo, const GeneralPlane*>
		res = world_.get_intersection( eyeline );

	
	// �������镨�̂�������΁A��̐F�ɂȂ�B
	if( !res.first.valid() ) {
		color_t col = phenv_.get_skycolor( eyeline.direction() );
		return col;
	}


	// �v�Z�ɕK�v�Ȑݒ�-------------------------------------
	const IntersectionInfo& info = res.first;
	const GeneralPlane& plane = *(res.second);
	double distance = info.distance();

	// �����Ɋւ���
	const vector3d hit_ptr = eyeline.get_point(distance);
	const vector3d eye_dir = minus(eyeline.direction());

	// �����Ɋւ���
	const Light& light = phenv_.light();
	double light_intensity = light.intensity_at(hit_ptr);
	const vector3d& light_dir=minus(light.ray_dir_at(hit_ptr));

	// ���̂Ɋւ���
	vector3d normal(info.normal());
	double dot_e_n = dot(normal, eye_dir);  
	if(dot_e_n<0) { // �@�����������̏ꍇ�A�x�N�g�����Ђ�����Ԃ��B
		normal = minus(normal);
		dot_e_n=-dot_e_n;
	}

	// ���̂̓����i�F�Ƃ��j�Ɋւ���
	const PlaneTraits& traits=plane.traits();
	const color_t col = traits.color();



	// ����������̌v�Z-------------------------------------
	// �����̌v�Z
	double amb_fac = phenv_.ambient() * traits.ambient_refrectance();

	// �g�U���ˌ��ƃX�y�L�����̌v�Z
	double dif_fac=0.0;
	double shade_fac=0;
	double spc_white_fac=0;

	double dot_n_l = dot(normal, light_dir);
	if(dot_n_l>0) { // �ʂ������̕����������Ă���

		// �e�̔���
		if( world_.get_intersection(HalfLine(hit_ptr,light_dir)).first.valid()==true ) {
			shade_fac=1.0;
		}

		// �g�U���ˌ�
		dif_fac = ( 1.0 - shade_fac) * light_intensity * dot_n_l * traits.diffuse_refrectance();
		
		double spc_power=200;
		// ���ˌ��̕���
		vector3d light_refrect = normalize(sub( scalar( normal, 2*dot(normal,light_dir) ), light_dir ));
		double s=dot(eye_dir,light_refrect);

		if(s>0.0) { // �X�y�L����������B
			spc_white_fac = ( 1.0 - shade_fac) * pow(s,spc_power) * light_intensity;
			spc_white_fac *= traits.specular_refrectance();
		}
	}

	// ���ʔ��ˌ��̌v�Z�E�E�E�ċA����
	color_t spc_fac;
	if(depth != 0) {
		vector3d eye_refrect = normalize(sub( scalar( normal, 2*dot(normal,eye_dir) ), eye_dir ));
		HalfLine next_eyeline( hit_ptr, eye_refrect );

		spc_fac = raytrace( next_eyeline,depth-1 );

		spc_fac.r(spc_fac.r()*traits.specular_refrectance());
		spc_fac.g(spc_fac.g()*traits.specular_refrectance());
		spc_fac.b(spc_fac.b()*traits.specular_refrectance());
	}

	// ���ʂ̌v�Z
	color_t ret;
	
	ret.r (col.r()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.r())
		.g (col.g()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.g())
		.b (col.b()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.b());

	return ret;

}

} // naemspace raytrace

