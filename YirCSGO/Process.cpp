#include "Process.h"

Process::Process(){
}

Process::~Process(){
}
//进程名取进程ID;
DWORD Process::ProcessName2Pid(LPCWSTR ProcName) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (SnapShot == INVALID_HANDLE_VALUE)return 0;
	PROCESSENTRY32 ProcsEntry32 = { sizeof(PROCESSENTRY32),0 };
	if (!Process32First(SnapShot, &ProcsEntry32))return 0;
	DWORD Pidr = 0;
	do 
	{
		if (wcscmp(ProcsEntry32.szExeFile, ProcName) == 0) {
			Pidr = ProcsEntry32.th32ProcessID;
			break;
		}
	} while (Process32Next(SnapShot, &ProcsEntry32));
	CloseHandle(SnapShot);
	return Pidr;
}
//进程ID取进程名;
void Process::Pid2ProcessName(DWORD pid, LPCWSTR lpPrcsName, UINT lpLen) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (SnapShot == INVALID_HANDLE_VALUE)return;
	PROCESSENTRY32 ProcsEntry32 = { sizeof(PROCESSENTRY32),0 };
	if (!Process32First(SnapShot, &ProcsEntry32))return;
	do
	{
		if (ProcsEntry32.th32ProcessID == pid) {
			UINT StrLen = wcslen(ProcsEntry32.szExeFile) * sizeof(WCHAR);
			if (StrLen < lpLen)CopyMemory((void*)lpPrcsName, ProcsEntry32.szExeFile, StrLen + 1);
			else CopyMemory((void*)lpPrcsName, ProcsEntry32.szExeFile, lpLen - 1);
			break;
		}
	} while (Process32Next(SnapShot, &ProcsEntry32));
	CloseHandle(SnapShot);
	return;
}
//窗口枚举事件
BOOL WINAPI EmWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD *Cmdlie = nullptr, Pid = 0;
	GetWindowThreadProcessId(hWnd, &Pid);
	//取参数地址,强制转换地址;
	Cmdlie = (DWORD*)lParam;
	//判断参数1进程id是否与窗口相同;
	if (Pid == Cmdlie[0]) {
		//保存窗口句柄到参数2中;
		Cmdlie[1] = (DWORD)hWnd;
		return FALSE; 
	}
	else return TRUE;
}
//进程ID取窗口句柄;
HWND Process::Pid2Hwnd(DWORD pid) {
	//创建一个有俩个参数的参数数组;
	DWORD Cmdr[2] = { pid,0 };
	//枚举窗口并传递参数数组地址;
	EnumWindows(EmWindowsProc, (LPARAM)Cmdr);
	//返回参数数组2,窗口句柄;
	return (HWND)Cmdr[1];
}
//窗口句柄取进程ID;
DWORD Process::Hwnd2Pid(HWND hWnd) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (SnapShot == INVALID_HANDLE_VALUE)return 0;
	PROCESSENTRY32 ProcsEntry32 = { sizeof(PROCESSENTRY32),0 };
	if (!Process32First(SnapShot, &ProcsEntry32))return 0;
	DWORD Pidr = 0, RePid = 0;
	GetWindowThreadProcessId(hWnd, &Pidr);
	do
	{
		if (ProcsEntry32.th32ProcessID == Pidr) {
			RePid = ProcsEntry32.th32ProcessID;
			break;
		}
	} while (Process32Next(SnapShot, &ProcsEntry32));
	CloseHandle(SnapShot);
	return RePid;
}
//窗口句柄取进程ID;
void Process::Hwnd2ProcessName(HWND hWnd, LPCWSTR lpPrcsName, UINT lpLen) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (SnapShot == INVALID_HANDLE_VALUE)return;
	PROCESSENTRY32 ProcsEntry32 = { sizeof(PROCESSENTRY32),0 };
	if (!Process32First(SnapShot, &ProcsEntry32))return;
	DWORD Pidr = 0;
	UINT Stlen = 0;
	GetWindowThreadProcessId(hWnd, &Pidr);
	do
	{
		if (ProcsEntry32.th32ProcessID == Pidr) {
			Stlen = wcslen(ProcsEntry32.szExeFile) * sizeof(WCHAR) + 1;
			if (Stlen < lpLen)CopyMemory((void*)lpPrcsName, ProcsEntry32.szExeFile, Stlen);
			else CopyMemory((void*)lpPrcsName, ProcsEntry32.szExeFile, lpLen - 1);
			break;
		}
	} while (Process32Next(SnapShot, &ProcsEntry32));
	CloseHandle(SnapShot);
	return;
}
//进程ID取模块基址;
LONG_PTR Process::GetModuleBaseAddr(LPCWSTR ModuleName, DWORD pid) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (SnapShot == INVALID_HANDLE_VALUE)return 0;
	MODULEENTRY32 ModuleEty32 = { sizeof(MODULEENTRY32),0 };
	if (!Module32First(SnapShot, &ModuleEty32))return 0;
	LONG_PTR ModuleBase = 0;
	do 
	{
		if (wcscmp(ModuleEty32.szModule, ModuleName) == 0) {
			ModuleBase = (LONG_PTR)ModuleEty32.modBaseAddr;
			break;
		}
	} while (Module32Next(SnapShot, &ModuleEty32));
	CloseHandle(SnapShot);
	return ModuleBase;
}
bool Process::ProcessNameIsexits(LPCWSTR ProcesName) {
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (SnapShot == INVALID_HANDLE_VALUE)return false;
	PROCESSENTRY32 ProcsEntry32 = { sizeof(PROCESSENTRY32),0 };
	if (!Process32First(SnapShot, &ProcsEntry32))return false;
	DWORD Pidr = 0;
	do
	{
		if (wcscmp(ProcsEntry32.szExeFile, ProcesName) == 0) {
			return true;
			break;
		}
	} while (Process32Next(SnapShot, &ProcsEntry32));
	CloseHandle(SnapShot);
	return false;
}
