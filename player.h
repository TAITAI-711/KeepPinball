//=============================================================================
//
// 地面処理 [player.h]
// Author : 
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include "renderer.h"
#include "main.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"
#include "texture.h"
#include "model.h"
#include "light.h"
#include "bullet.h"
#include "shadow.h"
#include "meshfield.h"
#include "game.h"
#include "sound.h"
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define M_GRAVITY			9.8f	// 重力
#define	M_CENTERGRAVITY		0.17f	// 中心に引き寄せられる重力
#define	VALUE_MOVE			(0.30f)						// 移動量
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)			// 回転量

#define CENTER_MAX_SPEED	10.0f	// 中心でスピードバフがかかる最大量
#define M_CENTER_MOVE_START_TIME	5	// 真ん中のバフがかかり始める時間
#define M_PLAYER_MAX_VEC			35	// プレイヤーの移動量最大



struct PLAYER
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き(回転)
	D3DXVECTOR3		scl;		// 大きさ(スケール)
	//float			spd;		// 移動スピード
	bool			use;		// 表示フラグ

	int				idxShadow;	// 影の識別子

	D3DXVECTOR3     size;		// 当たり判定用サイズ

	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
void SetPlayerReflectEnemy(ENEMY* pEnemy);
void SetPlayerReflectWall(WALL* pWall);
int GetTime(void);
