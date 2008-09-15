#ifndef _SDLEVENTPUMPER_H_
#define _SDLEVENTPUMPER_H_

#include "utils/eventpumper.h"

namespace syd {

class Window;
class WindowListener;
class MouseListener;
class KeyListener;
	
	
class SdlEventPumper : public EventPumper {

	Window& window_;
	bool quitRequested_;
	static SdlEventPumper* singleton_;

	WindowListener *	windowListener_;
	MouseListener *		mouseListener_;
	KeyListener *		keyListener_;


public:
	SdlEventPumper( Window& window );
	~SdlEventPumper();

public:
	virtual void pumpEvents();
	virtual bool toExit() const;


public:
	virtual SdlEventPumper& setListener ( WindowListener* l);
	virtual SdlEventPumper& setListener ( MouseListener* l);
	virtual SdlEventPumper& setListener ( KeyListener* l);

};


} // syd


#endif //!defined(_SDLEVENTPUMPER_H_)
