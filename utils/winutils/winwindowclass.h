#ifndef _WINWINDOWCLASS_H_
#define _WINWINDOWCLASS_H_

#include <memory>

#include <windows.h>


namespace syd {
class WinWindow;
class WindowListener;
class MouseListener;
class KeyListener;

class WinWindowClass  
{
	ATOM atom_;
	WNDCLASSEX wnd_class_;

	WindowListener *	windowListener_;
	MouseListener *	mouseListener_;
	KeyListener *	keyListener_;

public:
	WinWindowClass (WNDPROC wndproc);
	virtual ~WinWindowClass ();

public:
	operator const ATOM () const;
	operator const WNDCLASSEX& () const;

public:
	WinWindowClass& setListener ( WindowListener* l);
	WinWindowClass& setListener ( MouseListener* l);
	WinWindowClass& setListener ( KeyListener* l);

public:
	virtual LRESULT WndProc(WinWindow& win, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

};

} //syd
#endif // !defined(_WINWINDOWCLASS_H_)
