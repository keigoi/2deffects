//#include <boost/cstdint.hpp>

#include "raytracecore.hpp"
#include "utils/color.h"

namespace raytrace {


color_t RaytraceCore::raytrace( const geo::HalfLine &eyeline, unsigned depth ) const{
		
	using namespace geo;
	using namespace geo;

	// 視線と交差する最も近い物体の取得
	std::pair<IntersectionInfo, const GeneralPlane*>
		res = world_.get_intersection( eyeline );

	
	// 交差する物体が無ければ、空の色になる。
	if( !res.first.valid() ) {
		color_t col = phenv_.get_skycolor( eyeline.direction() );
		return col;
	}


	// 計算に必要な設定-------------------------------------
	const IntersectionInfo& info = res.first;
	const GeneralPlane& plane = *(res.second);
	double distance = info.distance();

	// 視線に関して
	const vector3d hit_ptr = eyeline.get_point(distance);
	const vector3d eye_dir = minus(eyeline.direction());

	// 光源に関して
	const Light& light = phenv_.light();
	double light_intensity = light.intensity_at(hit_ptr);
	const vector3d& light_dir=minus(light.ray_dir_at(hit_ptr));

	// 物体に関して
	vector3d normal(info.normal());
	double dot_e_n = dot(normal, eye_dir);  
	if(dot_e_n<0) { // 法線方向が負の場合、ベクトルをひっくり返す。
		normal = minus(normal);
		dot_e_n=-dot_e_n;
	}

	// 物体の特性（色とか）に関して
	const PlaneTraits& traits=plane.traits();
	const color_t col = traits.color();



	// ここから光の計算-------------------------------------
	// 環境光の計算
	double amb_fac = phenv_.ambient() * traits.ambient_refrectance();

	// 拡散反射光とスペキュラの計算
	double dif_fac=0.0;
	double shade_fac=0;
	double spc_white_fac=0;

	double dot_n_l = dot(normal, light_dir);
	if(dot_n_l>0) { // 面が光源の方向を向いている

		// 影の判定
		if( world_.get_intersection(HalfLine(hit_ptr,light_dir)).first.valid()==true ) {
			shade_fac=1.0;
		}

		// 拡散反射光
		dif_fac = ( 1.0 - shade_fac) * light_intensity * dot_n_l * traits.diffuse_refrectance();
		
		double spc_power=200;
		// 反射光の方向
		vector3d light_refrect = normalize(sub( scalar( normal, 2*dot(normal,light_dir) ), light_dir ));
		double s=dot(eye_dir,light_refrect);

		if(s>0.0) { // スペキュラがある。
			spc_white_fac = ( 1.0 - shade_fac) * pow(s,spc_power) * light_intensity;
			spc_white_fac *= traits.specular_refrectance();
		}
	}

	// 鏡面反射光の計算・・・再帰部分
	color_t spc_fac;
	if(depth != 0) {
		vector3d eye_refrect = normalize(sub( scalar( normal, 2*dot(normal,eye_dir) ), eye_dir ));
		HalfLine next_eyeline( hit_ptr, eye_refrect );

		spc_fac = raytrace( next_eyeline,depth-1 );

		spc_fac.r(spc_fac.r()*traits.specular_refrectance());
		spc_fac.g(spc_fac.g()*traits.specular_refrectance());
		spc_fac.b(spc_fac.b()*traits.specular_refrectance());
	}

	// 結果の計算
	color_t ret;
	
	ret.r (col.r()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.r())
		.g (col.g()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.g())
		.b (col.b()*(amb_fac + dif_fac) + spc_white_fac + spc_fac.b());

	return ret;

}

} // naemspace raytrace

