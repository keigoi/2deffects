#include "wineventpumper.h"
#include <windows.h>



namespace syd {


WinEventPumper::WinEventPumper() : wmquit_dispatched_(false) {
}


WinEventPumper::~WinEventPumper() {
}


void WinEventPumper::pumpEvents() {
	MSG msg;

	if(!wmquit_dispatched_) {

	    if(::PeekMessage(&msg,0,0,0,PM_NOREMOVE)) {

			if(::GetMessage(&msg,NULL,0,0) == 0) {
				wmquit_dispatched_=true;
				return;
			}

			//if( WM_KEYFIRST < msg.message && msg.message < WM_KEYLAST ){
			//	return state_;
			//}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

		}
	}

	return;
}


bool WinEventPumper::toExit() const {

	return wmquit_dispatched_;

}



} //syd
