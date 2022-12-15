//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : 
//
//=============================================================================
#include "wall.h"
#include "model.h"
#include "meshfield.h"
#include <corecrt_math_defines.h>


//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static DX11_MODEL	g_Model;			// ���f���ǂݍ���
static WALL			g_Wall[WALL_NUM];	// �Ǐ��


//=============================================================================
// ����������
//=============================================================================
HRESULT InitWall(void)
{
	// �ǂ̊p�x
	float WallRot = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < WALL_NUM; i++)
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float Rate = 0.0f;

		float CenterPosX = 0.0f;
		float CenterPosZ = 0.0f;

		CenterPosX = MESH_POS_X;
		CenterPosZ = MESH_POS_Z;

		Rate = (float)i / WALL_NUM; // ����

		Pos.x = MESH_WIDTH_HALF * cosf(2.0f * (float)M_PI * Rate);
		Pos.z = MESH_WIDTH_HALF * sinf(2.0f * (float)M_PI * Rate);

		float BaseAngle = atan2f(Pos.z, Pos.x);		// ���S�_���璸�_�ɑ΂���p�x
		D3DXVECTOR2 temp = D3DXVECTOR2(Pos.x, Pos.z);
		float Radius = D3DXVec2Length(&temp);		// ���S�_���璸�_�ɑ΂��鋗��

		Pos.x = MESH_POS_X + cosf(BaseAngle) * Radius;
		Pos.y = MESH_EDGE_POS_Y;
		Pos.z = MESH_POS_Z + sinf(BaseAngle) * Radius;


		g_Wall[i].pos = D3DXVECTOR3(Pos.x, Pos.y, Pos.z);
		g_Wall[i].rot = D3DXVECTOR3(0.0f, (1.0f - Rate) * (D3DX_PI * 2) + (D3DX_PI * 0.5f), 0.0f);
		g_Wall[i].scl = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
		g_Wall[i].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[i].spd = 0.0f;
		g_Wall[i].use = true;

		g_Wall[i].size = g_Wall[i].scl * 2;

		//�P�ʃx�N�g��������
		D3DXVec3Normalize(&g_Wall[i].dir, &g_Wall[i].dir);

		//D3DXVECTOR3 sPos = g_Wall[i].pos;
		//sPos.y = 0.0f;
		//// �e
		//g_Enemy[i].idxShadow = CreateShadow(sPos, 100.0f, 100.0f);
	}

	//obj���f���̓ǂݍ���
	LoadModel((char*)"data/MODEL/pinball_wall.obj", &g_Model);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWall(void)
{
	//obj���f���̉��
	UnloadModel(&g_Model);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWall(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	for (int i = 0; i < WALL_NUM; i++)
	{
		//use�t���O���I�t�̏ꍇ�̓X�L�b�v����
		if (g_Wall[i].use == false)
			continue;

		// ���[���h�}�g���b�N�X�̏������i�P�ʍs������֐��j
		D3DXMatrixIdentity(&g_Wall[i].mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Wall[i].scl.x, g_Wall[i].scl.y, g_Wall[i].scl.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[i].rot.y, g_Wall[i].rot.x, g_Wall[i].rot.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Wall[i].pos.x, g_Wall[i].pos.y, g_Wall[i].pos.z);
		D3DXMatrixMultiply(&g_Wall[i].mtxWorld, &g_Wall[i].mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Wall[i].mtxWorld);

		//obj���f���̕`��
		DrawModel(&g_Model);
	}
}

//=============================================================================
// �ǃZ�b�g
//=============================================================================
void SetWall(int WallNo, bool WallUse)
{
	g_Wall[WallNo].use = WallUse;

	return;
}

//=============================================================================
// �Ǐ����擾
//=============================================================================
WALL* GetWall(void)
{
	return g_Wall;
}
