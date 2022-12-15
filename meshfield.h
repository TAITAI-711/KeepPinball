//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX_2 (100 + 2)				//�K�v�Ȓ��_�̐� (4�ȏ�)
#define NUM_INDEX_2 (13 + ((NUM_VERTEX_2 - 6) * 3))
#define MESH_WIDTH		3000.0f				// ���a
#define MESH_WIDTH_HALF	(MESH_WIDTH / 2.0f)	// ���a
// ���S���W
#define MESH_POS_X		0.0f
#define MESH_POS_Y		-10.0f
#define MESH_POS_Z		0.0f
// �[�̍���
#define MESH_EDGE_POS_Y	100.0f

// �n�ʂ̍���
#define MESH_GROUND_POS_Y	-250.0f
#define MESH_GROUND_WIDTH	3500.0f

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

