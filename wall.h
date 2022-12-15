//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : 
//
//=============================================================================
#pragma once

#include <d3dx9.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WALL_NUM					150

struct WALL
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
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

void SetWall(int WallNo, bool WallUse);
WALL* GetWall(void);