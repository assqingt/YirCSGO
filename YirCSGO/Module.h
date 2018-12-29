#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <math.h>
typedef struct _CreenXY
{
	float x;
	float y;
}CreenXY, *PCreenXY;
#define M_RADPI 57.295779513082f
#define M_PI    3.14159265358979323846   // pi

bool WINAPI WorldToScreen(D3DVECTOR lpEnemyVec, D3DMATRIX lpViewMatrix, CreenXY *lpCreenXY, UINT GameWidth, UINT GameHeight,float *w);
float WINAPI GetD3Distance(D3DVECTOR lpEnemyVec, D3DVECTOR lpLocalVec);
float WINAPI GetD2Distance(D3DVECTOR lpEnemyVec, D3DVECTOR lpLocalVec);
float WINAPI FovToScale(UINT iFov, float Scale);
void WINAPI CalcAngle(D3DVECTOR lpLocalPlace, D3DVECTOR lpEnemyPlace, float *angles);
void WINAPI CalcAngle2(D3DVECTOR lpLocalPlace, D3DVECTOR lpEnemyPlace, float *angles);
void WINAPI NormalizeAngle(D3DVECTOR *angle);
void WINAPI Smooth(float x, float y, float *src, float *back, D3DVECTOR flLocalAngles, float smooth, short weapon);