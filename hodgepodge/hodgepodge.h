#ifndef _HODGEPODGE_H_
#define _HODGEPODGE_H_


#include <boost/scoped_array.hpp>
using boost::scoped_array;


class Hodgepodge {

	unsigned w_,h_;

	boost::scoped_array<int> cell_;
	boost::scoped_array<int> before_cell_;
	int sick_; //sick‚ÌƒŒƒxƒ‹
	double k1_,k2_;
	int g_;

private:
	inline int infection_count(scoped_array<int>& cell, int rel) const;
	inline int sick_count(scoped_array<int>& cell, int rel) const;
	inline int state_sum(scoped_array<int>& cell, int rel) const;


public:
	Hodgepodge& proceed();

	int* getPtr() {
		return cell_.get();
	}


public:
	Hodgepodge(unsigned w, unsigned h, unsigned sick, double k1, double k2, int g);



};


#endif //!defined(_HODGEPODGE_H_)