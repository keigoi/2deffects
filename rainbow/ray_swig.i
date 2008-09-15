%module raytrace
%{

#include "interface.h"
#include "geo.hpp"
#include "raytrace.hpp"
#include "raytracecore.hpp"
#include "photoenv.hpp"
#include "color.hpp"
#include "projectionsystem.hpp"
#include "planes.hpp"

%}
void  raytrace_draw(HDC hdc, unsigned w, unsigned h, unsigned depth);
