#include "main.h"
HWND ghwnd = 0, hCSGOWindow = 0;
bool gDraw = true, Cheat_Crosshair = true, Win_nShowUI = true, Cheat_BoxEsp = true, Cheat_BoneEsp = false, Cheat_LowObject = true,
Cheat_Aimbot = true, Cheat_AutoPressueGun = false, Cheat_Spotted = true, Cheat_Team = true, Cheat_BhopJump = false;
UINT Cl_Width = 0, Cl_Height = 0;
MenuItem gMenItem[MenuItemNum] = { 0 };
HANDLE HProcess = nullptr;
DWORD_PTR ClientdllBase = 0, EnginedllBase = 0;
BOOL WINAPI InitPrcoess() {
	Process Prc;
	DWORD pid = Prc.ProcessName2Pid(L"csgo.exe");
	hCSGOWindow = Prc.Pid2Hwnd(pid);
	//hCSGOWindow = FindWindow(L"Valve001",nullptr);
	if (!hCSGOWindow) { MessageBox(nullptr, L"���ȿ�����Ϸ!", nullptr, NULL); return FALSE; }
	//���������̺߳Ͱ�������߳�
	HProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (HProcess == INVALID_HANDLE_VALUE) { MessageBox(nullptr, L"��Ϸ���̴�ʧ��,��ȷ���Ƿ����Ա����!", nullptr, MB_OK); return FALSE; }
	ClientdllBase = Prc.GetModuleBaseAddr(L"client_panorama.dll", pid);
	EnginedllBase = Prc.GetModuleBaseAddr(L"engine.dll", pid);
	if (!EnginedllBase || !ClientdllBase) { MessageBox(nullptr, L"��Ϸ��ַ��ȡʧ��,��ȷ���Ƿ����Ա���л���Ϸδ����!", nullptr, MB_OK); return FALSE; }
	return TRUE;
}
void WINAPI InitMenu() {
	LOGFONT fontr;
	//��ȡԭ���岢�޸ĳ�����
	gettextstyle(&fontr);
	fontr.lfHeight = 12;
	fontr.lfWeight = FW_THIN;
	fontr.lfWidth = 5;
	fontr.lfItalic = false;
	CopyMemory(fontr.lfFaceName, L"����", 6);
	settextstyle(&fontr);
	//��Ӳ˵���
	AddMenuItem(L"��ӭʹ��Yir����-�汾:V1.21", Menu_Other, CheatType_Other, false);
	AddMenuItem(L"����͸��", Menu_Cheat, CheatType_BoxEsp, Cheat_BoxEsp);
	AddMenuItem(L"���͸��", Menu_Cheat, CheatType_LowObject, Cheat_LowObject);
	//AddMenuItem(L"����͸��", Menu_Cheat, CheatType_BoneEsp, Cheat_BoneEsp);
	AddMenuItem(L"�Զ���׼--F11", Menu_Cheat, CheatType_Aimbot, Cheat_Aimbot);
	AddMenuItem(L"����ѹǹ", Menu_Cheat, CheatType_AutoPressueGun, Cheat_AutoPressueGun);
	AddMenuItem(L"���Ӽ��", Menu_Cheat, CheatType_Spotted, Cheat_Spotted);
	AddMenuItem(L"����׼��", Menu_Cheat, CheatType_Crosshair, Cheat_Crosshair);
	AddMenuItem(L"���Ѽ��", Menu_Cheat, CheatType_Team, Cheat_Team);
	AddMenuItem(L"�Զ�����--F3", Menu_Cheat, CheatType_BhopJump, Cheat_BhopJump);
	AddMenuItem(L"����Ⱥ:1314520", Menu_Other, CheatType_Other, false);
	gMenItem[0].SelectItem = true;//Ĭ��ѡ�в˵�����
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	MAPO_VM_START;
	if (!InitPrcoess())ExitProcess(NULL);
	ghwnd = initgraph(Init_Width, Init_Height);
	//���ô��ڷ�� ----->����͸��,��괩͸,����ʾ��������;
	SetWindowLong(ghwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(ghwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
	// ��ʼ���˵�;
	InitMenu();
	InitConsole();
	printf("����̨��װ�ɹ�\n");
	//��ʼ�������߳�;
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)DrawUIThread, nullptr, 0, nullptr);
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)KeyState, nullptr, 0, nullptr);
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	closegraph();
	return 0;
	MAPO_VM_END
}
//���û��ƴ���λ��
void WINAPI SetUILocation() {
	MAPO_VM_START
	RECT ClRect;
	GetWindowRect(hCSGOWindow, &ClRect);
	//�жϴ����Ƿ��ޱ߿�
	if (GetWindowLong(hCSGOWindow, GWL_STYLE) == 0x94000000) {
		Cl_Height = (ClRect.bottom - ClRect.top);
		Cl_Width = (ClRect.right - ClRect.left);
		SetWindowLong(ghwnd, GWL_STYLE, WS_VISIBLE);
		SetWindowPos(ghwnd, HWND_TOPMOST, ClRect.left, ClRect.top, ClRect.right, ClRect.bottom, SWP_NOSENDCHANGING);
	}
	else {
		Cl_Height = (ClRect.bottom - ClRect.top);
		Cl_Width = (ClRect.right - ClRect.left);
		SetWindowLong(ghwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOPMOST, ClRect.left - 5, ClRect.top - 5, (ClRect.right - ClRect.left) + 10, (ClRect.bottom - ClRect.top) + 10, SWP_NOSENDCHANGING);

	}
	MAPO_VM_END
}
void WINAPI GetCSGORECT(UINT *GameWidth, UINT *GameHeight) {
	MAPO_VM_START
	RECT ClRect;
	GetClientRect(hCSGOWindow, &ClRect);
	*GameWidth = ClRect.right - ClRect.left;
	*GameHeight = ClRect.bottom - ClRect.top;
	return;
	MAPO_VM_END
}
//�����߳�
void WINAPI DrawUIThread() {
	MAPO_VM_START
	while (gDraw)
	{
		clearcliprgn();
		if (GetForegroundWindow() == hCSGOWindow) {
			ShowWindow(ghwnd, SW_SHOW);
			BeginBatchDraw();
			UINT GameWidth = 0, GameHeight = 0;
			GetCSGORECT(&GameWidth, &GameHeight);
			SetUILocation();
			if (Cheat_Crosshair)DrawTrajectoryCrosshair(GameWidth / 2, GameHeight / 2);
			if (Win_nShowUI)DrawMenu();
			if (Cheat_BoxEsp)DrawBoxEsp(GameWidth, GameHeight);
			if (Cheat_BoneEsp)DrawBoneEsp(GameWidth, GameHeight);
			if (Cheat_LowObject)LowObjectEsp();
			if (Cheat_Aimbot)AimbotEntity();
			EndBatchDraw();
		}
		else ShowWindow(ghwnd, SW_HIDE);
	}
	MAPO_VM_END
}
void WINAPI SetCheatType(UINT nCheatType) {
	MAPO_VM_START
	switch (nCheatType)
	{
	case CheatType_BoxEsp:
		gMenItem[CheatType_BoxEsp].ONFF = !gMenItem[CheatType_BoxEsp].ONFF;
		Cheat_BoxEsp = !Cheat_BoxEsp;
		break;
	case CheatType_LowObject:
		gMenItem[CheatType_LowObject].ONFF = !gMenItem[CheatType_LowObject].ONFF;
		Cheat_LowObject = !Cheat_LowObject;
		break;
		//����͸����ʱȡ��
	/*case CheatType_BoneEsp:
		gMenItem[CheatType_BoneEsp].ONFF = !gMenItem[CheatType_BoneEsp].ONFF;
		Cheat_BoneEsp = !Cheat_BoneEsp;
		break;*/
	case CheatType_Aimbot:
		gMenItem[CheatType_Aimbot].ONFF = !gMenItem[CheatType_Aimbot].ONFF;
		Cheat_Aimbot = !Cheat_Aimbot;
		break;
	case CheatType_AutoPressueGun:
		gMenItem[CheatType_AutoPressueGun].ONFF = !gMenItem[CheatType_AutoPressueGun].ONFF;
		Cheat_AutoPressueGun = !Cheat_AutoPressueGun;
		break;
	case CheatType_Spotted:
		gMenItem[CheatType_Spotted].ONFF = !gMenItem[CheatType_Spotted].ONFF;
		Cheat_Spotted = !Cheat_Spotted;
		break;
	case CheatType_Crosshair:
		gMenItem[CheatType_Crosshair].ONFF = !gMenItem[CheatType_Crosshair].ONFF;
		Cheat_Crosshair = !Cheat_Crosshair;
		break;
	case CheatType_Team:
		gMenItem[CheatType_Team].ONFF = !gMenItem[CheatType_Team].ONFF;
		Cheat_Team = !Cheat_Team;
		break;
	case CheatType_BhopJump:
		gMenItem[CheatType_BhopJump].ONFF = !gMenItem[CheatType_BhopJump].ONFF;
		Cheat_BhopJump = !Cheat_BhopJump;
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)BhopJump, nullptr, 0, nullptr);
		break;
	}
	MAPO_VM_END
}
//��ذ���״̬
UINT bin = 0;
void WINAPI KeyState() {
	MAPO_VM_START
	while (gDraw)
	{
		Process Prc;
		if (!Prc.ProcessNameIsexits(L"csgo.exe"))ExitProcess(0);
		if (GetAsyncKeyState(VK_HOME) & 0x8000)Win_nShowUI = !Win_nShowUI;
#pragma region �˵����ܲ���
		if (GetAsyncKeyState(VK_UP) & 0x8000 && Win_nShowUI) {
			for (int i = 0; i < MenuItemNum; i++) {
				if (i - 1 >= 0) {
					if (gMenItem[i].SelectItem) {
						gMenItem[i - 1].SelectItem = true;
						gMenItem[i].SelectItem = false;
						break;
					}
				}
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && Win_nShowUI) {
			for (int i = 0; i < MenuItemNum; i++) {
				if (i + 1 <= MenuItemNum && gMenItem[i + 1].lpItem) {
					if (gMenItem[i].SelectItem) {
						gMenItem[i + 1].SelectItem = true;
						gMenItem[i].SelectItem = false;
						break;
					}
				}
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && Win_nShowUI) {
			for (int i = 0; i < MenuItemNum; i++) {
				if (gMenItem[i].SelectItem) {
					SetCheatType(i);
					break;
				}
			}
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && Win_nShowUI) {
			for (int i = 0; i < MenuItemNum; i++) {
				if (gMenItem[i].SelectItem) {
					SetCheatType(i);
					break;
				}
			}
		}
#pragma endregion
		if (GetAsyncKeyState(VK_F3) & 0x8000) SetCheatType(CheatType_BhopJump);
		Sleep(130);
	}
	MAPO_VM_END
}
//���Ʋ˵���
void WINAPI DrawMenu() {
	MAPO_VM_START
	setcolor(RGB(156, 164, 255));
	rectangle(0, 0, CMenu_Width, CMenu_Height);
	for (int i = 0; i < MenuItemNum; i++) {
		if (gMenItem[i].lpItem) {
			setcolor(RGB(250, 164, 255));
			if (gMenItem[i].SelectItem)fillrectangle(CItem_Left, i*CItem_Height + 2, CItem_Width, i*CItem_Height + CItem_Height);
			else rectangle(CItem_Left, i*CItem_Height + 2, CItem_Width, i*CItem_Height + CItem_Height);
			outtextxy(CItemText_left, i*CItem_Height + 6, gMenItem[i].lpItem);
			setcolor(RGB(250, 0, 155));
			if (gMenItem[i].ONFF == true && gMenItem[i].Type != Menu_Other)outtextxy(CItem_Width - CItemONF_Left, i*CItem_Height + 6, L"<��>");
			else if (gMenItem[i].ONFF == false && gMenItem[i].Type != Menu_Other) { setcolor(RGB(250, 0, 0)); outtextxy(CItem_Width - CItemONF_Left, i*CItem_Height + 6, L"<��>"); }
		}
	}
	SetDefaultColor();
	MAPO_VM_END
}
//��Ӳ˵���
void WINAPI AddMenuItem(LPCWSTR lpItem,UINT Type,UINT CheatType,bool Onff) {
	MAPO_VM_START
	for (int i = 0; i < MenuItemNum; i++) {
		if (!gMenItem[i].lpItem) { 
			gMenItem[i].lpItem = lpItem; 
			gMenItem[i].Type = Type;
			gMenItem[i].CheatType = CheatType;
			gMenItem[i].ONFF = Onff;
			break; 
		}
	}
	MAPO_VM_END
}
//���Ƶ���׼��
void WINAPI DrawTrajectoryCrosshair(int x, int y) {
	MAPO_VM_START
	DWORD_PTR LocalPlayBase = 0;
	ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
	CreenXY AimPunchAngle = { 0 };
	ReadGameMemory((LPVOID)(LocalPlayBase + dwaimPunchAngleVel), &AimPunchAngle, sizeof(AimPunchAngle));
	if (AimPunchAngle.x < -3.f)AimPunchAngle.x += 9.f;
	x = (int)x - (int)AimPunchAngle.y;
	y = (int)y + (int)AimPunchAngle.x;
	setcolor(RGB(0, 0, 250));
	line(x - 4, y, x + 4, y);
	line(x, y - 4, x, y + 4);
	SetDefaultColor();
	MAPO_VM_END
}
//����Ĭ����ɫ(��ɫ)
void WINAPI SetDefaultColor() { MAPO_VM_START setcolor(RGB(255, 255, 255)); MAPO_VM_END }
#pragma region ���׹��ܴ�����
//�ڴ��д����
BOOL WINAPI ReadGameMemory(LPCVOID lpBaseAddress, LPVOID ReadDword, UINT ReadLen) { 
	MAPO_VM_START
	return ReadProcessMemory(HProcess, lpBaseAddress, ReadDword, ReadLen, nullptr); 
	MAPO_VM_END
}
BOOL WINAPI WriteGameMemory(LPVOID lpBaseAddress, LPCVOID WriteDword, UINT WriteLen) {
	MAPO_VM_START
		return WriteProcessMemory(HProcess, lpBaseAddress, WriteDword, WriteLen, nullptr);
	MAPO_VM_END
}
//���Ʒ���͸��
void WINAPI DrawBoxEsp(UINT GameWidth, UINT GameHeight) {
	MAPO_VM_START;
	D3DMATRIX ViewMatrix;
	DWORD_PTR LocalPlayBase;
	UINT LoalPlayHealth, LoalPlayCamps;
	D3DVECTOR LoalPlayPlace;
	if (Cheat_BoxEsp) {
		ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerHealth), &LoalPlayHealth, sizeof(LoalPlayHealth));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerCamps), &LoalPlayCamps, sizeof(LoalPlayCamps));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerPlace), &LoalPlayPlace, sizeof(LoalPlayPlace));
		DWORD iFov;
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerFov), &iFov, sizeof(iFov));
		ReadGameMemory((LPVOID)(ClientdllBase + dwViewMatrix), &ViewMatrix, sizeof(ViewMatrix));
		DWORD_PTR EntityListBase = ClientdllBase + dwEnemyPlayer;
		//ѭ������ʵ����Ϣ
		for (int i = 0; i < MaxPlayerNum; i++)
		{
			DWORD_PTR EntityAddress;
			ReadGameMemory((LPVOID)(EntityListBase + (i * 0x10)), &EntityAddress, sizeof(EntityAddress));
			//�ж�ʵ���Ƿ��Լ������������;
			if (EntityAddress == LocalPlayBase || EntityAddress <= 0)continue;
			UINT EntityHealth, EntityCamps, EntityArmor;
			D3DVECTOR EntityPlace;
			DWORD IsDormant;
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerHealth), &EntityHealth, sizeof(EntityHealth));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerCamps), &EntityCamps, sizeof(EntityCamps));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerPlace), &EntityPlace, sizeof(EntityPlace));
			ReadGameMemory((LPVOID)(EntityAddress + dwArmorValue), &EntityArmor, sizeof(EntityArmor));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerDormant), &IsDormant, sizeof(IsDormant));
			//�ж�ʵ���Ƿ�����Լ�3m��
			if (fabsf(GetD3Distance(EntityPlace, LoalPlayPlace) / 10.f) < 6.f)continue;
			if (EntityHealth > 0 && IsDormant == false) {
				CreenXY EntityCreen;
				float Scale;
				if (!WorldToScreen(EntityPlace, ViewMatrix, &EntityCreen, GameWidth, GameHeight,&Scale))continue;
				Scale = FovToScale(iFov, Scale);
				if (EntityHealth > 100)EntityHealth = 100;
				//��Ӫ�жϻ���
				if (EntityCamps == LoalPlayCamps && !Cheat_Team) {
					//���ƶ��ѷ���
					setcolor(RGB(0, 255, 0));
					float Box_left = EntityCreen.x - 10 / Scale, Box_right = EntityCreen.x + 10 / Scale;
					rectangle((int)(Box_left), (int)EntityCreen.y, (int)(Box_right), (int)(EntityCreen.y - 45 / Scale));
					//����Ѫ������
					setcolor(RGB(156, 155, 165));
					float Hp_Height = Scale;
					if (Hp_Height < 1)Hp_Height = 1.f;
					float HealthBox_Top = EntityCreen.y - 5 / Hp_Height;
					rectangle((int)(Box_left), (int)HealthBox_Top, (int)(Box_right), (int)EntityCreen.y);
					setcolor(RGB(100, 255, 0));
					float HealthBox_width = ((float)EntityHealth*(Box_right - Box_left)) / 100.f;
					fillrectangle((int)Box_left, (int)HealthBox_Top, (int)(Box_left + HealthBox_width), (int)EntityCreen.y);
					//���ƻ���ֵ����
					setcolor(RGB(156, 155, 165));
					float ArmorBox_Top = EntityCreen.y + 5 / Hp_Height;
					rectangle((int)(Box_left), (int)ArmorBox_Top, (int)(Box_right), (int)EntityCreen.y);
					setcolor(RGB(165, 55, 150));
					float ArmorBox_width = ((float)EntityArmor*(Box_right - Box_left)) / 100.f;
					fillrectangle((int)Box_left, (int)ArmorBox_Top, (int)(Box_left + ArmorBox_width), (int)EntityCreen.y);
					SetDefaultColor();

				}
				else if (EntityCamps != LoalPlayCamps) {
					//���Ƶ��˷���
					setcolor(RGB(255, 0, 0));
					float Box_left = EntityCreen.x - 10 / Scale, Box_right = EntityCreen.x + 10 / Scale;
					rectangle((int)(Box_left), (int)EntityCreen.y, (int)(Box_right), (int)(EntityCreen.y - 45 / Scale));
					//����Ѫ������
					setcolor(RGB(156, 155, 165));
					float Hp_Height = Scale;
					if (Hp_Height < 1)Hp_Height = 1.f;
					float HealthBox_Top = EntityCreen.y - 5 / Hp_Height;
					rectangle((int)(Box_left), (int)HealthBox_Top, (int)(Box_right), (int)EntityCreen.y);
					setcolor(RGB(100, 255, 0));
					float HealthBox_width = ((float)EntityHealth*(Box_right - Box_left)) / 100.f;
					fillrectangle((int)Box_left, (int)HealthBox_Top, (int)(Box_left + HealthBox_width), (int)EntityCreen.y);
					//���ƻ���ֵ����
					setcolor(RGB(156, 155, 165));
					float ArmorBox_Top = EntityCreen.y + 5 / Hp_Height;
					rectangle((int)(Box_left), (int)ArmorBox_Top, (int)(Box_right), (int)EntityCreen.y);
					setcolor(RGB(165, 55, 150));
					float ArmorBox_width = ((float)EntityArmor*(Box_right - Box_left)) / 100.f;
					fillrectangle((int)Box_left, (int)ArmorBox_Top, (int)(Box_left + ArmorBox_width), (int)EntityCreen.y);
					SetDefaultColor();
				}
			}
		}
	}
	MAPO_VM_END
}
//�Զ������߳�
void WINAPI BhopJump() {
	MAPO_VM_START
	while (Cheat_BhopJump)
	{
		DWORD_PTR LocalPlayBase = 0;
		ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + 0x24), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + 0x2c), &LocalPlayBase, sizeof(LocalPlayBase));
		DWORD JumpState;
		ReadGameMemory((LPVOID)(LocalPlayBase + 0x654), &JumpState, sizeof(JumpState));
		BeginBatchDraw();
		outtextxy(Cl_Width / 2 - 70, Cl_Height / 2 - 20, L"������������Ұڶ�ͬʱ����A/D!");
		EndBatchDraw(Cl_Width / 2 - 70, Cl_Height / 2 - 20, Cl_Width / 2 + 170, Cl_Width / 2 + 30);
		if (JumpState == 257) {
			JumpState = 6;
			WriteGameMemory((LPVOID)(ClientdllBase + dwForceJump), &JumpState, sizeof(JumpState));
			Sleep(30);
			JumpState = 4;
			WriteGameMemory((LPVOID)(ClientdllBase + dwForceJump), &JumpState, sizeof(JumpState));
		}
	}
	MAPO_VM_END
}
//���ƹ���͸��
void WINAPI DrawBoneEsp(UINT GameWidth, UINT GameHeight) {
	MAPO_VM_START
	matrix_t BoneMatrix[128];
	DWORD_PTR LocalPlayBase;
	UINT LoalPlayCamps;
	D3DMATRIX ViewMatrix;
	D3DVECTOR LoalPlayPlace;
	if (Cheat_BoneEsp) {
		DWORD iFov;
		ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerCamps), &LoalPlayCamps, sizeof(LoalPlayCamps));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerPlace), &LoalPlayPlace, sizeof(LoalPlayPlace));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerFov), &iFov, sizeof(iFov));
		ReadGameMemory((LPVOID)(ClientdllBase + dwViewMatrix), &ViewMatrix, sizeof(ViewMatrix));
		DWORD_PTR EntityListBase = ClientdllBase + dwEnemyPlayer;
		//ѭ������ʵ����Ϣ
		for (int i = 0; i < MaxPlayerNum; i++)
		{
			DWORD_PTR TmpAddress;
			ReadGameMemory((LPVOID)(EntityListBase + (i * 0x10)), &TmpAddress, sizeof(TmpAddress));
			//�ж�ʵ���Ƿ��Լ������������;
			if (TmpAddress == LocalPlayBase)continue;
			UINT EntityHealth, EntityCamps;
			DWORD IsDormant;
			ReadGameMemory((LPVOID)(TmpAddress + dwPlayerHealth), &EntityHealth, sizeof(EntityHealth));
			ReadGameMemory((LPVOID)(TmpAddress + dwPlayerCamps), &EntityCamps, sizeof(EntityCamps));
			ReadGameMemory((LPVOID)(TmpAddress + dwPlayerDormant), &IsDormant, sizeof(IsDormant));
			//���ߺ�Ѫ���ж�
			if (EntityHealth != 0 && IsDormant == false) {
				ReadGameMemory((LPVOID)(TmpAddress + dwBoneMatrix), &TmpAddress, sizeof(TmpAddress));
				ReadGameMemory((LPVOID)(TmpAddress), &BoneMatrix, sizeof(BoneMatrix));
				//������� 0:ƨ�� 8:ͷ�� 9:���� 11: ��� 40:�Ҽ� 70:ϥ��4:�� 5:����6:�ز�12:����13:����41:����42:����69:����,71:���76����77:��ϥ78:�ҽ�
				UINT nIndex[Bone_MaxIndex];
				//������Ӫѡ���Ӧ��������
				if (EntityCamps==3) {
#pragma region �����������
					UINT ndex[] = { CT_BoneIndex_ͷ��,CT_BoneIndex_�ز�,CT_BoneIndex_��� ,CT_BoneIndex_�Ҽ� ,CT_BoneIndex_���� ,CT_BoneIndex_���� ,CT_BoneIndex_����
					,CT_BoneIndex_���� ,CT_BoneIndex_����,CT_BoneIndex_����,CT_BoneIndex_�β� , CT_BoneIndex_��ϥ , CT_BoneIndex_��ϥ , CT_BoneIndex_��� , CT_BoneIndex_�ҽ�
					};
					CopyMemory(nIndex, ndex, sizeof(ndex));
#pragma endregion
				}
				else {
#pragma region ��ͽ��������
					UINT ndex[] = { T_BoneIndex_ͷ��,T_BoneIndex_�ز�,T_BoneIndex_��� ,T_BoneIndex_�Ҽ� ,T_BoneIndex_���� ,T_BoneIndex_���� ,T_BoneIndex_����
					,T_BoneIndex_���� ,T_BoneIndex_����,T_BoneIndex_����,T_BoneIndex_�β� , T_BoneIndex_��ϥ , T_BoneIndex_��ϥ , T_BoneIndex_��� , T_BoneIndex_�ҽ�
					};
					CopyMemory(nIndex, ndex, sizeof(ndex));
#pragma endregion
				}
#pragma region ������������
				//������������
				CreenXY BoneCreen[Bone_MaxIndex] = { 0 };
				float Scale;
				D3DVECTOR BonePlace;
				bool IsChr = true;
				for (int ii = 0; ii < Bone_MaxIndex; ii++)
				{
					BonePlace = { BoneMatrix[nIndex[ii]][0][3],BoneMatrix[nIndex[ii]][1][3] ,BoneMatrix[nIndex[ii]][2][3] };
					if (!WorldToScreen(BonePlace, ViewMatrix, &BoneCreen[ii], GameWidth, GameHeight, &Scale)) { IsChr = false; break; }
					Scale = FovToScale(iFov, Scale);
				}
				float Dist = fabsf(GetD3Distance(BonePlace, LoalPlayPlace) / 10.f);
				if (Dist < 6.f || Dist>100.f)continue;
#pragma endregion
#pragma region ��������
				if (IsChr) {
					if (Cheat_Team) {
						if (EntityCamps == LoalPlayCamps)setcolor(RGB(0, 255, 0));
						else setcolor(RGB(255, 0, 0));
						line((int)BoneCreen[Bone_ͷ��].x, (int)BoneCreen[Bone_ͷ��].y, (int)BoneCreen[Bone_�ز�].x, (int)BoneCreen[Bone_�ز�].y);
						line((int)BoneCreen[Bone_�ز�].x, (int)BoneCreen[Bone_�ز�].y, (int)BoneCreen[Bone_���].x, (int)BoneCreen[Bone_���].y);
						line((int)BoneCreen[Bone_�ز�].x, (int)BoneCreen[Bone_�ز�].y, (int)BoneCreen[Bone_�Ҽ�].x, (int)BoneCreen[Bone_�Ҽ�].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_���].x, (int)BoneCreen[Bone_���].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_�Ҽ�].x, (int)BoneCreen[Bone_�Ҽ�].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y);
						line((int)BoneCreen[Bone_�ز�].x, (int)BoneCreen[Bone_�ز�].y, (int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y);
						line((int)BoneCreen[Bone_����].x, (int)BoneCreen[Bone_����].y, (int)BoneCreen[Bone_�β�].x, (int)BoneCreen[Bone_�β�].y);
						line((int)BoneCreen[Bone_�β�].x, (int)BoneCreen[Bone_�β�].y, (int)BoneCreen[Bone_��ϥ].x, (int)BoneCreen[Bone_��ϥ].y);
						line((int)BoneCreen[Bone_�β�].x, (int)BoneCreen[Bone_�β�].y, (int)BoneCreen[Bone_��ϥ].x, (int)BoneCreen[Bone_��ϥ].y);
						line((int)BoneCreen[Bone_���].x, (int)BoneCreen[Bone_���].y, (int)BoneCreen[Bone_��ϥ].x, (int)BoneCreen[Bone_��ϥ].y);
						line((int)BoneCreen[Bone_�ҽ�].x, (int)BoneCreen[Bone_�ҽ�].y, (int)BoneCreen[Bone_��ϥ].x, (int)BoneCreen[Bone_��ϥ].y);
					}
				}
#pragma endregion
				/*D3DVECTOR BonePlace2[2];
				CreenXY BoneCreen2[2];
				for (int ii = 0; ii < 127; ii++) {
					BonePlace2[0].x = BoneMatrix[ii][0][3];
					BonePlace2[0].y = BoneMatrix[ii][1][3];
					BonePlace2[0].z = BoneMatrix[ii][2][3];
					if (!WorldToScreen(BonePlace2[0], ViewMatrix, &BoneCreen2[0], GameWidth, GameHeight, &Scale))continue;
					Scale = FovToScale(iFov, Scale);
					//solidcircle((int)BoneCreen.x, (int)BoneCreen.y, 1);
					/ *WCHAR but[25];
					swprintf(but, L"%d", ii);
					outtextxy((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, but);* /
					BonePlace2[1].x = BoneMatrix[ii + 1][0][3];
					BonePlace2[1].y = BoneMatrix[ii + 1][1][3];
					BonePlace2[1].z = BoneMatrix[ii + 1][2][3];
					if (!WorldToScreen(BonePlace2[1], ViewMatrix, &BoneCreen2[1], GameWidth, GameHeight, &Scale))continue;
					Scale = FovToScale(iFov, Scale);
					if (ii > 9 && ii <= 13) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii >= 3 && ii < 8) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii > 38 && ii < 42) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii > 72 && ii < 75) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii >= 66 && ii < 68) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii > 80 && ii < 83) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
					else if (ii == 6) {
						line((int)BoneCreen2[0].x, (int)BoneCreen2[0].y, (int)BoneCreen2[1].x, (int)BoneCreen2[1].y);
					}
				}*/
			}
		}
	}
	MAPO_VM_END
}
//���͸��
void WINAPI LowObjectEsp() {
	MAPO_VM_START;
	if (Cheat_LowObject) {
		DWORD_PTR GLowObjectBase, LocalPlayBase;
		UINT LoalPlayCamps;
		ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerCamps), &LoalPlayCamps, sizeof(LoalPlayCamps));
		ReadGameMemory((LPVOID)(ClientdllBase + dwGlowObjectManager), &GLowObjectBase, sizeof(GLowObjectBase));
		DWORD_PTR EntityListBase = ClientdllBase + dwEnemyPlayer;
		for (int i = 0; i < MaxPlayerNum; i++)
		{
			DWORD_PTR EntityAddress, GlowIndex = 0;
			UINT EntityiHealth, EntityCamps;
			bool IsDormant;
			ReadGameMemory((LPVOID)(EntityListBase + (i * 0x10)), &EntityAddress, sizeof(EntityAddress));
			if (EntityAddress == LocalPlayBase || EntityAddress <= 0)continue;
			ReadGameMemory((LPVOID)(EntityAddress + dwiGlowIndex), &GlowIndex, sizeof(GlowIndex));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerHealth), &EntityiHealth, sizeof(EntityiHealth));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerCamps), &EntityCamps, sizeof(EntityCamps));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerDormant), &IsDormant, sizeof(IsDormant));
			if (EntityiHealth > 0 && IsDormant == false) {
				if (!Cheat_Team && EntityCamps == LoalPlayCamps) {
					//���Ӽ��
					if (Cheat_Spotted && EntIsVisible(EntityAddress, LocalPlayBase))continue;
					GlowBase EntityLow;
					ReadGameMemory((LPVOID)(GLowObjectBase + (GlowIndex * 0x38) + 0x4), &EntityLow, sizeof(EntityLow));
					DWORD entityadr = GLowObjectBase + (GlowIndex * 0x38);
					EntityLow.r = 0.f;
					EntityLow.g = 0.5f;
					EntityLow.b = 0.f;
					EntityLow.a = 1.f;
					EntityLow.m_bRenderWhenOccluded = true;
					WriteGameMemory((LPVOID)(entityadr + 0x4), &EntityLow, sizeof(EntityLow));
				}
				else if (EntityCamps != LoalPlayCamps) {
					//���Ӽ��
					if (Cheat_Spotted && EntIsVisible(EntityAddress, LocalPlayBase))continue;
					GlowBase EntityLow;
					ReadGameMemory((LPVOID)(GLowObjectBase + (GlowIndex * 0x38) + 0x4), &EntityLow, sizeof(EntityLow));
					DWORD entityadr = GLowObjectBase + (GlowIndex * 0x38);
					EntityLow.r = 0.5f;
					EntityLow.g = 0.f;
					EntityLow.b = 0.f;
					EntityLow.a = 1.f;
					EntityLow.m_bRenderWhenOccluded = true;
					WriteGameMemory((LPVOID)(entityadr + 0x4), &EntityLow, sizeof(EntityLow));
				}
			}
		}
	}
	MAPO_VM_END;
}
//�����Ƿ�ɼ�
bool WINAPI EntIsVisible(DWORD ent, DWORD local)
{
	MAPO_VM_START
	int mask, PBASE;
	ReadGameMemory((LPVOID)(ent + dwbSpottedByMask), &mask, sizeof(mask));
	ReadGameMemory((LPVOID)(local + 0x64), &PBASE, sizeof(PBASE));
	PBASE--;
	return (mask & (1 << PBASE)) > 0;
	MAPO_VM_END
}
//�Զ���׼
void WINAPI AimbotEntity() {
	DWORD_PTR LocalPlayBase,EngineStateBase;
	UINT LoalPlayHealth, LoalPlayCamps;
	D3DVECTOR LoalPlayPlace;
	if (Cheat_Aimbot) {
		ReadGameMemory((LPVOID)(ClientdllBase + dwLocalPlayer), &LocalPlayBase, sizeof(LocalPlayBase));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerHealth), &LoalPlayHealth, sizeof(LoalPlayHealth));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerCamps), &LoalPlayCamps, sizeof(LoalPlayCamps));
		ReadGameMemory((LPVOID)(LocalPlayBase + dwPlayerPlace), &LoalPlayPlace, sizeof(LoalPlayPlace));
		ReadGameMemory((LPVOID)(EnginedllBase + dwEngineState), &EngineStateBase, sizeof(EngineStateBase));
		DWORD_PTR EntityListBase = ClientdllBase + dwEnemyPlayer;
		D3DMATRIX ViewMatrix;
		ReadGameMemory((LPVOID)(ClientdllBase + dwViewMatrix), &ViewMatrix, sizeof(ViewMatrix));
		//ѭ������ʵ����Ϣ
		for (int i = 0; i < MaxPlayerNum; i++)
		{
			DWORD_PTR EntityAddress;
			ReadGameMemory((LPVOID)(EntityListBase + (i * 0x10)), &EntityAddress, sizeof(EntityAddress));
			//�ж�ʵ���Ƿ��Լ������������;
			if (EntityAddress == LocalPlayBase || EntityAddress <= 0)continue;
			UINT EntityHealth, EntityCamps;
			D3DVECTOR EntityPlace;
			DWORD IsDormant;
			CreenXY MousAngles;
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerHealth), &EntityHealth, sizeof(EntityHealth));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerCamps), &EntityCamps, sizeof(EntityCamps));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerPlace), &EntityPlace, sizeof(EntityPlace));
			ReadGameMemory((LPVOID)(EntityAddress + dwPlayerDormant), &IsDormant, sizeof(IsDormant));
			ReadGameMemory((LPVOID)(EngineStateBase + dwMouseAngles), &MousAngles, sizeof(MousAngles));
			float Angles[3];
			if (EntityHealth > 0 && IsDormant == false) {
				DWORD_PTR BoneAddress;
				ReadGameMemory((LPVOID)(EntityAddress + dwBoneMatrix), &BoneAddress, sizeof(BoneAddress));
				matrix_t BoneMatrix[128];
				ReadGameMemory((LPVOID)(BoneAddress), &BoneMatrix, sizeof(BoneMatrix));
				float Distance = fabsf(GetD3Distance(EntityPlace, LoalPlayPlace)) / 10.f;
				if (Distance < 130.f) {
					/*D3DVECTOR localAngles, aimpunch;
					float smoothed[2];
					D3DVECTOR aimpunch = mem.ReadMemory<Vector>(csgo, localPlayer + aimPunch);
					aimpunch.x = aimpunch.x * 2.f;
					aimpunch.y = aimpunch.y * 2.f;
					DWORD x = mem.ReadMemory<DWORD>(csgo, localPlayer + m_hActiveWeapon) & 0xfff;
					DWORD y = mem.ReadMemory<DWORD>(csgo, client + entityList + (x - 1) * 0x10);
					short z = mem.ReadMemory<short>(csgo, y + m_iItemDefinitionIndex);
					localAngles = mem.ReadMemory<Vector>(csgo, clientbase + clientAngle);
					CalcAngle(Players[0].Pos, Players[Ind].Pos, Players[Ind].Angle);
					Smooth(aimpunch.x, aimpunch.y, Players[Ind].Angle, smoothed, localAngles, aimsmooth, z);
					mem.WriteMemory<float>(csgo, clientbase + clientAngle, smoothed[0]);
					mem.WriteMemory<float>(csgo, clientbase + clientAngle + 0x4, smoothed[1]);
					lasttarget = Ind;*/
					EntityPlace = { BoneMatrix[8][0][3],BoneMatrix[8][1][3] ,BoneMatrix[8][2][3] };
					CalcAngle2(LoalPlayPlace, EntityPlace, Angles);
					NormalizeAngle((D3DVECTOR*)&Angles);
					Angles[0] /= 8.f;
					printf("X:%f--Y:%f-Z:%f\n", Angles[0], Angles[1], Angles[2]);
					CreenXY EntityCreen;
					float Scale;
					if (!WorldToScreen(EntityPlace, ViewMatrix, &EntityCreen, Cl_Width, Cl_Height, &Scale))continue;
					solidcircle((int)(EntityCreen.x), (int)EntityCreen.y, 2);
					line((int)(EntityCreen.x), (int)EntityCreen.y, (int)(EntityCreen.x), (int)EntityCreen.y - Cl_Height/2);
					WriteGameMemory((LPVOID)(EngineStateBase + dwMouseAngles), &Angles, sizeof(Angles));
				}
			}
		}
	}
}
#pragma endregion