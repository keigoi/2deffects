#ifndef _PLAINBMP_H_
#define _PLAINBMP_H_

#include "utils/bitmap.h"

namespace syd {

class PlainBmp : public Bitmap {
	unsigned width_, height_;

public:
	virtual unsigned width() const;
	virtual unsigned height() const;

public:
	virtual Bitmap& lock();
	virtual Bitmap& unlock();
	//virtual color_t* getPtr();

public:
	void attach(void* ptr, unsigned w, unsigned h);


public:
	virtual Bitmap* clone(unsigned w, unsigned h, size_t tagsize) const;

public:
	PlainBmp(unsigned w, unsigned h, size_t tagsize);
	PlainBmp(void* ptr, unsigned w, unsigned h, size_t tagsize);
	virtual ~PlainBmp();

public:
	virtual void invariant() const;

};


} // syd
#endif //_PLAINBMP_H_
