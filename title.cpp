//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
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
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_TextureNo = 0;	// �e�N�X�`�����
static int	g_Texture_SentakuNo = 0;	// �e�N�X�`�����
static int	g_SE_OK_No = 0;				// SE
static int	g_SE_Sentaku_No = 0;		// SE
static int	g_BGM_Title_No = 0;			// BGM

static int	g_SentakuNo = 0;	// �I�𒆂̔ԍ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	//�e�N�X�`������
	g_TextureNo = LoadTexture("data/TEXTURE/Pinball_Title.png");
	g_Texture_SentakuNo = LoadTexture("data/TEXTURE/Pinball_Sentaku.png");

	g_SE_OK_No = LoadSound("data/SE/ok.wav");
	g_SE_Sentaku_No = LoadSound("data/SE/sentaku.wav");

	g_BGM_Title_No = LoadSound("data/BGM/title.wav");

	g_SentakuNo = 0;

	//255(-1)�����[�v�w�肷��Ɩ������[�v
	PlaySound(g_BGM_Title_No, -1);
	SetVolume(g_BGM_Title_No, 0.35f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	StopSoundAll();
}

//=============================================================================
// �X�V����
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
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
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

