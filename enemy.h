//=============================================================================
//
// 地面処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_NUM					50
#define ENEMY_REFLECT_POWER			30.0f
#define ENEMY_REFLECT_PLUS_POWER	(1.0f / 30.0f)

#define ENEMY_SPEED_UP_TIME			10		// スピードアップする時間
#define ENEMY_SPEED_UP				0.5f	// スピードアップ量


//#define	ENEMY_VALUE_MOVE		(5.0f)						// 移動量
//#define	ENEMY_VALUE_ROTATE		(D3DX_PI * 0.02f)			// 回転量

struct ENEMY
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き(回転)
	D3DXVECTOR3		scl;		// 大きさ(スケール)
	D3DXVECTOR3		dir;		// 移動方向
	float			spd;		// 移動スピード
	bool			use;		// 表示フラグ

	int				idxShadow;	// 影の識別子

	D3DXVECTOR3     size;		// 当たり判定用サイズ

	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
