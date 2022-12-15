//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : 
//
//=============================================================================
#include "wall.h"
#include "model.h"
#include "meshfield.h"
#include <corecrt_math_defines.h>


//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DX11_MODEL	g_Model;			// モデル読み込み
static WALL			g_Wall[WALL_NUM];	// 壁情報


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWall(void)
{
	// 壁の角度
	float WallRot = 0;

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < WALL_NUM; i++)
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float Rate = 0.0f;

		float CenterPosX = 0.0f;
		float CenterPosZ = 0.0f;

		CenterPosX = MESH_POS_X;
		CenterPosZ = MESH_POS_Z;

		Rate = (float)i / WALL_NUM; // 割合

		Pos.x = MESH_WIDTH_HALF * cosf(2.0f * (float)M_PI * Rate);
		Pos.z = MESH_WIDTH_HALF * sinf(2.0f * (float)M_PI * Rate);

		float BaseAngle = atan2f(Pos.z, Pos.x);		// 中心点から頂点に対する角度
		D3DXVECTOR2 temp = D3DXVECTOR2(Pos.x, Pos.z);
		float Radius = D3DXVec2Length(&temp);		// 中心点から頂点に対する距離

		Pos.x = MESH_POS_X + cosf(BaseAngle) * Radius;
		Pos.y = MESH_EDGE_POS_Y;
		Pos.z = MESH_POS_Z + sinf(BaseAngle) * Radius;


		g_Wall[i].pos = D3DXVECTOR3(Pos.x, Pos.y, Pos.z);
		g_Wall[i].rot = D3DXVECTOR3(0.0f, (1.0f - Rate) * (D3DX_PI * 2) + (D3DX_PI * 0.5f), 0.0f);
		g_Wall[i].scl = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
		g_Wall[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[i].spd = 0.0f;
		g_Wall[i].use = true;

		g_Wall[i].size = g_Wall[i].scl * 2;

		//単位ベクトル化する
		D3DXVec3Normalize(&g_Wall[i].dir, &g_Wall[i].dir);

		//D3DXVECTOR3 sPos = g_Wall[i].pos;
		//sPos.y = 0.0f;
		//// 影
		//g_Enemy[i].idxShadow = CreateShadow(sPos, 100.0f, 100.0f);
	}

	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/pinball_wall.obj", &g_Model);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	//objモデルの解放
	UnloadModel(&g_Model);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	for (int i = 0; i < WALL_NUM; i++)
	{
		//useフラグがオフの場合はスキップする
		if (g_Wall[i].use == false)
			continue;

		// ワールドマトリックスの初期化（単位行列を作る関数）
		D3DXMatrixIdentity(&g_Wall[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Wall[i].scl.x, g_Wall[i].scl.y, g_Wall[i].scl.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[i].rot.y, g_Wall[i].rot.x, g_Wall[i].rot.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Wall[i].pos.x, g_Wall[i].pos.y, g_Wall[i].pos.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&g_Wall[i].mtxWorld);

		//objモデルの描画
		DrawModel(&g_Model);
	}
}

//=============================================================================
// 壁セット
//=============================================================================
void SetWall(int WallNo, bool WallUse)
{
	g_Wall[WallNo].use = WallUse;

	return;
}

//=============================================================================
// 壁情報を取得
//=============================================================================
WALL* GetWall(void)
{
	return g_Wall;
}
