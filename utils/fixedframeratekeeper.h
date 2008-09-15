#ifndef _FIXEDFRAMERATEKEEPER_H_
#define _FIXEDFRAMERATEKEEPER_H_

namespace syd {

typedef unsigned long (__stdcall * WINTIMERFUNCPLAIN)(void);

typedef unsigned long (__cdecl * TIMERFUNCPLAIN)(void);

inline void nothing(){} // inline�͂��܂���

class FixedFramerateKeeper {

	unsigned long realFramerate_; //���ۂ̃t���[�����[�g(�O�̈�b�̃t���[�����[�g�J�E���g�̍��v)
	unsigned long fixedFramerate_; // �ڎw���t���[�����[�g
	unsigned long framerateCount_; // ���蒆�̃t���[�����[�g�̃J�E���g
	double prevTime_; //�O��̌Ăяo���̎���
	unsigned long framerateCountTimeBase_; // ���݃J�E���g���̃t���[�����[�g�́A�J�E���g�J�n����

public:
	template < class _TIMERFUNC >
	void wait() {
		wait(nothing);
	}


	template < class _TIMERFUNC, class _WAITFUNC > 
	void wait( _TIMERFUNC _t, _WAITFUNC& _w) {

		//�t���[�����[�g�Œ菈��
		double stopTime = prevTime_ + ( ( 1.0 / fixedFramerate_ ) *1000.0);

		//�ǂ�������
		double now = _t();
		if( stopTime + 100 < now ) {
			stopTime=now;
		}
		
		while( _t() < stopTime ) {
			_w();
		}

		//Framerate
		framerateCount_++;

		if( framerateCountTimeBase_ + 1000 < stopTime ) { //��b�o���Ă�����

			framerateCountTimeBase_ = stopTime; // �V���ȃt���[�����[�g����̊J�n����
			realFramerate_ = framerateCount_; //�t���[�����[�g�̍X�V
			framerateCount_ = 0; // �t���[�����[�g�J�E���^�̃��Z�b�g

		}

		prevTime_ = stopTime;

	}

	unsigned long getRealFramerate() const {
		return realFramerate_;
	}

	unsigned long getFixedFramerate() const {
		return fixedFramerate_;
	}

	void setFixedFramerate(unsigned long r){
		fixedFramerate_ = r;
	}

public:
	FixedFramerateKeeper(unsigned frameRate )
		: realFramerate_( 0 ),
		  fixedFramerate_( frameRate ),
		  framerateCount_( 0 ),
		  prevTime_( 0 ),
		  framerateCountTimeBase_( prevTime_ ) {}

	~FixedFramerateKeeper() {};

};


} //syd


#endif //!defined(_FIXEDFRAMERATEKEEPER_H_)