//=============================================================================
//
// 壁処理 [wall.h]
// Author : 
//
//=============================================================================
#pragma once

#include <d3dx9.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_NUM					150

struct WALL
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
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

void SetWall(int WallNo, bool WallUse);
WALL* GetWall(void);