#ifndef _RAYTRACE__HPP_
#define _RAYTRACE__HPP_

#include "geo.hpp"
#include "raytracecore.hpp"
#include "projectionsystem.hpp"

namespace syd {

	class syd::Bitmap;
	class syd::Rect;

} // namespace syd


namespace raytrace {


syd::Bitmap& raytrace( const raytrace::ProjectionSystem& proj,
					   const syd::Rect& rect,
					   const raytrace::RaytraceCore& core,
					   unsigned depth,
					   syd::Bitmap& bmp );

} // namespace raytrace

#endif //!defined(_RAYTRACE__HPP_)
