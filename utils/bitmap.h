#ifndef _SYD_BITMAP_H_
#define _SYD_BITMAP_H_

#include <iostream>
#include <exception>
#include <cassert>

namespace syd {

class Bitmap// : public boost::noncopyable 
{
protected:
	void* pointer_;
	const size_t tagsize_;
	bool locked_;

public:
	virtual unsigned width() const =0;
	virtual unsigned height() const =0;

public:
	virtual Bitmap& lock() =0;
	virtual Bitmap& unlock() =0;

public:
	virtual Bitmap* clone(unsigned w, unsigned h, unsigned tagsize) const =0;

public:
	virtual void invariant() const =0;

public:
	Bitmap(size_t tagsize) : tagsize_(tagsize), locked_(false), pointer_(0) {}
	virtual ~Bitmap(){}


public:
	template<class P>
	P* getPtr(P* &ptr) {

		assert ( locked_==true );

		invariant();
		
		if( sizeof(P)!=tagsize_ ) {
			std::cout << "warning : tagsize not adapt" << std::endl;
		}
		
		return ptr = reinterpret_cast<P*>(pointer_);
	}

};


class bitmap_exception : public std::exception {

public:
	const char* what() const throw() {
		return 	"can't lock bitmap";
	}

};

class bmp_lock_key {
	Bitmap& bmp_;

public:

	bmp_lock_key( Bitmap& bmp ) : bmp_(bmp) {
		bmp_.lock();
	}

	~bmp_lock_key() {
		bmp_.unlock();
	}

};

}  //syd

#endif // _SYD_BITMAP_H_

