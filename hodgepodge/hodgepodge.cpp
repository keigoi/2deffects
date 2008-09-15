#include "hodgepodge.h"
#include <stdlib.h>

#define for if(0);else for

inline int Hodgepodge::infection_count(scoped_array<int>& cell, int rel) const
{
	int ret=
		(cell[rel]!=0)
		+(cell[rel-w_]!=0)
		+(cell[rel+w_]!=0)
		+(cell[rel-1]!=0)
		+(cell[rel+1]!=0);
	return ret;
}


inline int Hodgepodge::sick_count(scoped_array<int>& cell, int rel) const
{
	return 
		(cell[rel-w_]==sick_)
		+(cell[rel+w_]==sick_)
		+(cell[rel-1]==sick_)
		+(cell[rel+1]==sick_);
}


inline int Hodgepodge::state_sum(scoped_array<int>& cell, int rel) const
{
	return 
		cell[rel]
		+ cell[rel-1] + cell[rel+1]
		+ cell[rel-w_] + cell[rel+w_];
}


Hodgepodge::Hodgepodge(unsigned w, unsigned h, unsigned sick, double k1, double k2, int g)
		: cell_(new int[w*h]), 
		before_cell_(new int[w*h])
{

	w_=w;
	h_=h;
	sick_=sick;
	k1_=k1;
	k2_=k2;
	g_=g;

	for(int i = 0; i < w_; i++) {
		cell_[i] = cell_[w_*(h_-1)+i] =0;

	}		

	for(int i = w_; i < w_ * (h_-1); i++) {
		// かなりの低確率(1/(RAND_MAX+1))のsick_+1もsick_と同じにみなされる。
		cell_[i] = (float)rand()/RAND_MAX*(sick_+1);
	}

	for(int i = 0; i < h_; i++) {
		cell_[i*w_] = cell_[(i*w_)+(w_-1)] = 0;
	}

	for(int i = 0; i < w_ * h_; i++) {
		before_cell_[i] = cell_[i];
	}
}


Hodgepodge& Hodgepodge::proceed()
{

	cell_.swap(before_cell_);

	int rel=w_+1; // (1,1)から開始
	
	//for( int y = 1; y < h_-1; y++) {
	for(int gy=h_-2; gy>0; gy--) { //ダウンカウンタ

		//for( int x = 1; x < w_-1; x++) {
		for(int gx=w_-2; gx>0; gx--) {
	
			int next_state=-1;
			{
				int state = before_cell_[rel];
				if(state ==0) {

					next_state = 
						int(infection_count(before_cell_ ,rel)/k1_)
						+ int(sick_count(before_cell_, rel)/k2_);

				} else if( state < sick_ ) {

					// infection_count() >= 1 よりOK(devide by zeroは起こらない)
					next_state
						= state_sum(before_cell_,rel) / infection_count(before_cell_, rel)
							+ g_;

				} else {
					next_state=0;
				}
			}

			if(next_state > sick_){
				next_state = sick_;
			}
			cell_[rel]=next_state;
			rel++;
		} // for x

		rel += 2; //右端と左端は飛ばす

	} // for y

	return *this;

}
