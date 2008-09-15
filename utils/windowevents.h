#ifndef _SYD_EVENTS_H_
#define _SYD_EVENTS_H_

#include "utils/geometry.h"
#include <memory>

namespace syd {

class Window;

class WindowListener {

public:
	virtual void windowResized ( Window& src ) =0;
	virtual void windowClosing  ( Window& src ) =0;
	virtual void windowDeiconified ( Window& src ) =0;
	virtual void windowIconified ( Window& src ) =0;

	virtual ~WindowListener() {}

};

class WindowAdapter : public WindowListener {

public:
	void windowResized ( Window& src ) {}
	void windowClosing  ( Window& src ) {}
	void windowDeiconified ( Window& src ) {}
	void windowIconified ( Window& src ) {}

	~WindowAdapter(){};

};


class MouseEvent {

	Window& src_;
	Point point_;
	int button_;
	
public:
	enum { LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON };

public:
	MouseEvent( Window& src, Point point, int button )
		 : src_( src ), point_( point ), button_( button ) {}
	~MouseEvent() {}

public:
	Window& getSource() const { return src_; }
	Point getPoint() const { return point_; }
	int getButton() const { return button_; }

};


class MouseListener {

public:

	//SDL_MouseButtonEvent
//	virtual void mouseClicked ( std::auto_ptr< MouseEvent > event ) =0;
	virtual void mousePressed ( std::auto_ptr< MouseEvent > event ) =0;
	virtual void mouseReleased ( std::auto_ptr< MouseEvent > event ) =0;

	//SDL_MouseMotionEvent
//	virtual void mouseDragged ( std::auto_ptr< MouseEvent > event ) =0;
	virtual void mouseMoved ( std::auto_ptr< MouseEvent > event ) =0;

	virtual ~MouseListener() {}
};

class MouseAdapter : public MouseListener {

public:

//	void mouseClicked ( std::auto_ptr< MouseEvent > event ) {}
	void mousePressed ( std::auto_ptr< MouseEvent > event ) {}
	void mouseReleased ( std::auto_ptr< MouseEvent > event ) {}
//	void mouseDragged ( std::auto_ptr< MouseEvent > event ) {}
	void mouseMoved ( std::auto_ptr< MouseEvent > event ) {}

	~MouseAdapter(){};
};


class KeyEvent {

	Window& src_;
	int keycode_;
	
public:
	KeyEvent( Window& src, int keycode )
		: src_( src ), keycode_( keycode ) {}
	~KeyEvent() {}

public:
	Window& getSource() const { return src_; }
	int getKeycode() const { return keycode_; }

};


class KeyListener {

public:
	//SDL_KEYBOARDEVENT‚Å
//	virtual void keyPressed ( std::auto_ptr< KeyEvent > event ) =0;
	virtual void keyReleased ( std::auto_ptr< KeyEvent > event ) =0;
	virtual void keyTyped ( std::auto_ptr< KeyEvent > event ) =0;
	
	virtual ~KeyListener() {}
};



class KeyAdapter : public KeyListener {

public:
//	void keyPressed ( std::auto_ptr< KeyEvent > event ) {}
	void keyReleased ( std::auto_ptr< KeyEvent > event ) {}
	void keyTyped ( std::auto_ptr< KeyEvent > event ) {}
	
	~KeyAdapter() {}
};


} //namespace syd

#endif //_SYD_EVENTS_H_
