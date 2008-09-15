#include "utils/winutils/winwindow.h"
#include "utils/fixedframeratekeeper.h"

namespace depend {

inline syd::Window* new_window(){

	return new syd::WinWindow;
	//return new syd::SdlWindow;
}
} // namespace depend
