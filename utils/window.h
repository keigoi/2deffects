#ifndef _SYD_WINDOW_H_
#define _SYD_WINDOW_H_

#include "utils/geometry.h"

namespace syd
{
class Bitmap;
class EventPumper;
class WindowListener;
class MouseListener;
class KeyListener;

class Window {

public:
// accessor
	virtual Rect rect() const =0;
	virtual Bitmap& getBuffer() const =0;
	virtual bool isAlive() const=0;
	virtual EventPumper& getEventPumper() const =0;

public:
// manipulator
	virtual Window& drawText ( Point point, const char* str ) =0;
	virtual Window& refresh ()=0;
	virtual Window& setRect ( const Rect& rect ) =0;
	virtual Window& setTitle ( const char* str ) =0;
	virtual Window& show ( bool showState ) =0;
// listener
	virtual Window& setListener ( WindowListener* l) =0; 
	virtual Window& setListener ( MouseListener* l) =0;
	virtual Window& setListener ( KeyListener* l) =0;

public:
// for providing prototype
	virtual Window* clone() const =0;
// creator
	virtual Window& create( const Rect& rect ) =0;
// destroyer 
	virtual void destroy() =0;


public:
// destructor
	virtual ~Window() {};

public:
// invariant
	virtual void invariant() const =0;
};

} //namespace syd

#endif // (_SYD_WINDOW_H_)
