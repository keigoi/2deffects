// $Id: WinIdle.cpp,v 1.1.1.1 2002/03/20 15:12:19 noname Exp $

#include "utils/winutils/winidle.h"
extern "C" {
#include <windows.h>
}

namespace syd {

bool WinIdle() {
	static bool state_=true;
	MSG msg;

    if(::PeekMessage(&msg,0,0,0,PM_NOREMOVE)) {

		if(::GetMessage(&msg,NULL,0,0) == 0) {
			state_=false;
			return state_;
		}

		//if( WM_KEYFIRST < msg.message && msg.message < WM_KEYLAST ){
		//	return state_;
		//}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

	}

	return state_;
}


} // syd