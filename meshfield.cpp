//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "texture.h"
#include "meshfield.h"
#include "renderer.h"
#include "corecrt_math_defines.h"
#include "player.h"
#include "sound.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct FIELD
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXMATRIX  mtxWorld;
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer* g_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@

static ID3D11Buffer* g_VertexBuffer_2 = NULL;	// ���_���
static ID3D11Buffer* g_IndexBuffer_2 = NULL;	// �C���f�b�N�X�o�b�t�@

static ID3D11Buffer* g_VertexBuffer_3 = NULL;	// ���_���
static ID3D11Buffer* g_IndexBuffer_3 = NULL;	// �C���f�b�N�X�o�b�t�@

static FIELD	g_Field;	// �t�B�[���h���
static int		g_Stage_TexNo;	// �e�N�X�`���ԍ�
static int		g_StageCenter_TexNo;	// �e�N�X�`���ԍ�
static int		g_Ground_TexNo;	// �e�N�X�`���ԍ�

static int numVertex = 4;
static int numIndex = 4;

static int g_ColorChangeCnt = 0;

static int g_SE_ColorChange_No = 0;
static bool g_SE_Flag = false;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(void)
{
	g_ColorChangeCnt = 0;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Field.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Field.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`������
	g_StageCenter_TexNo = LoadTexture("data/TEXTURE/Pinball_StageCenter.png");
	g_Stage_TexNo = LoadTexture("data/TEXTURE/Pinball_Stage.png");
	g_Ground_TexNo = LoadTexture("data/TEXTURE/fade_white.png");

	g_SE_ColorChange_No = LoadSound("data/SE/colorchange.wav");

	g_SE_Flag = false;


	int yoko = 1;
	int tate = 1;

	//�K�v�Ȓ��_�����v�Z����
	numVertex = (yoko + 1) * (tate + 1);
	numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);

	
	// �X�e�[�W
	ID3D11Device* pDevice = GetDevice();

	// ���_�o�b�t�@����2
	D3D11_BUFFER_DESC bd_2;
	ZeroMemory(&bd_2, sizeof(bd_2));
	bd_2.Usage = D3D11_USAGE_DYNAMIC;
	bd_2.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX_2;
	bd_2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd_2, NULL, &g_VertexBuffer_2);

	// �C���f�b�N�X�o�b�t�@����2
	ZeroMemory(&bd_2, sizeof(bd_2));
	bd_2.Usage = D3D11_USAGE_DYNAMIC;
	bd_2.ByteWidth = sizeof(unsigned short) * NUM_INDEX_2;
	bd_2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd_2, NULL, &g_IndexBuffer_2);

	// �n��
	ID3D11Device* pDevice3 = GetDevice();

	// ���_�o�b�t�@����3
	D3D11_BUFFER_DESC bd_3;
	ZeroMemory(&bd_3, sizeof(bd_3));
	bd_3.Usage = D3D11_USAGE_DYNAMIC;
	bd_3.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd_3.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice3->CreateBuffer(&bd_3, NULL, &g_VertexBuffer_3);

	// �C���f�b�N�X�o�b�t�@����3
	ZeroMemory(&bd_3, sizeof(bd_3));
	bd_3.Usage = D3D11_USAGE_DYNAMIC;
	bd_3.ByteWidth = sizeof(unsigned short) * numIndex;
	bd_3.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice3->CreateBuffer(&bd_3, NULL, &g_IndexBuffer_3);


	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		

		// ���_���W�̐ݒ�
		pVtx[0].Position = D3DXVECTOR3(-200.0f, 0.0f, -200.0f);
		pVtx[1].Position = D3DXVECTOR3(-200.0f, 0.0f, 200.0f);
		pVtx[2].Position = D3DXVECTOR3(200.0f, 0.0f, -200.0f);
		pVtx[3].Position = D3DXVECTOR3(200.0f, 0.0f, 200.0f);
		// UV�l�̐ݒ�
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		// �@���̐ݒ�
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�J���[�̐ݒ�
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		pIdx[0] = 0;
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 3;
				
 		GetDeviceContext()->Unmap(g_IndexBuffer, 0);
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer_2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float Hankei;
		Hankei = MESH_WIDTH / 2.0f;

		vertex[0].Position = D3DXVECTOR3(MESH_POS_X, MESH_POS_Y, MESH_POS_Z);
		vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.5f, 0.5f);

		float rot = ((float)M_PI / 180.0f);
		//float KesuVertex = ((1.0f - ArufaWariai) * (NUM_VERTEX_2 - 2));

		for (int i = 1; i < NUM_VERTEX_2; i++)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float Rate = 0.0f;

			float u = 0.0f;
			float v = 0.0f;

			Rate = (float)(i - 1) / (NUM_VERTEX_2 - 2); // ����

			x = Hankei * cosf(2.0f * (float)M_PI * Rate);
			z = Hankei * sinf(2.0f * (float)M_PI * Rate);

			float BaseAngle = atan2f(z, x);		// ���S�_���璸�_�ɑ΂���p�x
			D3DXVECTOR2 temp = D3DXVECTOR2(x, z);
			float Radius = D3DXVec2Length(&temp);	// ���S�_���璸�_�ɑ΂��鋗��

			x = MESH_POS_X + cosf(BaseAngle + rot) * Radius;
			z = MESH_POS_Z + sinf(BaseAngle + rot) * Radius;


			u = 0.5f * cosf(2.0f * (float)M_PI * Rate) + 0.5f;
			v = 0.5f * sinf(2.0f * (float)M_PI * Rate) + 0.5f;

			/*if (i <= KesuVertex)
			{
				x = MESH_POS_X;
				y = MESH_POS_Y;
			}*/

			y = MESH_EDGE_POS_Y;

			vertex[i].Position = D3DXVECTOR3(x, y, z);
			vertex[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			vertex[i].TexCoord = D3DXVECTOR2(u, v);
		}

		GetDeviceContext()->Unmap(g_VertexBuffer_2, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer_2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx = (unsigned short*)msr.pData;

		for (int i = 0, j = 0; i < NUM_INDEX_2; i++)
		{
			pIdx[i] = j;

			if (i >= 3)
			{
				switch ((i - 2) % 3)
				{
				case 0:
					j++;
					break;
				case 1:
					pIdx[i] = 0;
					break;
				case 2:
					pIdx[i] = j - 1;
					break;
				default:
					break;
				}
			}
			else
			{
				j++;
			}
		}

		GetDeviceContext()->Unmap(g_IndexBuffer_2, 0);
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer_3, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx_3 = (VERTEX_3D*)msr.pData;


		// ���_���W�̐ݒ�
		pVtx_3[0].Position = D3DXVECTOR3(-MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, -MESH_GROUND_WIDTH);
		pVtx_3[1].Position = D3DXVECTOR3(-MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, MESH_GROUND_WIDTH);
		pVtx_3[2].Position = D3DXVECTOR3(MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, -MESH_GROUND_WIDTH);
		pVtx_3[3].Position = D3DXVECTOR3(MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, MESH_GROUND_WIDTH);
		// UV�l�̐ݒ�
		pVtx_3[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx_3[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx_3[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx_3[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		// �@���̐ݒ�
		pVtx_3[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�J���[�̐ݒ�
		pVtx_3[0].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[1].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[2].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[3].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer_3, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer_3, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short* pIdx_3 = (unsigned short*)msr.pData;

		pIdx_3[0] = 0;
		pIdx_3[1] = 1;
		pIdx_3[2] = 2;
		pIdx_3[3] = 3;

		GetDeviceContext()->Unmap(g_IndexBuffer_3, 0);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer)
		g_IndexBuffer->Release();

	//���_�o�b�t�@�̉��
	if (g_VertexBuffer)
		g_VertexBuffer->Release();


	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer_2)
		g_IndexBuffer_2->Release();

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer_2)
		g_VertexBuffer_2->Release();

	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer_3)
		g_IndexBuffer_3->Release();

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer_3)
		g_VertexBuffer_3->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
	// �t�B�[���h�̐F�ύX
	if (GetTime() % 10 == 0 && GetTime() > 0)
	{
		if (g_ColorChangeCnt == 0)
		{
			//{//���_�o�b�t�@�̒��g�𖄂߂�
			//	D3D11_MAPPED_SUBRESOURCE msr;
			//	ID3D11Buffer* VertexBuffer_2 = g_VertexBuffer_2;

			//	GetDeviceContext()->Map(g_VertexBuffer_2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			//	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

			//	D3DXCOLOR Color = D3DXCOLOR((rand() % 101) * 0.01f, (rand() % 101) * 0.01f, (rand() % 101) * 0.01f, 1.0f);

			//	for (int i = 0; i < NUM_VERTEX_2; i++)
			//	{
			//		vertex[i].Diffuse = Color;
			//	}

			//	GetDeviceContext()->Unmap(g_VertexBuffer_2, 0);
			//}
			{//���_�o�b�t�@�̒��g�𖄂߂�
				D3D11_MAPPED_SUBRESOURCE msr;
				GetDeviceContext()->Map(g_VertexBuffer_2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

				VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

				D3DXCOLOR Color = D3DXCOLOR((rand() % 101) * 0.01f, (rand() % 101) * 0.01f, (rand() % 101) * 0.01f, 1.0f); // �F�ω�

				float Hankei;
				Hankei = MESH_WIDTH / 2.0f;

				vertex[0].Position = D3DXVECTOR3(MESH_POS_X, MESH_POS_Y, MESH_POS_Z);
				vertex[0].Diffuse = Color;
				vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				vertex[0].TexCoord = D3DXVECTOR2(0.5f, 0.5f);

				float rot = ((float)M_PI / 180.0f);
				//float KesuVertex = ((1.0f - ArufaWariai) * (NUM_VERTEX_2 - 2));

				for (int i = 1; i < NUM_VERTEX_2; i++)
				{
					float x = 0.0f;
					float y = 0.0f;
					float z = 0.0f;
					float Rate = 0.0f;

					float u = 0.0f;
					float v = 0.0f;

					Rate = (float)(i - 1) / (NUM_VERTEX_2 - 2); // ����

					x = Hankei * cosf(2.0f * (float)M_PI * Rate);
					z = Hankei * sinf(2.0f * (float)M_PI * Rate);

					float BaseAngle = atan2f(z, x);		// ���S�_���璸�_�ɑ΂���p�x
					D3DXVECTOR2 temp = D3DXVECTOR2(x, z);
					float Radius = D3DXVec2Length(&temp);	// ���S�_���璸�_�ɑ΂��鋗��

					x = MESH_POS_X + cosf(BaseAngle + rot) * Radius;
					z = MESH_POS_Z + sinf(BaseAngle + rot) * Radius;


					u = 0.5f * cosf(2.0f * (float)M_PI * Rate) + 0.5f;
					v = 0.5f * sinf(2.0f * (float)M_PI * Rate) + 0.5f;

					/*if (i <= KesuVertex)
					{
						x = MESH_POS_X;
						y = MESH_POS_Y;
					}*/

					y = MESH_EDGE_POS_Y;

					vertex[i].Position = D3DXVECTOR3(x, y, z);
					vertex[i].Diffuse = Color;
					vertex[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					vertex[i].TexCoord = D3DXVECTOR2(u, v);
				}

				GetDeviceContext()->Unmap(g_VertexBuffer_2, 0);
			}
			g_ColorChangeCnt++;
		}
		else
		{
			g_ColorChangeCnt++;
		}

		if (!g_SE_Flag)
		{
			PlaySound(g_SE_ColorChange_No, 0);
			SetVolume(g_SE_ColorChange_No, 0.3f);

			AddScore(100 * GetTime());
			g_SE_Flag = true;
		}		
	}
	else
	{
		if (g_SE_Flag)
		{
			g_SE_Flag = false;
			StopSound(g_SE_ColorChange_No);
		}
	}
	
	if (g_ColorChangeCnt >= 10)
	{
		g_ColorChangeCnt = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	for (int i = 0; i < 3; i++)
	{
		SetCullingMode(CULL_MODE_BACK);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;

		ID3D11Buffer* VertexBuffer = NULL;	// ���_�o�b�t�@
		ID3D11Buffer* IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@

		if (i == 0)
		{
			VertexBuffer = g_VertexBuffer;
			IndexBuffer = g_IndexBuffer;
		}
		else if (i == 1)
		{
			VertexBuffer = g_VertexBuffer_2;
			IndexBuffer = g_IndexBuffer_2;
		}
		else
		{
			VertexBuffer = g_VertexBuffer_3;
			IndexBuffer = g_IndexBuffer_3;
		}

		GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// �v���~�e�B�u�g�|���W�ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �}�e���A���ݒ�
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		int TexNo = 0;

		if (i == 0)
		{
			TexNo = g_StageCenter_TexNo;
		}
		else if (i == 1)
		{
			TexNo = g_Stage_TexNo;
		}
		else
		{
			TexNo = g_Ground_TexNo;
		}

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(TexNo));


		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Field.mtxWorld);

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&g_Field.mtxWorld);

		int Index = 0;

		if (i == 0)
		{
			Index = numIndex;
		}
		else if (i == 1)
		{
			Index = NUM_INDEX_2;
		}
		else
		{
			Index = numIndex;
		}

		// �|���S���̕`��
		GetDeviceContext()->DrawIndexed(Index, 0, 0);
	}
}

