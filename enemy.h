//=============================================================================
//
// �n�ʏ��� [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_NUM					50
#define ENEMY_REFLECT_POWER			30.0f
#define ENEMY_REFLECT_PLUS_POWER	(1.0f / 30.0f)

#define ENEMY_SPEED_UP_TIME			10		// �X�s�[�h�A�b�v���鎞��
#define ENEMY_SPEED_UP				0.5f	// �X�s�[�h�A�b�v��


//#define	ENEMY_VALUE_MOVE		(5.0f)						// �ړ���
//#define	ENEMY_VALUE_ROTATE		(D3DX_PI * 0.02f)			// ��]��

struct ENEMY
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����(��])
	D3DXVECTOR3		scl;		// �傫��(�X�P�[��)
	D3DXVECTOR3		dir;		// �ړ�����
	float			spd;		// �ړ��X�s�[�h
	bool			use;		// �\���t���O

	int				idxShadow;	// �e�̎��ʎq

	D3DXVECTOR3     size;		// �����蔻��p�T�C�Y

	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
