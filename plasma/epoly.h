#include "geo.hpp"
#include "utils/color.h"
#include <vector>

struct ColoredTriangle {
	syd::color_t color;
	geo::Triangle triangle;
	ColoredTriangle(syd::color_t col, geo::Triangle tri) {
		color = col;
		triangle = tri;
	}
};

void draw(
		syd::Bitmap& bmp, 
		std::vector<ColoredTriangle> tris, 
		double screen_z, 
		double camera_z);

geo::Triangle
rotate_on_xyz(
		const geo::Triangle& tri, 
		float rad_x,
		float rad_y,
		float rad_z
		);