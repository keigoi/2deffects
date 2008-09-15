#include <memory>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <ctime>
#include <cmath>

#include "depend.hpp"
#include "utils/bitmap.h"
#include "utils/window.h"
#include "utils/eventpumper.h"
#include "utils/color.h"

#include "raytrace.hpp"


#define for if(false);else for




inline double rand500() {
	using namespace std;
	return 500.0-1000.0*rand()/RAND_MAX;
}

inline double rand1() {
	using namespace std;
	return (double)rand()/RAND_MAX*0.6+0.4;
}

void draw( syd::Bitmap& buf ) {

	using namespace geo;
	using namespace raytrace;

	typedef vector3d v3d;
	typedef GeneralPlane gp;

	//投影系
	ProjectionSystem proj;
	proj.plane_pos( v3d( 0.0, 0.0, 100.0 ) )
		.u_dir( scalar(normalize(v3d( 15, 0.0, 0.0 )),2.5))
		.v_dir( scalar(normalize(v3d( 0.0, 15, 0.0 )),2.5))
		.camera_dist( 800.0 );


	// 虹用の定数値
	Angle2RefractionTable* a2r = new Angle2RefractionTable;
	std::ifstream r2lfile( "refraction2lambda.txt" );
	Refraction2LambdaTable* r2l = new Refraction2LambdaTable( r2lfile );
	std::ifstream l2rgbfile( "lambda2rgb.txt" );
	Lambda2RgbTable* l2rgb = new Lambda2RgbTable( l2rgbfile );


	//光源系
	//PointLight point_light(v3d(100,100,0),10000000.0);
	ParallelLight paralell_light(v3d(0.2,-1,-2), 0.8);


	PhotoEnvironment phenv;
	phenv.light( new ParallelLight(paralell_light) )
		 .ambient(1.0)
		 .skycolor( color_t(0.3,0.3,0.8) )
		 //.skycolor( color_t(0,0,0) )
		 .rainbow(true)
		 .rainbow_factor(a2r,r2l,l2rgb,0.2);


	//３Ｄワールド系
	World world;

	//使いまわし表面特性
	PlaneTraits traits( 0.3, 0.9, 0.55, color_t(0,0,0) );


	//三角形の配置（見えないかも！
	for(int i=0; i<2; i++) {
		v3d p0 = v3d(rand500()*2.4,rand500()*1.2,-rand500()/2-1500);
		Triangle tri(p0,
					add(p0,v3d(rand500()*1.6,rand500()*2,rand500()*0.7)),
					add(p0,v3d(rand500()*1.6,rand500()*2,rand500()*0.7)) );
		v3d normal = tri.normal();
		vector3d eyeline = normalize( sub(proj.camera_pos(),tri.barycenter()) );
		//if( dot( eyeline, normal ) <0)tri.turnover();
		world.set_plane(
				gp( tri,
					traits.color( color_t(rand1(),rand1(),rand1()) )
				   )
			  );
	}


	//球の配置
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


	// デカい鏡の配置
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

	
	// 描画！！！
	{
		syd::bmp_lock_key key(buf);
		raytrace::raytrace( proj, syd::Rect(-320, 240, 320, -240 ), RaytraceCore(world,phenv), 2, buf);
	}

}

int main(int argc, char** argv) {
//int SDL_main(int argc, char** argv) {

	using namespace syd;

	srand(time(0));

	std::auto_ptr<Window> pwin( depend::new_window() );
	Window& win = *pwin;

	
	win.create( Rect(100,100,420,340) );
	EventPumper& evp = win.getEventPumper();

	draw( win.getBuffer() );

	win.show(true);

	while(!evp.toExit()) {
		evp.pumpEvents();
	}


	return 0;
}

#if 0
#ifndef _MSC_VER

#include <windows.h>

extern "C"
int WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	return SDL_main(0,0);
}
#endif
#endif
