#ifndef _SDLWINDOW_H_
#define _SDLWINDOW_H_
//
#include "utils/window.h"
//
#include <memory>

struct SDL_Surface;

namespace syd {

class SdlBitmap;
class EventPumper;
class SdlEventPumper;

class SdlWindow : public Window {

	Rect rect_;
	std::auto_ptr<SdlBitmap> buffer_p;

	static unsigned long videoModeOption_;
	static bool canSetVideoMode_;
	static SdlWindow* singleton_;  // create()‚ÅƒZƒbƒg‚³‚ê‚é

	std::auto_ptr<SdlEventPumper> eventPumper_;

public:
	virtual Rect rect() const;
	virtual Bitmap& getBuffer() const;
	virtual bool isAlive() const;
	virtual EventPumper& getEventPumper() const;

public:
	virtual Window* clone() const;

public:
	virtual Window& setListener ( WindowListener* l);
	virtual Window& setListener ( MouseListener* l);
	virtual Window& setListener ( KeyListener* l);
	virtual Window& setRect(const Rect& rect);
	virtual Window& show(bool showState);
	virtual Window& drawText(Point point, const char* str);
	virtual Window& setTitle(const char* str);
	virtual Window& refresh();

public:
	virtual Window& create(const Rect& rect);

public:
	virtual void destroy();


public:
	SdlWindow();
	virtual ~SdlWindow();

public:
	static void usingFullScreen( bool flag );
	static void usingDoubleBuffer( bool flag );
	static void usingHardwareSurface( bool flag );

public:
	virtual void invariant() const; 	// not implemented!!!

};


} //syd

#endif //_SDLWINDOW_H_
