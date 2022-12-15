/*==============================================================================

   �Q�[������ [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "game.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "meshfield.h"
#include "collision.h"
#include "sound.h"
#include "score.h"
#include "camera.h"
#include "light.h"
#include "shadow.h"
#include "wall.h"

/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �\���̐錾
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
   �O���[�o���ϐ��̒�`
------------------------------------------------------------------------------*/
static int g_BGMNo = 0;
static bool g_ResultFlag = false;


/*------------------------------------------------------------------------------
   �������֐�
------------------------------------------------------------------------------*/
void InitGame(void)
{
	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// �e�̏�����
	InitShadow();

	// �n�ʂ̏�����
	InitMeshField();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �ǂ̏�����
	InitWall();

	// �e�̏�����
	InitBullet();

	// �X�R�A�̏�����
	InitScore();

	SetLightEnable(true);

	g_ResultFlag = false;

	g_BGMNo = LoadSound("data/BGM/game.wav");

	PlaySound(g_BGMNo, -1);
	SetVolume(g_BGMNo, 0.3f);
}

/*------------------------------------------------------------------------------
   �I������������֐�
------------------------------------------------------------------------------*/
void UninitGame()
{
	//�������Ƃ͋t���ɏI���������s��
	UninitScore();
	UninitShadow();
	UninitBullet();
	UninitWall();
	UninitEnemy();
	UninitPlayer();
	UninitMeshField();

	StopSoundAll();
}

/*------------------------------------------------------------------------------
   �X�V����������֐�
------------------------------------------------------------------------------*/
void UpdateGame(void)
{
	// �J�����X�V
	UpdateCamera();

	// �n�ʂ̍X�V����
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// �ǂ̍X�V����
	UpdateWall();

	// �e�̍X�V����
	UpdateBullet();

	// �e�̍X�V����
	UpdateShadow();

	// �X�R�A�̍X�V����
	UpdateScore();

	// �����蔻��̍X�V����
	UpdateCollision();

	//�X�y�[�X�L�[��������Ă��āA�t�F�[�h�������ł͂Ȃ��Ƃ�
	if (g_ResultFlag && GetFadeState() == FADE_NONE)
	{
		SetVolume(g_BGMNo, 0.1f);

		//RESULT�ֈڍs����
		SceneTransition(SCENE_RESULT);
	}
}

/*------------------------------------------------------------------------------
   �`�揈��������֐�
------------------------------------------------------------------------------*/
void DrawGame(void)
{
	// �v���C���[���_
	D3DXVECTOR3 pos = GetPlayer()->pos;
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	SetCameraAT(pos);
	SetCamera();

	// �[�x�e�X�g�L��
	SetDepthEnable(true);

	// ���C�e�B���O�L��
	SetLightEnable(true);

	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �ǂ̍X�V����
	DrawWall();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();//���W��2D�ϊ�

	// 2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	// �X�R�A�̕`�揈��
	DrawScore();
}

void SetResultFlag(void)
{
	g_ResultFlag = true;
	return;
}