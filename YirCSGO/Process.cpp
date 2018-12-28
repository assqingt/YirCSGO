#include "Process.h"

Process::Process(){
}

Process::~Process(){
}
//������ȡ����ID;
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
//����IDȡ������;
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
//����ö���¼�
BOOL WINAPI EmWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD *Cmdlie = nullptr, Pid = 0;
	GetWindowThreadProcessId(hWnd, &Pid);
	//ȡ������ַ,ǿ��ת����ַ;
	Cmdlie = (DWORD*)lParam;
	//�жϲ���1����id�Ƿ��봰����ͬ;
	if (Pid == Cmdlie[0]) {
		//���洰�ھ��������2��;
		Cmdlie[1] = (DWORD)hWnd;
		return FALSE; 
	}
	else return TRUE;
}
//����IDȡ���ھ��;
HWND Process::Pid2Hwnd(DWORD pid) {
	//����һ�������������Ĳ�������;
	DWORD Cmdr[2] = { pid,0 };
	//ö�ٴ��ڲ����ݲ��������ַ;
	EnumWindows(EmWindowsProc, (LPARAM)Cmdr);
	//���ز�������2,���ھ��;
	return (HWND)Cmdr[1];
}
//���ھ��ȡ����ID;
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
//���ھ��ȡ����ID;
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
//����IDȡģ���ַ;
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
