#pragma once
#include "main.h"
class Process
{
public:
	Process();
	~Process();
	DWORD ProcessName2Pid(LPCWSTR ProcName);
	void Pid2ProcessName(DWORD pid, LPCWSTR lpPrcsName, UINT lpLen);
	DWORD Hwnd2Pid(HWND hWnd);
	void Hwnd2ProcessName(HWND hWnd, LPCWSTR lpPrcsName, UINT lpLen);
	HWND Pid2Hwnd(DWORD pid);
	LONG_PTR GetModuleBaseAddr(LPCWSTR ModuleName,DWORD pid);
	bool ProcessNameIsexits(LPCWSTR ProcesName);
};

