#ifndef _SYD_WINWINDOW_H_
#define _SYD_WINWINDOW_H_

#include "utils/window.h"
#include <memory>
#include <cassert>

#include <windows.h>



namespace syd
{
class DibSection;
class WinEventPumper;
class WinWindowClass;

class WinWindow : public Window
{

	Rect rect_;
	bool view_state_; // true=visible / false=invisible

	std :: auto_ptr< DibSection >		buffer_;
	std :: auto_ptr< WinWindowClass >	wndClass_;
	std :: auto_ptr< WinEventPumper >   eventPumper_;

	HWND hwnd_;

	static LRESULT CALLBACK WndProc_ ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	virtual Window& createBuffer ();

public:
	virtual Rect	rect () const;
	virtual Bitmap& getBuffer () const;
	virtual bool	isAlive () const;
	virtual EventPumper& getEventPumper() const;

public:
	virtual Window* clone () const;

public:
	virtual Window& setListener ( WindowListener* l);
	virtual Window& setListener ( MouseListener* l);
	virtual Window& setListener ( KeyListener* l);
	virtual Window& setRect ( const Rect& rect );
	virtual Window& show ( bool showState );
	virtual Window& drawText ( Point point, const char* );
	virtual Window& setTitle ( const char* str );
	virtual Window& refresh ();

public:
	virtual Window& create ( const Rect& rect );

public:
	virtual void destroy();

public:
	Window& refresh ( HDC hDC );

public:
	operator HWND () const;

public:
	WinWindow ();
	virtual ~WinWindow ();

public:
	virtual void invariant() const;
};


inline void WinWindow::invariant() const {

	assert ( hwnd_ != 0 );

	assert ( rect_ .width()  > 0 );
	assert ( rect_ .height() > 0 );

	assert ( buffer_    .get() != 0 );
	assert ( wndClass_ .get() != 0 );

}


} // namespace syd

#endif // (_SYD_WINWINDOW_H_)
