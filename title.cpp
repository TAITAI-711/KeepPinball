//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "fade.h"
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
static int	g_Texture_SentakuNo = 0;	// テクスチャ情報
static int	g_SE_OK_No = 0;				// SE
static int	g_SE_Sentaku_No = 0;		// SE
static int	g_BGM_Title_No = 0;			// BGM

static int	g_SentakuNo = 0;	// 選択中の番号

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	//テクスチャ生成
	g_TextureNo = LoadTexture("data/TEXTURE/Pinball_Title.png");
	g_Texture_SentakuNo = LoadTexture("data/TEXTURE/Pinball_Sentaku.png");

	g_SE_OK_No = LoadSound("data/SE/ok.wav");
	g_SE_Sentaku_No = LoadSound("data/SE/sentaku.wav");

	g_BGM_Title_No = LoadSound("data/BGM/title.wav");

	g_SentakuNo = 0;

	//255(-1)をループ指定すると無限ループ
	PlaySound(g_BGM_Title_No, -1);
	SetVolume(g_BGM_Title_No, 0.35f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	StopSoundAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if ((GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE)) && GetFadeState() == FADE_NONE && g_SentakuNo == 0)
	{
		SceneTransition(SCENE_GAME);
		PlaySound(g_SE_OK_No, 0);
		SetVolume(g_SE_OK_No, 1.5f);
	}
	else if ((GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE)) && GetFadeState() == FADE_NONE && g_SentakuNo == 1)
	{
		SetEndGame();
		PlaySound(g_SE_OK_No, 0);
		SetVolume(g_SE_OK_No, 1.5f);
	}

	if ((GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP)) && g_SentakuNo != 0)
	{
		g_SentakuNo = 0;
		PlaySound(g_SE_Sentaku_No, 0);
		SetVolume(g_SE_Sentaku_No, 1.5f);
	}
	else if ((GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN)) && g_SentakuNo != 1)
	{
		g_SentakuNo = 1;
		PlaySound(g_SE_Sentaku_No, 0);
		SetVolume(g_SE_Sentaku_No, 1.5f);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	// １枚のポリゴンの頂点とテクスチャ座標を設定
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	float posY = 0.0f;

	if (g_SentakuNo == 0)
	{
		posY = 280.0f;
	}
	else
	{
		posY = 385.0f;
	}
	DrawSpriteColor(g_Texture_SentakuNo, SCREEN_WIDTH / 2.0f, posY, 
		557.0f / 2.0f, 71.0f / 2.0f, 
		0.0f, 0.0f, 1.0f, 1.0f, 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

