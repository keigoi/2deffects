#ifndef _EVENTPUMPER_H_
#define _EVENTPUMPER_H_


namespace syd {


class EventPumper {

public:
	void operator()(){
		pumpEvents();
	}
	virtual void pumpEvents() =0;
	virtual bool toExit()const =0;
	virtual ~EventPumper() {}

};



}


#endif //!defined(_EVENTPUMPER_H_)

