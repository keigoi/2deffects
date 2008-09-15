#include "setup.h"


#include "utils/winutils/winwindow.h"

#include <crtdbg.h>
#include <windows.h>
#include <mmsystem.h>



using std::auto_ptr;

namespace setup { 


	auto_ptr<syd::Window> create_window() {

		return auto_ptr<syd::Window>(new syd::WinWindow);

	}

	void sleep() {
		::Sleep(1);
	}

	unsigned long timer() {
		return timeGetTime();
	}






} // namespace factory