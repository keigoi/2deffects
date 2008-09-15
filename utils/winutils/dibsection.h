// $Id: DibSection.h,v 1.4 2002/03/21 13:33:51 noname Exp $

#ifndef _SYD_DIBSECTION_H_
#define _SYD_DIBSECTION_H_

#include "utils/bitmap.h"

#include <windows.h>

namespace syd {

class DibSection : public Bitmap
{
	int			width_, height_;
	bool		turnover_;
	HBITMAP		hbitmap_;

private:
	DibSection ( const DibSection& );
	const DibSection& operator = ( const DibSection& );

// accessor (override)
public:
	virtual unsigned width  () const;
	virtual unsigned height () const;

// manipulator (override)
public:
	virtual Bitmap&		lock ();
	virtual Bitmap&		unlock ();
//	virtual color_t*	getPtr ();

// creator (override)
public:
	virtual Bitmap*		clone ( unsigned w, unsigned h, unsigned tagsize ) const;

// constructor / destructor
public:
	DibSection ( unsigned width, unsigned height, unsigned tagsize, bool turnover );
	virtual ~DibSection ();

// accessor
public:
	HBITMAP	getHBitmap () const;

// invariant
public:
	virtual void invariant() const;

};

} //syd
#endif // _SYD_DIBSECTION_H_