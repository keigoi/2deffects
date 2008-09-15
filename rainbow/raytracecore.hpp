#ifndef __SYD__RAYTRACE__HPP__
#define __SYD__RAYTRACE__HPP__

#include <vector>
#include <boost/scoped_ptr.hpp>

#include "utils/bitmap.h"
#include "utils/geometry.h"  // for syd::Rect

#include "geo.hpp"
#include "planes.hpp"
#include "photoenv.hpp"


namespace raytrace {


class RaytraceCore {
	const World& world_;
	const PhotoEnvironment& phenv_;

public:
	color_t raytrace( const geo::HalfLine &eyeline, unsigned depth ) const;

public:
	RaytraceCore( const World& w, const PhotoEnvironment& ph )
		: world_(w), phenv_(ph) {
	}
};


} // namespace raytrace


#endif //!defined(__SYD__RAYTRACE__HPP__)
