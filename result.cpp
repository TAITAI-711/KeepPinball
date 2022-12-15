//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
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
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_TextureNo = 0;	// �e�N�X�`�����
static int	g_Score = 0;		// �X�R�A���
static int	g_SE_OK_No = 0;		// SE
static int	g_BGM_Result_No = 0;// BGM


//=============================================================================
// ����������
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
// �I������
//=============================================================================
void UninitResult(void)
{
	UninitScore();

	StopSoundAll();
}

//=============================================================================
// �X�V����
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
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	DrawScore();
}
