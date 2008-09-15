#include "plasma_main.hpp"
#include <memory>
#include "utils/window.h"
#include "utils/winutils/winwindow.h"
#include "utils/eventpumper.h"
#include "utils/fixedframeratekeeper.h"
#include "utils/plainbmp.h"
#include <windows.h>
#include <cmath>

#include "geo.hpp"
#include "epoly.h"

std::vector<ColoredTriangle> 
rotate_on_xyz(
		std::vector<ColoredTriangle> tris, 
		float rad_x,
		float rad_y,
		float rad_z
		) {

	std::vector<ColoredTriangle> ret;
	for(std::vector<ColoredTriangle>::const_iterator iter = tris.begin();
		iter != tris.end();
		iter++) {

		geo::Triangle newTri = rotate_on_xyz(iter->triangle, rad_x, rad_y, rad_z);

		ret.push_back(ColoredTriangle(iter->color, newTri));
	}

	return ret;
}


uint8_t
calc_table2(int x, int y, unsigned w, unsigned h) {
	unsigned w2=w/2, h2=h/2;
	double temp=sqrt(16.0 + (h2-y)*(h2-y) + (w2-x)*(w2-x)) - 4.0;
	return (uint8_t)(( sin(temp/14/(w/320.0)) +1.0) * 90.0);
}

uint8_t
calc_table3(int x, int y, unsigned w, unsigned h) {
	unsigned w2=w/2, h2=h/2;
	double temp=sqrt(16.0 + (h2-y)*(h2-y) + (w2-x)*(w2-x)) - 4.0;
	return (uint8_t)(( sin(temp/19) +1.0) * 90.0);
}

typedef unsigned long (*TIMERFUNC)();
typedef void (*WAITFUNC)();

void 
demo_main(syd::Window& win, TIMERFUNC timer, WAITFUNC wait ) {

	enum{ WIDTH = 400, HEIGHT = 300 };
	int cw = WIDTH/5;

	using namespace syd;
	using namespace geo;

	// plasma用テーブル - auto_ptr<Bitmap> にしたら落ちた
	std::auto_ptr<PlainBmp> ptable( prepare_table(WIDTH*2, HEIGHT*2, calc_table2) );
	Bitmap& table = *ptable;

	

	// ウィンドウ生成
	win.create( Rect(0,0,WIDTH,HEIGHT) );
	win.setTitle("Effect \"Plasma\"");
	EventPumper& evp = win.getEventPumper();

	win.show(true);

	// 固定フレームレートを実現するモジュール
	syd::FixedFramerateKeeper fkeeper(30);

	color_t red(255,0,0,0);
	color_t green(0,255,0,0);
	color_t blue(0,0,255,0);
	color_t orange(255,200,0,0);
	color_t cyan(0,255,255,0);
	color_t white(255,255,255,0);
	color_t purple(255,0,255,0);
	color_t yellow(255,255,0,0);
	color_t lyellow(255,255,128,0);
	color_t gray(180,240,100,0);
	geo::vector3d p1(-cw, cw, cw), p2(-cw, cw, -cw), p3(cw, cw, cw), p4(cw, cw, -cw),
		p5(-cw, -cw, cw), p6(-cw, -cw, -cw), p7(cw, -cw, cw), p8(cw, -cw, -cw);
	std::vector<ColoredTriangle> cube;
	cube.push_back(ColoredTriangle(yellow, Triangle(p1,p2,p3)));
	cube.push_back(ColoredTriangle(yellow, Triangle(p2,p4,p3)));
	cube.push_back(ColoredTriangle(orange, Triangle(p2,p6,p4)));
	cube.push_back(ColoredTriangle(orange, Triangle(p4,p6,p8)));
	cube.push_back(ColoredTriangle(blue, Triangle(p4,p8,p7)));
	cube.push_back(ColoredTriangle(blue, Triangle(p4,p7,p3)));
	cube.push_back(ColoredTriangle(green, Triangle(p5,p7,p6)));
	cube.push_back(ColoredTriangle(green, Triangle(p6,p7,p8)));
	cube.push_back(ColoredTriangle(white, Triangle(p2,p1,p6)));
	cube.push_back(ColoredTriangle(white, Triangle(p1,p5,p6)));
	cube.push_back(ColoredTriangle(red, Triangle(p1,p3,p5)));
	cube.push_back(ColoredTriangle(red, Triangle(p3,p7,p5)));

	geo::vector3d p9(0, cw*2, 0), p10(-cw, -cw*1.4, -cw), p11(cw, -cw*1.4, -cw), p12(0, -cw*1.4, cw);
	std::vector<ColoredTriangle> cone;
	cone.push_back(ColoredTriangle(cyan, Triangle(p9,p10,p11)));
	cone.push_back(ColoredTriangle(purple, Triangle(p9,p11,p12)));
	cone.push_back(ColoredTriangle(gray,   Triangle(p9,p12,p10)));
	cone.push_back(ColoredTriangle(lyellow,   Triangle(p10,p12,p11)));


	float rad_x=0.0, rad_y=0.0, rad_z=0.0;
	unsigned t = 0;
	while(!evp.toExit()) {
		std::vector<ColoredTriangle> newTris(rotate_on_xyz(cube, rad_x, rad_y, rad_z));
		std::vector<ColoredTriangle> rotatedCone(rotate_on_xyz(cone, rad_y, rad_z, rad_x));
		newTris.insert(newTris.end(), rotatedCone.begin(), rotatedCone.end());
		win.refresh();
		plasma_draw( win.getBuffer(), table, t);
		// draw a cube and a cone
		draw(win.getBuffer(), newTris, -WIDTH, -WIDTH*2);
		syd::bmp_lock_key key__(win.getBuffer());
		uint8_t* topLeft = win.getBuffer().getPtr(topLeft);
		_BLUR32_(topLeft, topLeft, WIDTH, HEIGHT);

		evp.pumpEvents();
		t++;
		fkeeper.wait(timer, wait);
		rad_x = sin(t*0.01) * 5;
		rad_y = cos(t*0.01) * 3;
		rad_z = sin(t*0.01+0.5) * 8;
	}
}


// ----- platform dependent part -----
void sleep() {
	::Sleep(1);
}

unsigned long timer() {
	return timeGetTime();
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	std::auto_ptr<syd::Window> pwin(new syd::WinWindow);
	demo_main(*pwin, timer, sleep);
	return 0;
}



/*
void red_gradation_test(syd::Bitmap& bmp) {
	unsigned w = bmp.width(), h = bmp.height();

	syd::bmp_lock_key key__(bmp);
	syd::color_t* pix = bmp.getPtr(pix);

	for(unsigned y=0; y<h; y++) {
		for(unsigned x=0; x<w; x++) {
			pix[x+y*w].r = y;
		}
	}
}
uint8_t
calc_table1(int x, int y, unsigned w, unsigned h) {
	return
	(uint8_t)(( sqrt(16.0 + (h-y)*(h-y) + (w-x)*(w-x))-4) * 2.5 * (w/320.0));
}

*/

