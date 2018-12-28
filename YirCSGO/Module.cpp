#include "Module.h"
bool WINAPI WorldToScreen(D3DVECTOR lpEnemyVec, D3DMATRIX lpViewMatrix, CreenXY *lpCreenXY, UINT GameWidth, UINT GameHeight,float *Scale) {
	lpCreenXY->x = lpEnemyVec.x*lpViewMatrix._11 + lpEnemyVec.y*lpViewMatrix._12 + lpEnemyVec.z*lpViewMatrix._13 + lpViewMatrix._14;
	lpCreenXY->y = lpEnemyVec.x*lpViewMatrix._21 + lpEnemyVec.y*lpViewMatrix._22 + lpEnemyVec.z*lpViewMatrix._23 + lpViewMatrix._24;
	float w = lpEnemyVec.x*lpViewMatrix._41 + lpEnemyVec.y*lpViewMatrix._42 + lpEnemyVec.z*lpViewMatrix._43 + lpViewMatrix._44;
 	if (w < 0.01f)return false;
	float x = GameWidth / 2.f;
	float y = GameHeight / 2.f;
	float invm = 1.0f / w;
	lpCreenXY->x *= invm;
	lpCreenXY->y *= invm;
	x += 0.5f*lpCreenXY->x*GameWidth + 0.5f;
	y -= 0.5f*lpCreenXY->y*GameHeight + 0.5f;
	lpCreenXY->x = x;
	lpCreenXY->y = y;
	*Scale = w / GameHeight;
	return true;
}
float WINAPI FovToScale(UINT iFov,float Scale) {
	if (Scale == 0)Scale = 0.05f;
	switch (iFov)
	{
	case 45:
		Scale /= 2.5f;
		break;
	case 40:
		Scale /= 2.8f;
		break;
	case 15:
		Scale /= 7.8f;
		break;
	case 10:
		Scale /= 11.8f;
		break;
	}
	return Scale;
}
bool WINAPI WorldToScreen2(D3DVECTOR lpEnemyVec, float **lpViewMatrix, CreenXY *lpCreenXY, UINT GameWidth, UINT GameHeight) {
	float rew[2] = { 0 };
	lpEnemyVec.x = lpEnemyVec.x*lpViewMatrix[0][0] + lpEnemyVec.y*lpViewMatrix[0][1] + lpEnemyVec.z*lpViewMatrix[0][2] + lpViewMatrix[0][3];
	lpEnemyVec.y = lpEnemyVec.x*lpViewMatrix[1][0] + lpEnemyVec.y*lpViewMatrix[1][1] + lpEnemyVec.z*lpViewMatrix[1][2] + lpViewMatrix[1][3];
	float w = lpEnemyVec.x*lpViewMatrix[3][0] + lpEnemyVec.y*lpViewMatrix[3][1] + lpEnemyVec.z*lpViewMatrix[3][2] + lpViewMatrix[3][3];
	//float w = lpEnemyVec.x*lpViewMatrix._41 + lpEnemyVec.y*lpViewMatrix._42 + lpEnemyVec.z*lpViewMatrix._43 + lpViewMatrix._44;
	if (w < 0.01f)return false;
	float x = GameWidth / 2.f;
	float y = GameHeight / 2.f;
	float invm = 1.0f / w;
	lpCreenXY->x *= invm;
	lpCreenXY->y *= invm;
	x += 0.5f*lpCreenXY->x*GameWidth + 0.5f;
	y -= 0.5f*lpCreenXY->y*GameHeight + 0.5f;
	lpCreenXY->x = x;
	lpCreenXY->y = y;
	return true;
}
float WINAPI GetD3Distance(D3DVECTOR lpEnemyVec, D3DVECTOR lpLocalVec) {
	return sqrtf(powf(lpLocalVec.x - lpEnemyVec.x, 2.f) + powf(lpLocalVec.y - lpEnemyVec.y, 2.f) + powf(lpLocalVec.z - lpEnemyVec.z, 2.f));
}
float WINAPI GetD2Distance(D3DVECTOR lpEnemyVec, D3DVECTOR lpLocalVec) {
	return sqrtf(powf(lpLocalVec.x - lpEnemyVec.x, 2.f) + powf(lpLocalVec.y - lpEnemyVec.y, 2.f));
}
void WINAPI CalcAngle(D3DVECTOR lpLocalPlace, D3DVECTOR lpEnemyPlace, float *angles)
{
	float Delta[3] = { (lpLocalPlace.x - lpEnemyPlace.x), (lpLocalPlace.y - lpEnemyPlace.y), (lpLocalPlace.z - lpEnemyPlace.z) };
	angles[0] = atanf(Delta[2] / sqrtf(Delta[0] * Delta[0] + Delta[1] * Delta[1])) * M_RADPI;
	angles[1] = atanf(Delta[1] / Delta[0]) * M_RADPI;
	angles[2] = 0.0f;
	if (Delta[0] >= 0.0) angles[1] += 180.0f;
}
void WINAPI Smooth(float x, float y, float *src, float *back, D3DVECTOR flLocalAngles, float smooth, short weapon)
{
	float smoothdiff[2];
	src[0] -= flLocalAngles.x;
	src[1] -= flLocalAngles.y;
	if (src[0] > 180)  src[0] -= 360;
	if (src[1] > 180)  src[1] -= 360;
	if (src[0] < -180) src[0] += 360;
	if (src[1] < -180) src[1] += 360;
	smoothdiff[0] = (src[0] - x) * smooth;
	smoothdiff[1] = (src[1] - y) * smooth;
	back[0] = flLocalAngles.x + smoothdiff[0];
	back[1] = flLocalAngles.y + smoothdiff[1];
	back[2] = flLocalAngles.z;
	if (back[0] > 180)  back[0] -= 360;
	if (back[1] > 180)  back[1] -= 360;
	if (back[0] < -180) back[0] += 360;
	if (back[1] < -180) back[1] += 360;
	if (back[0] > 89.0f) back[0] = 89.0f;
	else if (back[0] < -89.0f) back[0] = -89.0f;
	if (back[1] > 180.0f) back[1] = 180.0f;
	else if (back[1] < -180.0f) back[1] = -180.0f;
	back[2] = 0.f;

}