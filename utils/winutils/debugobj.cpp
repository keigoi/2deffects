#include <crtdbg.h>

#ifdef _DEBUG

namespace {
struct DebugObj__ {
	DebugObj__() {
		int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(tmpDbgFlag);
		//TRACE0("DebugFlag Set.\n");
	}
};

} // noname namespace


static DebugObj__ obj;


#endif // _DEBUG