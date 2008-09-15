#include <windows.h>
#include <stdio.h>
#include <iostream>
#ifndef __BORLANDC__
using namespace std;
#endif

#include "utils/winutils/winconsole.h"

namespace {
	FILE* stdout_;
	FILE* stdin_;
	FILE* stderr_;
	FILE* oldstdout_;
	FILE* oldstdin_;
	FILE* oldstderr_;
}

namespace syd {

bool WinCreateConsole() {

	TCHAR szOutputPipeName[256];
	TCHAR szInputPipeName[256];
	TCHAR szErrorPipeName[256];

	DWORD cid = GetCurrentProcessId();
	
	oldstdout_ = stdout;
	oldstdin_ = stdin;
	oldstderr_ = stderr;

	sprintf( szOutputPipeName, "\\\\.\\pipe\\%dcout", cid );
	sprintf( szInputPipeName, "\\\\.\\pipe\\%dcin", cid );
	sprintf( szErrorPipeName, "\\\\.\\pipe\\%dcerr", cid );
	BOOL rc=FALSE;
/*
		rc=
		(stdout_=freopen( szOutputPipeName, "a", stdout )) != NULL &&
		(stdin_=freopen( szInputPipeName, "r", stdin )) != NULL &&
		(stderr_=freopen(szErrorPipeName, "a", stderr )) != NULL;
*/
	if(rc == FALSE) {
		rc = AllocConsole();

		if(rc != FALSE) {
			rc = (stdout_=freopen("CONOUT$", "a", stdout )) != NULL &&
				 (stdin_=freopen("CONIN$", "r", stdin )) != NULL &&
				 (stderr_=freopen("CONERR$", "a", stderr )) != NULL;
	
		}
	}

	if(rc != FALSE) {
		ios::sync_with_stdio();
	} else {
		//MessageBox(0,"error allocationg console",0,MB_OK);
	}

	return (bool)rc;

} //createConsole

bool WinFreeConsole() {
//	fclose(stdout_);
//	fclose(stdin_);
//	fclose(stderr_);
	BOOL res = FreeConsole();
	freopen("CONOUT$", "a", stdout );
	freopen("CONIN$", "r", stdin );
	freopen("CONERR$", "a", stderr );
	return res;
}

} //syd