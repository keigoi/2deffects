// raylib.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�
#include <windows.h>
#include "utils/winutils/winconsole.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			syd::WinCreateConsole();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			syd::WinFreeConsole();
			break;
    }
    return TRUE;
}


