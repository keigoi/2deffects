#include "geo.hpp"
#include "utils/bitmap.h"
#include "utils/color.h"
#include <vector>
#include <cmath>
#include "epoly.h"

geo::Triangle
rotate_on_xyz(
		const geo::Triangle& tri, 
		float rad_x,
		float rad_y,
		float rad_z
		) {

	float sinY = sin(rad_y),
          cosYcosZ = cos(rad_y)*cos(rad_z),
          cosYsinZ = cos(rad_y)*sin(rad_z),
          sinXcosY = sin(rad_x)*cos(rad_y),
          cosXcosY = cos(rad_x)*cos(rad_y),

          cosXsinZ = cos(rad_x)*sin(rad_z),
          sinXcosZ = sin(rad_x)*cos(rad_z),
          cosXcosZ = cos(rad_x)*cos(rad_z),
          sinXsinZ = sin(rad_x)*sin(rad_z),

		  sinXsinYcosZ = sinXcosZ * sinY,
          sinXsinYsinZ = sinXsinZ * sinY,
          cosXsinYcosZ = cosXcosZ * sinY,
          cosXsinYsinZ = cosXsinZ * sinY;

	geo::vector3d newPts[3];
	const geo::vector3d *p = tri.three_points();
	for(int i=0; i<3; i++) {
		double tmpX = p[i].x(),
        		tmpY = p[i].y(),
				tmpZ = p[i].z();

		double 
			newX = cosYcosZ * tmpX - cosYsinZ * tmpY + sinY * tmpZ,
			newY = (cosXsinZ + sinXsinYcosZ) * tmpX
        				+ (cosXcosZ - sinXsinYsinZ) * tmpY
						-  sinXcosY * tmpZ,
			newZ = (sinXsinZ - cosXsinYcosZ) * tmpX
        				+ (sinXcosZ + cosXsinYsinZ) * tmpY
						+  cosXcosY * tmpZ;
		newPts[i] = geo::vector3d(newX, newY, newZ);
	}

	return geo::Triangle(newPts[0], newPts[1], newPts[2]);
}

void clear_zbuffer(double* zbuf, unsigned w, unsigned h)
{
	for(int i=0; i<w*h-1; i++) {
		zbuf[i] = 99999999999;
	}
}

void draw_line(
		syd::Bitmap &bmp, 
		long x1, long x2, long y, long z1, long z2,
		syd::color_t color, double* zbuf) {

	unsigned width = bmp.width(), height = bmp.height();

	if (y<0 || bmp.height()<=y )return;

	if(x2<x1) {
		long temp;

		temp=x1;
		x1=x2;
		x2=temp;

		temp=z1;
		z1=z2;
		z2=temp;

	}

    float dz;
    if(x1!=x2){
        dz = ((float)(z2-z1))/(float)(x2-x1);
	} else {
		dz = 1;
	}

	long p = x1+ y * width;
	zbuf += p;
	
	syd::bmp_lock_key key__(bmp);
	syd::color_t *ptr = bmp.getPtr(ptr);
	ptr += p;


	float   z  = z1;
    if(x2>=width)x2=width;

	for(int i=x1; i<=x2; i++){

		if(i>=0){
			if(z<*zbuf){
  				*zbuf=z;
				*ptr=color;
			}
		}
		zbuf++;
		ptr++;
 		z+=dz;
	}

}


void draw_triangle(
		syd::Bitmap& bmp, 
		const geo::Triangle& tri, 
		syd::color_t color, 
		double screen_z, 
		double camera_z,
		double* zbuf) {

	using namespace geo;
	vector3d p0 = tri.three_points()[0],
		p1 = tri.three_points()[1],
		p2 = tri.three_points()[2];

	//ベクトルp1p0とp2p0の外積のz成分
	long vz = (p1.x() - p0.x())
        		*(p2.y() - p0.y())
			- (p1.y() - p0.y())
				*(p2.x() - p0.x());
	if(vz>0) { return; }

	//三角形の頂点の並べ替え Y座標が小さい順にp0,p1,p2とする
	vector3d temp;
	if(p0.y() > p1.y()){
		temp=p0;
		p0=p1;
		p1=temp;
	}
	if(p1.y() > p2.y()){
		temp=p1;
		p1=p2;
		p2=temp;
	}
	if(p0.y() > p1.y()){
		temp=p0;
		p0=p1;
		p1=temp;
	}

	///描画本体…

	float x1, x2, z1, z2;
    float p0p1_xy, p0p2_xy, p1p2_xy,
    	  p0p1_zy, p0p2_zy, p1p2_zy;
	long y0=p0.y(),
		 y1=p1.y(),
		 y2=p2.y();


	if(y0 != y1){
		x1=x2=p0.x();
		z1=z2=p0.z();
		p0p1_xy = (p1.x() - p0.x()) / (float)(y1-y0);
		p0p1_zy = (p1.z() - p0.z()) / (float)(y1-y0);
		p0p2_xy=  (p2.x() - p0.x()) / (float)(y2-y0);
		p0p2_zy=  (p2.z() - p0.z()) / (float)(y2-y0);
	}else{
		x1=p1.x();
		x2=p0.x();
		z1=p1.z();
		z2=p0.z();
		if(y1 == y2){
			draw_line(bmp, x1, x2, y1, z1, z2, color, zbuf);
			x2=p2.x();
			z2=p2.z();
			p0p2_xy=0.0;
			p0p2_zy=0.0;
		}else{
			p0p2_xy= (p2.x() - p0.x()) / (float)(y2-y0);
			p0p2_zy= (p2.z() - p0.z()) / (float)(y2-y0);
		}

	}

	if(y2 != y1){
		p1p2_xy= (p2.x() - p1.x()) / (float)(y2-y1);
		p1p2_zy= (p2.z() - p1.z()) / (float)(y2-y1);
	} else {
		p1p2_xy = 0.0;
		p1p2_zy = 0.0;
	}

	unsigned width = bmp.width(),
		height = bmp.height();

    if(y1>height){y1=y2=height;}
    else if(y2>height){y2=height;}

	for(int _y=y0; _y<y1; _y++){
		draw_line(bmp, x1, x2, _y, z1, z2, color, zbuf);

		x1+=p0p1_xy;
		x2+=p0p2_xy;
		z1+=p0p1_zy;
		z2+=p0p2_zy;
	}

	for(int y=y1; y<=y2; y++){
		draw_line(bmp, x1, x2, y, z1, z2, color, zbuf);

		x1+=p1p2_xy;
		x2+=p0p2_xy;
		z1+=p1p2_zy;
		z2+=p0p2_zy;
	}

}
geo::Triangle calc_projected_triangle(
	const geo::Triangle& tri,
	double w, double h, 
	double screen_z, double camera_z) {

	using namespace geo;
	
	vector3d projected[3];

	const vector3d* p = tri.three_points();
	for(int i=0; i<3; i++) {

		double x,y,z;
		if((-camera_z + p[i].z())==0){
        	x = p[i].x() + w/2;
            y = p[i].y() + h/2;
        	z = p[i].z();
		}else{
        	x = (p[i].x() * (screen_z-camera_z)) / (-camera_z + p[i].z())  + w/2;
        	y = -(p[i].y() * (screen_z-camera_z)) / (-camera_z + p[i].z())  + h/2;
        	z = p[i].z();
		}
		
		projected[i] = vector3d(x,y,z);
    }

	return Triangle(projected[0], projected[1], projected[2]);
}


void draw(
		syd::Bitmap& bmp, 
		std::vector<ColoredTriangle> tris, 
		double screen_z, 
		double camera_z) {

	using namespace geo;
	
	unsigned w=bmp.width(), h=bmp.height();

	double* zbuf = new double[w*h];
	clear_zbuffer(zbuf, w, h);

	for(std::vector<ColoredTriangle>::const_iterator iter = tris.begin();
		iter != tris.end();
		iter++) {

		Triangle projected_tri = calc_projected_triangle(iter->triangle, w, h, screen_z, camera_z);
		draw_triangle(bmp, projected_tri, iter->color, screen_z, camera_z, zbuf);
	}


	delete[] zbuf;
}

/*

void EPoly::RotateOnX(schar rad)
{
    long 	 n = Objs[MovingObject].NumOfPoints-1;
    long	*p = Objs[MovingObject].points;

    for(int i=n; i>=0; i--){

    	float tmpY = Pnts[*p].Y,
              tmpZ = Pnts[*p].Z;

		Pnts[*p].Y = tmpY*cos_table[rad] - tmpZ*sin_table[rad];
        Pnts[*p].Z = tmpY*sin_table[rad] + tmpZ*cos_table[rad];

        p++;
	}
}

geo::vector3d rotate_on_y(geo::vector3d p, schar rad)
{
    float tmpX = p.x(),
          tmpZ = p.z();

	p.x( =  tmpX*cos_table[rad] + tmpZ*sin_table[rad];
    Pnts[*p].Z = -tmpX*sin_table[rad] + tmpZ*cos_table[rad];

    p++;
}

void EPoly::RotateOnZ(schar rad)
{
    long 	 n = Objs[MovingObject].NumOfPoints-1;
    long	*p = Objs[MovingObject].points;

    for(int i=n; i>=0; i--){

    	float tmpX = Pnts[*p].X,
              tmpY = Pnts[*p].Y;

		Pnts[*p].X = tmpX*cos_table[rad] - tmpY*sin_table[rad];
        Pnts[*p].Y = tmpX*sin_table[rad] + tmpY*cos_table[rad];

		p++;
	}
}

void EPoly::RotateOnXYZ(schar rad_x, schar rad_y,schar rad_z)
{
	float sinY = sin_table[rad_y],
          cosYcosZ = cos_table[rad_y]*cos_table[rad_z],
          cosYsinZ = cos_table[rad_y]*sin_table[rad_z],
          sinXcosY = sin_table[rad_x]*cos_table[rad_y],
          cosXcosY = cos_table[rad_x]*cos_table[rad_y],

          cosXsinZ = cos_table[rad_x]*sin_table[rad_z],
          sinXcosZ = sin_table[rad_x]*cos_table[rad_z],
          cosXcosZ = cos_table[rad_x]*cos_table[rad_z],
          sinXsinZ = sin_table[rad_x]*sin_table[rad_z],

		  sinXsinYcosZ = sinXcosZ * sinY,
          sinXsinYsinZ = sinXsinZ * sinY,
          cosXsinYcosZ = cosXcosZ * sinY,
          cosXsinYsinZ = cosXsinZ * sinY;

    long 	 n = Objs[MovingObject].NumOfPoints;
    long	*p = Objs[MovingObject].points;

    for(int i=0; i<n; i++){

    	float tmpX = Pnts[*p].X,
        	  tmpY = Pnts[*p].Y,
              tmpZ = Pnts[*p].Z;

        Pnts[*p].X = cosYcosZ * tmpX - cosYsinZ * tmpY + sinY * tmpZ;
        Pnts[*p].Y = (cosXsinZ + sinXsinYcosZ) * tmpX
        			+ (cosXcosZ - sinXsinYsinZ) * tmpY
                    -  sinXcosY * tmpZ;
        Pnts[*p].Z = (sinXsinZ - cosXsinYcosZ) * tmpX
        			+ (sinXcosZ + cosXsinYsinZ) * tmpY
                    +  cosXcosY * tmpZ;
		p++;
	}


}

*/