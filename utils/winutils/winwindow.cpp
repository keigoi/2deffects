#include <cassert>
#include <cstring>
#include <iostream>

#include "utils/windowevents.h"

#include "utils/winutils/winwindow.h"

#include "utils/winutils/winwindowclass.h"
#include "utils/winutils/wineventpumper.h"
#include "utils/winutils/dibsection.h"

#include <windows.h>


using namespace std;
namespace syd{


Rect WinWindow::rect () const {

	invariant();

	return rect_;

}


WinWindow::operator HWND () const {
	
	invariant();
	return hwnd_;

}


bool WinWindow::isAlive () const {

	invariant();

	if( :: IsWindow ( hwnd_ ) == FALSE ) {

		return false;

	} else {

		return true;

	}

}


Bitmap& WinWindow::getBuffer () const {

	invariant();

	return *buffer_;

}


Window* WinWindow::clone () const {

	return new WinWindow;

}


Window& WinWindow::refresh () {

	if ( hwnd_ == 0 ) return *this;

/*	RECT winRect;
	Rect rect;

	::GetClientRect ( hwnd_, &winRect );
	rect.setBottomRight ( Point ( winRect.right, winRect.bottom ) );
	
	if( rect.width () != rect_.width () || 
	    rect.height () != rect_.height () ) {

		rect_.setWidth(rect.width ());
		rect_.setHeight(rect.height ());

		createBuffer ();

	}

*/	//::InvalidateRect ( hwnd_, NULL, FALSE );
	HDC dc = ::GetDC(hwnd_);
	refresh(dc);
	::ReleaseDC(hwnd_, dc);

	invariant();
	return *this;

}


Window& WinWindow::createBuffer () {

	assert ( hwnd_ != 0 );

	if ( buffer_.get () != 0 ) {

		if( rect_.width () > buffer_->width () || 
			rect_.height () > buffer_->height () ) {

			delete buffer_.release();

			auto_ptr < DibSection > 
				buf( new DibSection ( rect_.width(), rect_.height(), 4 , false ) );

			buffer_ = buf;
		}

	} else {
		auto_ptr < DibSection > buf( new DibSection ( rect_.width(), rect_.height(), 4, false ) );
		buffer_ = buf;
	}

	return *this;

}



Window& WinWindow::setRect ( const Rect& rect ) {

	invariant();
	assert ( rect.size() > 0 );

	if ( rect_ != rect ) {

		rect_ = rect;

		BOOL res = ::SetWindowPos(*this,
						HWND_TOP,
						rect_ .topLeft() .x(),
						rect_ .topLeft() .y(),
 						rect_ .width() + GetSystemMetrics ( SM_CXFIXEDFRAME ) * 2,
	 					rect_ .height() + GetSystemMetrics ( SM_CYCAPTION ),
						SWP_NOZORDER);
		assert ( res != 0 );

		createBuffer ();
	}
			
	return *this;

}


Window& WinWindow::show ( bool showState ) {
	
	invariant();

	view_state_ = showState;

	if ( view_state_==true ) {

		::ShowWindow ( *this, SW_SHOW );
		::UpdateWindow ( *this );

	} else {
	
		::ShowWindow ( *this, SW_HIDE );

	}

	return *this;

}


Window& WinWindow::create ( const Rect& rect ) {

	assert ( rect.size() > 0 );

	auto_ptr < WinWindowClass > wc ( new WinWindowClass(WndProc_) );
	wndClass_ = wc;
	ATOM wndAtom = ( ATOM ) ( *wndClass_ );
	assert ( wndAtom != 0 );

	rect_ = rect;
	assert ( rect_.size() > 0 );

	hwnd_ = :: CreateWindow (
				( LPCSTR ) wndAtom,
				"",
 				WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX & ( ~WS_MAXIMIZEBOX | WS_THICKFRAME ),
				rect_ .topLeft() .x(),
				rect_ .topLeft() .y(),
 				rect_ .width() + GetSystemMetrics ( SM_CXFIXEDFRAME ) * 2,
 				rect_ .height() + GetSystemMetrics ( SM_CYCAPTION )
							+ GetSystemMetrics ( SM_CYFIXEDFRAME ) * 2,
				0,  //親ウィンドウのハンドル
				0,  //メニューハンドル
				:: GetModuleHandle ( 0 ),  //アプリケーションインスタンスのハンドル
				( LPVOID ) this );

	assert ( hwnd_ != 0 );

	createBuffer();

	if ( view_state_ == true ) {
		show ( true );
	}

	invariant();

	return *this;

}


Window& WinWindow::setTitle ( const char* str ) {

	invariant();
	assert ( str != NULL );

	BOOL res = :: SetWindowText ( hwnd_, str );
	assert ( res != 0 );

	return *this;

}


WinWindow::WinWindow () : rect_ ( Point(0,0),Point(0,0) ),
						 wndClass_ ( NULL ),
						 buffer_ ( NULL ) {

	view_state_ = false;
	hwnd_ = 0;

	eventPumper_ = auto_ptr< WinEventPumper >( new WinEventPumper );

	cout << "WinWindow::WinWindow()" << endl;


}

void WinWindow::destroy() {

	if ( :: IsWindow(hwnd_) != 0) {

		BOOL res = :: DestroyWindow ( *this );
		assert ( res != FALSE );

	}

}


WinWindow::~WinWindow () {

	destroy();

	cout << "WinWindow::~WinWindow()" << endl;

}


Window& WinWindow::drawText ( Point point, const char *str )
{
	invariant();
	
	assert ( point .x()  <  rect_ .width() );
	assert ( point .y()  <  rect_ .height() );
	assert ( str != 0 );

	HDC hDC;
	bool ownDC;
	if ( buffer_.get()  !=  NULL ) {
		ownDC=false;
		hDC = :: CreateCompatibleDC ( NULL );
		HGDIOBJ res = :: SelectObject ( hDC, buffer_ ->getHBitmap() );
		assert ( res != 0 );
	} else {
		ownDC=true;
		hDC = ::GetDC(hwnd_);
	}

	BOOL res = ::TextOut ( hDC,
					point .x(),
					point .y(),
					str,
					strlen ( str )
				);

	assert ( res != FALSE );

	if(ownDC) {
		::ReleaseDC(hwnd_, hDC);
	} else {
		::DeleteDC(hDC);
	}
	return *this;

}


Window& WinWindow::refresh ( HDC hDC )
{
	invariant();
	
//	cout << "WinWindow::refresh(" << pbuffer_->width() << ","
//		 << pbuffer_->height() << ")" << endl;
//	
//	cout << "destDC(win):" << ::GetDC(*this) << "," << endl
//		<< "srcDC(buf) :" << pbuffer_->getDC() << endl;
//
//	cout << "buffer HBITMAP is:" << pbuffer_->getHBitmap() << endl;

	if ( hDC != NULL ) {
	
		if ( buffer_ .get()  !=  NULL && view_state_ == true) {

			HDC srcDC = ::CreateCompatibleDC(0);
			HGDIOBJ res = ::SelectObject(srcDC, buffer_->getHBitmap());
			//assert(res!=0);

			BOOL bltres = 
			::BitBlt ( hDC, //転送先DC
				0, 0,	//転送先 左上隅 座標
				buffer_->width(),
				buffer_->height(), //転送先 幅＆高さ
				srcDC,  //転送元DC
				0, 0, //転送元 左上隅 座標
				SRCCOPY  //ラスタオペレーションコード
				); 

			//assert ( bltres != FALSE );
			::DeleteDC(srcDC);

		}

	}

	return *this;

}


Window& WinWindow::setListener ( WindowListener* l) {
	
	wndClass_->setListener(l);
	return *this;

}


Window& WinWindow::setListener ( MouseListener* l) {

	wndClass_->setListener(l);
	return *this;

}


Window& WinWindow::setListener ( KeyListener* l) {

	wndClass_->setListener(l);
	return *this;

}


EventPumper& WinWindow::getEventPumper() const {

	return *eventPumper_;

}



LRESULT CALLBACK WinWindow::WndProc_(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	if(message == WM_NCCREATE) {
		CREATESTRUCT *cs=(CREATESTRUCT*)lParam;
		::SetWindowLong(hWnd, GWL_USERDATA, (long)cs->lpCreateParams);
	} else if( message == WM_CREATE ) {
		CREATESTRUCT *cs=(CREATESTRUCT*)lParam;
		::SetWindowLong(hWnd, GWL_USERDATA, (long)cs->lpCreateParams);
	}
	
	WinWindow* win=
		reinterpret_cast<WinWindow*>(::GetWindowLong(hWnd, GWL_USERDATA));

	switch( message ) 
	{
		case WM_PAINT:
			hdc = ::BeginPaint (*win, &ps);

			RECT rt;
			::GetClientRect( *win, &rt );
			win->refresh(hdc);
			::EndPaint( hWnd, &ps );
			return 0;

		default:
			if( win ){
				return win->wndClass_->WndProc(*win, hWnd, message,wParam, lParam);
			} else {
				return ::DefWindowProc(hWnd, message, wParam, lParam);
			}

	}
}



} //syd