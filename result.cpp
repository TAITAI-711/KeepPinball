//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : 
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "fade.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_TextureNo = 0;	// テクスチャ情報
static int	g_Score = 0;		// スコア情報
static int	g_SE_OK_No = 0;		// SE
static int	g_BGM_Result_No = 0;// BGM


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	g_TextureNo = LoadTexture("data/TEXTURE/Pinball_Result.png");
	g_SE_OK_No = LoadSound("data/SE/ok.wav");
	g_BGM_Result_No = LoadSound("data/BGM/result.wav");

	PlaySound(g_BGM_Result_No, -1);
	SetVolume(g_BGM_Result_No, 0.4f);

	g_Score = GetScore();

	InitScore();
	AddScore(g_Score);
	SetScorePos(D3DXVECTOR3(SCREEN_CENTER_X + 82.0f, 310.0f, 0.0f));
	SetScoreColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	UninitScore();

	StopSoundAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	UpdateScore();

	if ((GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE)) && GetFadeState() == FADE_NONE)
	{
		SceneTransition(SCENE_TITLE);
		PlaySound(g_SE_OK_No, 0);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	DrawScore();
}
