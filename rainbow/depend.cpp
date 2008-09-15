#include "depend.hpp"
#include "utils/winutils/winwindow.h"
//#include "utils/sdlutils/sdlwindow.h"

namespace depend {

syd::Window* new_window(){

	return new syd::WinWindow;
	//return new syd::SdlWindow;
}


} // namespace depend
