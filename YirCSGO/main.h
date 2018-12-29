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
#pragma region ²Ëµ¥Ïîºê
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
#pragma region ×÷±×¹¦ÄÜÀàÐÍºê
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

#pragma region ¹Ç÷ÀÎ»ÖÃºê
#define Bone_MaxIndex 15
#define Bone_Í·²¿ 0
#define Bone_ÐØ²¿ 1
#define Bone_×ó¼ç 2
#define Bone_ÓÒ¼ç 3
#define Bone_×óÖâ 4
#define Bone_ÓÒÖâ 5
#define Bone_×óÍó 6
#define Bone_ÓÒÍó 7
#define Bone_ÉÏÑü 8
#define Bone_ÏÂÑü 9
#define Bone_ÍÎ²¿ 10
#define Bone_×óÏ¥ 11
#define Bone_ÓÒÏ¥ 12
#define Bone_×ó½Å 13
#define Bone_ÓÒ½Å 14
#pragma region ¾¯²ì¹Ç÷À±êÊ¶ºê
#define CT_BoneIndex_Í·²¿ 8
#define CT_BoneIndex_ÐØ²¿ 6
#define CT_BoneIndex_×ó¼ç 11
#define CT_BoneIndex_ÓÒ¼ç 40
#define CT_BoneIndex_×óÖâ 12
#define CT_BoneIndex_ÓÒÖâ 41
#define CT_BoneIndex_×óÍó 13
#define CT_BoneIndex_ÓÒÍó 42
#define CT_BoneIndex_ÉÏÑü 5
#define CT_BoneIndex_ÏÂÑü 4
#define CT_BoneIndex_ÍÎ²¿ 3
#define CT_BoneIndex_×óÏ¥ 70
#define CT_BoneIndex_ÓÒÏ¥ 77
#define CT_BoneIndex_×ó½Å 71
#define CT_BoneIndex_ÓÒ½Å 78
#pragma endregion
#pragma region ·ËÍ½¹Ç÷À±êÊ¶ºê
#define T_BoneIndex_Í·²¿ 8
#define T_BoneIndex_ÐØ²¿ 6
#define T_BoneIndex_×ó¼ç 10
#define T_BoneIndex_ÓÒ¼ç 38
#define T_BoneIndex_×óÖâ 11
#define T_BoneIndex_ÓÒÖâ 39
#define T_BoneIndex_×óÍó 13
#define T_BoneIndex_ÓÒÍó 42
#define T_BoneIndex_ÉÏÑü 5
#define T_BoneIndex_ÏÂÑü 4
#define T_BoneIndex_ÍÎ²¿ 3
#define T_BoneIndex_×óÏ¥ 66
#define T_BoneIndex_ÓÒÏ¥ 73
#define T_BoneIndex_×ó½Å 67
#define T_BoneIndex_ÓÒ½Å 74

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