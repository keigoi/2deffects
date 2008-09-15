#ifndef _FIXEDFRAMERATEKEEPER_H_
#define _FIXEDFRAMERATEKEEPER_H_

namespace syd {

typedef unsigned long (__stdcall * WINTIMERFUNCPLAIN)(void);

typedef unsigned long (__cdecl * TIMERFUNCPLAIN)(void);

inline void nothing(){} // inlineはごまかし

class FixedFramerateKeeper {

	unsigned long realFramerate_; //実際のフレームレート(前の一秒のフレームレートカウントの合計)
	unsigned long fixedFramerate_; // 目指すフレームレート
	unsigned long framerateCount_; // 測定中のフレームレートのカウント
	double prevTime_; //前回の呼び出しの時刻
	unsigned long framerateCountTimeBase_; // 現在カウント中のフレームレートの、カウント開始時刻

public:
	template < class _TIMERFUNC >
	void wait() {
		wait(nothing);
	}


	template < class _TIMERFUNC, class _WAITFUNC > 
	void wait( _TIMERFUNC _t, _WAITFUNC& _w) {

		//フレームレート固定処理
		double stopTime = prevTime_ + ( ( 1.0 / fixedFramerate_ ) *1000.0);

		//追いつき処理
		double now = _t();
		if( stopTime + 100 < now ) {
			stopTime=now;
		}
		
		while( _t() < stopTime ) {
			_w();
		}

		//Framerate
		framerateCount_++;

		if( framerateCountTimeBase_ + 1000 < stopTime ) { //一秒経っていたら

			framerateCountTimeBase_ = stopTime; // 新たなフレームレート測定の開始時間
			realFramerate_ = framerateCount_; //フレームレートの更新
			framerateCount_ = 0; // フレームレートカウンタのリセット

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