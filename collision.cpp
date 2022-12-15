//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "shadow.h"
#include "wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �����蔻�菈��
//=============================================================================
void UpdateCollision(void)
{
	PLAYER*	pPlayer = GetPlayer();		// �v���C���[�̃|�C���^�[���擾
	ENEMY*	pEnemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[���擾
	WALL*	pWall = GetWall();			// �ǂ̃|�C���^�[���擾

	if (pPlayer[0].use)
	{
		// �o���p�[�Ƒ���L����
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (!pEnemy[i].use)
				continue;

//			if (CollisionBB(pPlayer->pos, pEnemy[i].pos, pPlayer->size, pEnemy[i].size))
			if (CollisionBS(pPlayer->pos, pEnemy[i].pos, pPlayer->size.x / 2, pEnemy[i].size.x / 2))
			{
				// �G�L�����N�^�[������
				/*pEnemy[i].use = false;*/
				// �e�������Ȃ��Ă͂Ȃ炢
				/*ReleaseShadow(pEnemy[i].idxShadow);*/

				SetPlayerReflectEnemy(&pEnemy[i]);

				int Time = GetTime();

				int Score = (Time / 5 + 1) * 10;
				// �X�R�A��ǉ�
				AddScore(Score);
			}
		}

		// �ǂƃv���C���[
		for (int i = 0; i < WALL_NUM; i++)
		{
			if (!pWall[i].use)
				continue;
			
			if (CollisionVS(pWall[i].pos, pPlayer->pos, pPlayer->size.x / 2))
			{
				// ����
				SetPlayerReflectWall(&pWall[i]);

				// �Ǎ폜
				SetWall(i, false);

				// �X�R�A��ǉ�
				AddScore(100);
			}
		}
	}
}

//=============================================================================
// ���E��BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool CollisionBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2)
{
	float Axmax = pos1.x + (size1.x / 2);
	float Axmin = pos1.x - (size1.x / 2);
	float Aymax = pos1.y + (size1.y / 2);
	float Aymin = pos1.y - (size1.y / 2);
	float Azmax = pos1.z + (size1.z / 2);
	float Azmin = pos1.z - (size1.z / 2);

	float Bxmax = pos2.x + (size2.x / 2);
	float Bxmin = pos2.x - (size2.x / 2);
	float Bymax = pos2.y + (size2.y / 2);
	float Bymin = pos2.y - (size2.y / 2);
	float Bzmax = pos2.z + (size2.z / 2);
	float Bzmin = pos2.z - (size2.z / 2);

	if ((Axmax > Bxmin) && (Axmin < Bxmax))
	{
		if ((Aymin < Bymax) && (Aymax > Bymin))
		{
			if ((Azmax > Bzmin) && (Azmin < Bzmax))
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// ���E��BS�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CollisionBS(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	//pos1��pos2���Ȃ��x�N�g�������
	D3DXVECTOR3 dist = pos2 - pos1;

	//������x�N�g���̑傫�����v�Z����
//	float length = D3DXVec3Length(&dist);

	//Sq�����Ă���֐��͕������̌v�Z�����Ȃ��o�[�W����
	float length = D3DXVec3LengthSq(&dist);

	//1��2�̔��a�𑫂�
	float size = size1 + size2;

	//���肷�鑊��̕����悷��
	size = size * size;

	//���a�̕����傫���Ƃ��A��̉~�͏d�Ȃ��Ă���
	if (length < size)
		return true;

	return false;
}


//=============================================================================
// �_�Ƌ��̏Փ�
//=============================================================================
bool CollisionVS(D3DXVECTOR3 PosV1, D3DXVECTOR3 PosC2, float size1)
{
	D3DXVECTOR3 dist = PosV1 - PosC2;

	float length = D3DXVec3Length(&dist);

	if (length <= sqrtf(size1 * size1))
		return true;

	return false;
}
