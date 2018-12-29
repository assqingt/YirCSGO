#include "Debug.h"

BOOL WINAPI InitConsole() {
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);   // ----------------------------------------------
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);//��iostream��ʹ�����ǵĿ���̨���
	SetConsoleTitle(L"[ConsoleDebug]----OutDebugLog");
	HWND hConsole = GetConsoleWindow();
	if (!hConsole)return FALSE;
	return TRUE;
}

void WINAPI CloseConsole() {
	fclose((FILE*)stdin);
	fclose((FILE*)stdout);
	HWND hConsole = GetConsoleWindow();
	FreeConsole();
	PostMessage(hConsole, WM_CLOSE, NULL, NULL);
}