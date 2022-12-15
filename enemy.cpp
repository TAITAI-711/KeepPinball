//=============================================================================
//
// バンパーの表示処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"
#include "texture.h"
#include "model.h"
#include "light.h"
#include "player.h"
#include "shadow.h"
#include "meshfield.h"

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
static ENEMY		g_Enemy[ENEMY_NUM];	// エネミー情報

static bool			g_SpeedChangeFlag = false;	// バンパーのスピード変化フラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	g_SpeedChangeFlag = false;

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int x_Fugou = 0;
		int z_Fugou = 0;

		if (rand() % 2 == 0)
			x_Fugou = 1;
		else
			x_Fugou = -1;

		if (rand() % 2 == 0)
			z_Fugou = 1;
		else
			z_Fugou = -1;


		x = (float)(rand() % 500 + 100) * x_Fugou;
		y = 30.0f;
		z = (float)(rand() % 500 + 100) * z_Fugou;

		g_Enemy[i].pos = D3DXVECTOR3(x, y, z);
		g_Enemy[i].rot = D3DXVECTOR3(0.0f, frand() * D3DX_PI, 0.0f);
		g_Enemy[i].scl = D3DXVECTOR3(4.0f, 5.0f, 4.0f);
		g_Enemy[i].dir = D3DXVECTOR3(frand() - 0.5f, 0.0f, frand() - 0.5f);
		g_Enemy[i].spd = 3.0f;
		g_Enemy[i].use = true;

		g_Enemy[i].size = D3DXVECTOR3(60.0f, 120.0f, 60.0f);

		//単位ベクトル化する
		D3DXVec3Normalize(&g_Enemy[i].dir, &g_Enemy[i].dir);

		D3DXVECTOR3 sPos = g_Enemy[i].pos;
		sPos.y = 0.0f;
		// 影
		//g_Enemy[i].idxShadow = CreateShadow(sPos, 100.0f, 100.0f);
	}

	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/pinball_Bumper.obj", &g_Model);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	//objモデルの解放
	UnloadModel(&g_Model);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//useフラグがオフの場合はスキップする
		if (g_Enemy[i].use == false)
			continue;

		//座標の更新
		float XZVec = 0.0f;
		float XVec = 0.0f;
		float ZVec = 0.0f;

		XVec = (float)fabs(g_Enemy[i].pos.x);
		ZVec = (float)fabs(g_Enemy[i].pos.z);

		XZVec = sqrtf(XVec * XVec + ZVec * ZVec) / (MESH_WIDTH / 2.0f);

		// Y座標更新
		g_Enemy[i].pos.y = XZVec * MESH_EDGE_POS_Y - 30.0f;
		g_Enemy[i].pos += g_Enemy[i].dir * g_Enemy[i].spd;

		//回転の更新
		g_Enemy[i].rot.y += 0.05f;

		// 反射
		if (fabs(g_Enemy[i].pos.x) > 1000.0f)
			g_Enemy[i].dir.x *= -1;

		if (fabs(g_Enemy[i].pos.z) > 1000.0f)
			g_Enemy[i].dir.z *= -1;

		//影の位置の更新
		/*D3DXVECTOR3 sPos = g_Enemy[i].pos;
		sPos.y = 0.0f;
		SetPositionShadow(g_Enemy[i].idxShadow, sPos);*/

		int Time = GetTime();

		// バンパースピードアップ
		if (Time % ENEMY_SPEED_UP_TIME == 0 && Time > 0)
		{
			if (!g_SpeedChangeFlag)
			{
				g_Enemy[i].spd += ENEMY_SPEED_UP;
				g_SpeedChangeFlag = true;
			}
		}
		else
		{
			if (g_SpeedChangeFlag)
				g_SpeedChangeFlag = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//useフラグがオフの場合はスキップする
		if (g_Enemy[i].use == false)
			continue;

		// ワールドマトリックスの初期化（単位行列を作る関数）
		D3DXMatrixIdentity(&g_Enemy[i].mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rot.y, g_Enemy[i].rot.x, g_Enemy[i].rot.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorld, &g_Enemy[i].mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&g_Enemy[i].mtxWorld);

		//objモデルの描画
		DrawModel(&g_Model);
	}
}


//=============================================================================
// エネミー情報を取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return g_Enemy;
}

