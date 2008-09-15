#include "utils/winutils/dibsection.h"
#include "utils/color.h"
#include "interface.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include <ctime>
#include <cmath>


#if 0//defined(__SWIG__)
#define  STDCALL_
#else
#define STDCALL_ __stdcall
#endif

#define for if(false);else for

using namespace geo;


namespace {
	raytrace::ProjectionSystem proj_;
	raytrace::PhotoEnvironment phenv_;
	syd::Rect proj_area_;

bool check() { // 正確なチェックじゃあない。
	
	using namespace std;

	if( proj_.camera_dist() > 0.0 &&
		norm(proj_.u_dir()) > 0.0 &&
		norm(proj_.v_dir()) > 0.0) {

		if( phenv_.ambient() >= 0.0 ) {

			if( proj_area_.size() !=0.0 ) {
				cout<< proj_area_.topLeft().x() << ","
					<< proj_area_.topLeft().y() << ","
					<< proj_area_.bottomRight().x() << ","
					<< proj_area_.bottomRight().y()
					<< endl;
				return true;
			}
		} else {
			cout << phenv_.ambient() << ","
				 << phenv_.get_skycolor(vector3d(1,0,0)).r() << ","
				 << phenv_.get_skycolor(vector3d(1,0,0)).g() << ","
				 << phenv_.get_skycolor(vector3d(1,0,0)).b() << ","
				 << phenv_.light().intensity_at(vector3d(0,0,0)) << endl;
		}
	}
	return false;
}

} // noname namespace;



inline double rand500() {
	using namespace std;
	return 500.0-1000.0*rand()/RAND_MAX;
}

inline double rand1() {
	using namespace std;
	return (double)rand()/RAND_MAX*0.6+0.4;
}


raytrace::World* new_random_world() {

	using namespace raytrace;
	using namespace geo;

	typedef vector3d v3d;
	typedef GeneralPlane gp;

	//３Ｄワールド系
	World* pworld=new World;
	World& world=*pworld;

	PlaneTraits traits( 0.3, 0.9, 0.55, color_t(0,0,0) );
	
	for(int i=0; i<2; i++) {
		v3d p0 = v3d(rand500()*2.4,rand500()*1.2,-rand500()/2-1500);
		Triangle tri(p0,
					add(p0,v3d(rand500()*1.6,rand500()*2,rand500()*0.7)),
					add(p0,v3d(rand500()*1.6,rand500()*2,rand500()*0.7)) );
		v3d normal = tri.normal();
		world.set_plane(
				gp( tri,
					traits.color( color_t(rand1(),rand1(),rand1()) )
				   )
			  );
	}

	traits.specular_refrectance(0.55);
	for(int i=5; i<10; i++) {
		int r=(rand500()+500)/4;
		int z=-r-(rand500()/2+800)/2;
		world.set_plane(
				gp( Sphere( v3d(rand500(),rand500(),z), r ),
					traits.color( color_t(rand1(),rand1(),rand1()) )
				  )
			  );
	}

	int x=-1000,z=-4000,y=-900;
	int ww=2000,hh=3000,dd=3990;

	v3d v1(x,y,z+dd/3),v2(x+ww,y,z+dd/3),v3(x+ww,y+hh,z+dd/3),v4(x,y+hh,z+dd/3);

	traits.diffuse_refrectance(0.1);
	traits.specular_refrectance(0.9);
	world.set_plane( gp( Triangle( v1,v3,v2 ),  // 立ってる方
						traits.color(color_t(0.2,0.2,0.2))
						)
					);
	world.set_plane( gp( Triangle( v1,v4,v3 ), traits) );


	ww+=3000;x-=1500;
	z-=2000;dd+=2000;
	v3d v5(x,y,z),v6(x+ww,y,z),v7(x+ww,y,z+dd),v8(x,y,z+dd);
	traits.diffuse_refrectance(0.7);
	traits.specular_refrectance(0.45);
	world.set_plane( gp( Triangle( v5,v6,v7 ),
						 traits.color(color_t(0.2,0.2,0.8))
					   )
					);
	world.set_plane( gp( Triangle( v5,v7,v8 ),
						 traits
					   )
					);


	return pworld;

}

void STDCALL_ proj_system( const vector3d* plane_pos, const vector3d* u_dir, const vector3d* v_dir, double camera_dist ) {

	proj_.plane_pos( *plane_pos )
		 .u_dir( normalize( *u_dir ))
		 .v_dir( normalize( *v_dir ))
		 .camera_dist( camera_dist );
}
void STDCALL_ proj_area( const syd::Rect* rect ) {
	proj_area_=*rect;
}

void STDCALL_ photo_env( const vector3d* light_dir, double light_intensity, double ambient,
			   const raytrace::color_t* skycol, int rainbow, double rainbow_intensity ) {

	using namespace raytrace;

	ParallelLight paralell_light( *light_dir, light_intensity);

	Angle2RefractionTable* a2r = new Angle2RefractionTable;
	Refraction2LambdaTable* r2l = new Refraction2LambdaTable( std::fstream( "refraction2lambda.txt" ) );
	Lambda2RgbTable* l2rgb = new Lambda2RgbTable( std:: fstream( "lambda2rgb.txt" ) );

	phenv_.light( new ParallelLight(paralell_light) )
		  .ambient( ambient )
		  .skycolor( *skycol )
		  .rainbow( (bool)rainbow )
		  .rainbow_factor( a2r, r2l, l2rgb, rainbow_intensity );

}



void draw( syd::Bitmap& buf, unsigned depth ) {

	using namespace geo;
	using namespace raytrace;

	typedef vector3d v3d;

	// ３Ｄオブジェクト
	World* pworld = new World;//new_random_world();
	
	// 描画！！！
	{
		syd::bmp_lock_key key(buf);
		raytrace::raytrace( proj_, proj_area_, RaytraceCore(*pworld,phenv_), depth, buf);
	}

	delete pworld;
}

void STDCALL_ raytrace_draw(HDC hdc, unsigned w, unsigned h, unsigned depth) {

	using namespace syd;


	srand(time(0));

	if( check() ){ 
		syd::DibSection dib(w,h,4);
		draw( dib, depth );
		BitBlt(hdc, 0,0,w,h, dib.getDC(), 0,0, SRCCOPY);
	} else {
		std::cout << "bad param" << std::endl;
	}

}


