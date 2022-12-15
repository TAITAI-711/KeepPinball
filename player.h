//=============================================================================
//
// �n�ʏ��� [player.h]
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
// �}�N����`
//*****************************************************************************
#define M_GRAVITY			9.8f	// �d��
#define	M_CENTERGRAVITY		0.17f	// ���S�Ɉ����񂹂���d��
#define	VALUE_MOVE			(0.30f)						// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)			// ��]��

#define CENTER_MAX_SPEED	10.0f	// ���S�ŃX�s�[�h�o�t��������ő��
#define M_CENTER_MOVE_START_TIME	5	// �^�񒆂̃o�t��������n�߂鎞��
#define M_PLAYER_MAX_VEC			35	// �v���C���[�̈ړ��ʍő�



struct PLAYER
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����(��])
	D3DXVECTOR3		scl;		// �傫��(�X�P�[��)
	//float			spd;		// �ړ��X�s�[�h
	bool			use;		// �\���t���O

	int				idxShadow;	// �e�̎��ʎq

	D3DXVECTOR3     size;		// �����蔻��p�T�C�Y

	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
void SetPlayerReflectEnemy(ENEMY* pEnemy);
void SetPlayerReflectWall(WALL* pWall);
int GetTime(void);
