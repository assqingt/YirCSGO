#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <graphics.h>
#include <d3d9.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#include "Mapo.h"
#include "Process.h"
#include "Module.h"
#include "Offset.h"
#include "Debug.h"

#define _wsp_sdk_
#define Init_Width 1920
#define Init_Height 1080
#pragma region �˵����
#define CMenu_Width 150
#define CMenu_Height 210
#define CItem_Width 145
#define CItem_Height 20
#define CItem_Left 5
#define CItemText_left 7
#define CItemONF_Left 25
#define MenuItemNum 10
#define Menu_Cheat 0
#define Menu_Other 3
#pragma endregion
#pragma region ���׹������ͺ�
#define CheatType_BoxEsp 1
#define CheatType_LowObject 2
#define CheatType_BoneEsp 2
#define CheatType_Aimbot 3
#define CheatType_AutoPressueGun 4
#define CheatType_Spotted 5
#define CheatType_Crosshair 6
#define CheatType_Team 7
#define CheatType_BhopJump 8
#define CheatType_Other 0
#define MaxPlayerNum 64
#define PLAYER_ON_FLOOR (1 << 0)
#pragma endregion

#pragma region ����λ�ú�
#define Bone_MaxIndex 15
#define Bone_ͷ�� 0
#define Bone_�ز� 1
#define Bone_��� 2
#define Bone_�Ҽ� 3
#define Bone_���� 4
#define Bone_���� 5
#define Bone_���� 6
#define Bone_���� 7
#define Bone_���� 8
#define Bone_���� 9
#define Bone_�β� 10
#define Bone_��ϥ 11
#define Bone_��ϥ 12
#define Bone_��� 13
#define Bone_�ҽ� 14
#pragma region ���������ʶ��
#define CT_BoneIndex_ͷ�� 8
#define CT_BoneIndex_�ز� 6
#define CT_BoneIndex_��� 11
#define CT_BoneIndex_�Ҽ� 40
#define CT_BoneIndex_���� 12
#define CT_BoneIndex_���� 41
#define CT_BoneIndex_���� 13
#define CT_BoneIndex_���� 42
#define CT_BoneIndex_���� 5
#define CT_BoneIndex_���� 4
#define CT_BoneIndex_�β� 3
#define CT_BoneIndex_��ϥ 70
#define CT_BoneIndex_��ϥ 77
#define CT_BoneIndex_��� 71
#define CT_BoneIndex_�ҽ� 78
#pragma endregion
#pragma region ��ͽ������ʶ��
#define T_BoneIndex_ͷ�� 8
#define T_BoneIndex_�ز� 6
#define T_BoneIndex_��� 10
#define T_BoneIndex_�Ҽ� 38
#define T_BoneIndex_���� 11
#define T_BoneIndex_���� 39
#define T_BoneIndex_���� 13
#define T_BoneIndex_���� 42
#define T_BoneIndex_���� 5
#define T_BoneIndex_���� 4
#define T_BoneIndex_�β� 3
#define T_BoneIndex_��ϥ 66
#define T_BoneIndex_��ϥ 73
#define T_BoneIndex_��� 67
#define T_BoneIndex_�ҽ� 74

#pragma endregion
#pragma endregion

#pragma pack(push,1)
typedef struct MenuItem_
{
	LPCWSTR lpItem;
	UINT Type;
	UINT CheatType;
	bool SelectItem;
	bool ONFF;
}MenuItem, *PMenuItem;
typedef unsigned char      uint8_t;
struct GlowBase
{
	float r;
	float g;
	float b;
	float a;
	uint8_t unk1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
};
typedef float matrix_t[3][4];
#pragma pack(pop)

void WINAPI DrawUIThread();
void WINAPI DrawTrajectoryCrosshair(int x, int y);
void WINAPI KeyState();
void WINAPI DrawMenu();
void WINAPI AddMenuItem(LPCWSTR lpItem, UINT Type, UINT CheatType, bool Onff);
void WINAPI SetDefaultColor();
void WINAPI SetCheatType(UINT nCheatType);
void WINAPI DrawBoxEsp(UINT GameWidth, UINT GameHeight);
BOOL WINAPI ReadGameMemory(LPCVOID lpBaseAddress, LPVOID ReadDword, UINT ReadLen);
BOOL WINAPI WriteGameMemory(LPVOID lpBaseAddress, LPCVOID WriteDword, UINT WriteLen);
void WINAPI SetUILocation();
void WINAPI GetCSGORECT(UINT *GameWidth, UINT *GameHeight);
void WINAPI BhopJump();
void WINAPI DrawBoneEsp(UINT GameWidth, UINT GameHeight);
void WINAPI LowObjectEsp();
bool WINAPI EntIsVisible(DWORD ent, DWORD local);
void WINAPI AimbotEntity();
BOOL WINAPI InitPrcoess();
void WINAPI InitMenu();