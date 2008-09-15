#include <cassert>
#include <iostream>

#include "utils/winutils/WinWindowClass.h"
#include "utils/windowevents.h"



using namespace std;
namespace syd {

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WinWindowClass::WinWindowClass(WNDPROC wndproc) : windowListener_(0), mouseListener_(0), keyListener_(0)
{
	cout << "WinWindowClass::WinWindowClass()"<<endl;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)wndproc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ::GetModuleHandle(0);
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "WNDCLASS_SYD";
	wcex.hIconSm		= 0;

	atom_ = ::RegisterClassEx( &wcex );
	wnd_class_ = wcex;


	assert(atom_ != 0);
}

WinWindowClass::~WinWindowClass()
{
	BOOL res = UnregisterClass((LPCSTR)atom_,
							::GetModuleHandle(0));

	if(res == 0) {
		//ウィンドウが残っていると、UnregisterClassは失敗することがある。その時：
		cout <<"---UnregisterClass Failed!!"<<endl;
	}

	cout << "WinWindowClass::~WinWindowClass" <<endl;
}


WinWindowClass::operator const ATOM() const {
	return atom_;
}

WinWindowClass::operator const WNDCLASSEX&() const {
	return wnd_class_;
}


WinWindowClass& WinWindowClass::setListener(WindowListener* l) {

	windowListener_ = l;
	return *this;
}

WinWindowClass& WinWindowClass::setListener(MouseListener* l) {

	mouseListener_ = l;
	return *this;
}

WinWindowClass& WinWindowClass::setListener(KeyListener* l) {

	keyListener_ = l;
	return *this;
}


LRESULT WinWindowClass::WndProc(WinWindow& win, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	if(message == WM_DESTROY) {
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hwnd, message, wparam, lparam);
}



}//namespace syd