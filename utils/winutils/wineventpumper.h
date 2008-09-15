#ifndef _WINEVENTPUMPER_H_
#define _WINEVENTPUMPER_H_

#include "utils/eventpumper.h"


namespace syd {
class WindowListener;
class MouseListener;
class KeyListener;


class WinEventPumper : public EventPumper {

	bool wmquit_dispatched_;

public:
	WinEventPumper();
	~WinEventPumper();

public:

	virtual void pumpEvents();
	virtual bool toExit() const;

private:
};

} //syd

#endif //!defined(_WINEVENTPUMPER_H_)