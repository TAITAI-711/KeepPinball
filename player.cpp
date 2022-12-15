//=============================================================================
//
// �|���S���\������ [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static DX11_MODEL	g_Model;		// ���f���ǂݍ���
static PLAYER		g_Player;		// �v���C���[���		
static float        g_Rot;			// �L�����N�^�[�̌���
static D3DXVECTOR3	g_PlayerVec;	// �v���C���[�ɂ�����x�N�g��
static int			g_SE_Bumper;	// �o���p�[SE
static int			g_SE_Wall_1;	// ��SE1
static int			g_SE_Wall_2;	// ��SE2

static int			g_Time;
static int			g_Timecnt;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.scl = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	g_Player.use = true;

	//�����蔻��p�T�C�Y�̏����ݒ�
	g_Player.size = D3DXVECTOR3(90.0f, 90.0f, 90.0f);

	//obj���f���̓ǂݍ���
	LoadModel((char*)"data/MODEL/pinball_ball.obj", &g_Model);

	g_SE_Bumper = LoadSound("data/SE/Bumper.wav");
	g_SE_Wall_1 = LoadSound("data/SE/Wall_1.wav");
	g_SE_Wall_2 = LoadSound("data/SE/Wall_2.wav");

	//�e�̐ݒ�
	D3DXVECTOR3 sPos = g_Player.pos;
	sPos.y = 0.0f;
	g_Player.idxShadow = CreateShadow(sPos, 50.0f, 50.0f);

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	LIGHT *pLight = NULL;
	pLight = GetLightData(0);
	pLight->Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);// ���̌���
	pLight->Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);	// ���̐F
	pLight->Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	pLight->Enable = true;									// ���̃��C�g��ON
	SetLightData(0, pLight);								// ����Őݒ肵�Ă���

	// �_�����̐ݒ�
	pLight = GetLightData(1);
	pLight->Position = D3DXVECTOR4(0.0f,500.0f, 0.0f, 0.0f);
	pLight->Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);// ���̌���
	pLight->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
	pLight->Attenuation = 1000.0f;							// ��������
	pLight->Type = LIGHT_TYPE_POINT;						// ���s����
	pLight->Enable = true;									// ���̃��C�g��ON
	SetLightData(1, pLight);								// ����Őݒ肵�Ă���

	g_PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Time = 0;
	g_Timecnt = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//obj���f���̉��
	UnloadModel(&g_Model);
	StopSoundAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	//�ړ����x�̌�������
	float Speed = 0;

	if (GetKeyboardPress(DIK_A))
	{
		Speed = VALUE_MOVE;
		g_Rot = -D3DX_PI / 2;
	}
	if (GetKeyboardPress(DIK_D))
	{
		Speed = VALUE_MOVE;
		g_Rot = D3DX_PI / 2;
	}
	if (GetKeyboardPress(DIK_W))
	{
		Speed = VALUE_MOVE;
		g_Rot = 0.0f;
	}
	if (GetKeyboardPress(DIK_S))
	{
		Speed = VALUE_MOVE;
		g_Rot = D3DX_PI;
	}

	// Y���W�X�V
	float XZVec = 0.0f;
	float XVec = 0.0f;
	float ZVec = 0.0f;

	XVec = (float)fabs(g_Player.pos.x);
	ZVec = (float)fabs(g_Player.pos.z);

	XZVec = sqrtf(XVec * XVec + ZVec * ZVec) / (MESH_WIDTH / 2.0f); // �^�񒆂���[�܂ł̂ǂ��̈ʒu�ɂ��邩

	// ���S�Ɍ������ďd��
	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f) - g_Player.pos;

	float Length = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);

	float Normal_X = Vec.x / Length;
	float Normal_Y = Vec.y / Length;
	float Normal_Z = Vec.z / Length;

	Vec = D3DXVECTOR3(Normal_X * M_CENTERGRAVITY, Normal_Y * 0.0f, Normal_Z * M_CENTERGRAVITY);

	// �v���C���[�̈ړ���
	float PlayerVecLength = sqrtf(g_PlayerVec.x * g_PlayerVec.x + g_PlayerVec.z * g_PlayerVec.z);
	float PlayerVecNomalX = 0.0f;
	float PlayerVecNomalZ = 0.0f;

	if (PlayerVecLength > 0.0f)
	{
		PlayerVecNomalX = g_PlayerVec.x / PlayerVecLength;
		PlayerVecNomalZ = g_PlayerVec.z / PlayerVecLength;
	}
	
	// �^�񒆂̃X�s�[�h�A�b�v
	if (XZVec < 0.085f && 
		g_Time > M_CENTER_MOVE_START_TIME &&
		PlayerVecLength < CENTER_MAX_SPEED)
	{
		// ���S�Ɍ������ė͂�������
		/*float VecX_2 = 0.0f;
		float VecZ_2 = 0.0f;
		float Length_2 = 0.0f;
		float Normal_X_2 = 0.0f;
		float Normal_Z_2 = 0.0f;

		VecX_2 = -g_Player.pos.x;
		VecZ_2 = -g_Player.pos.z;

		Length_2 = sqrtf(VecX_2 * VecX_2 + VecZ_2 * VecZ_2);

		Normal_X_2 = VecX_2 / Length_2;
		Normal_Z_2 = VecZ_2 / Length_2;

		Normal_X_2 = Normal_X_2 * 7.0f;
		Normal_Z_2 = Normal_Z_2 * 7.0f;
		g_PlayerVec += D3DXVECTOR3(Normal_X_2, 0.0f, Normal_Z_2);*/

		g_PlayerVec += D3DXVECTOR3(g_PlayerVec.x * 0.5f, 0.0f, g_PlayerVec.z * 0.5f);
	}


	{	// �����������Ƀv���C���[���ړ�������
		// �����������Ƀv���C���[���������Ă��鏊
		g_Player.rot.y = g_Rot + cam->rot.y;

		/*g_Player.pos.x += sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z += cosf(g_Player.rot.y) * g_Player.spd;*/
	}

	if (XZVec <= 1.0f)
	{
		// ���S�Ɍ������ďd��
		g_PlayerVec += Vec;
		// Y���W�X�V
		g_Player.pos.y = XZVec * MESH_EDGE_POS_Y + 10.0f;
		// �����������Ƀv���C���[���ړ�������
		g_PlayerVec += D3DXVECTOR3(sinf(g_Player.rot.y) * Speed, 0.0f, cosf(g_Player.rot.y) * Speed);
	}
	else
	{
		g_PlayerVec.y -= M_GRAVITY * 0.01f;
	}

	if (PlayerVecLength > M_PLAYER_MAX_VEC)
	{
		g_PlayerVec.x = PlayerVecNomalX * M_PLAYER_MAX_VEC;
		g_PlayerVec.z = PlayerVecNomalZ * M_PLAYER_MAX_VEC;
	}
	else
	{
		g_PlayerVec *= 0.997f;
	}

	g_Player.pos += g_PlayerVec;

	// �e������
	/*if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(g_Player.pos);
	}*/

	//�e�̈ʒu�̍X�V
	D3DXVECTOR3 sPos = g_Player.pos;
	//sPos.y = 0.0f;
	SetPositionShadow(g_Player.idxShadow, sPos);

#ifdef _DEBUG	// �f�o�b�O����\������
	char* str = GetDebugStr();
	printf(&str[strlen(str)], " rotX:%.2f rotY:%.2f", g_Player.rot.x, g_Player.rot.y);

#endif
	
	// ����
	g_Timecnt++;

	if (g_Timecnt >= 60)
	{
		g_Time++;
		g_Timecnt = 0;
	}


	if (g_Player.pos.y <= MESH_GROUND_POS_Y + 10.0f)
	{
		g_Player.pos.y = MESH_GROUND_POS_Y + 10.0f;
		SetResultFlag();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏������i�P�ʍs������֐��j
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);

	                   //��1���� ��          ��2����      ��    ��3����
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&g_Player.mtxWorld);

	//�J�����O�Ȃ�
	SetCullingMode(CULL_MODE_NONE);

	//obj���f���̕`��
	DrawModel(&g_Model);

	//�J�����O�����ɖ߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �v���C���[�̔���(�o���p�[)
//=============================================================================
void SetPlayerReflectEnemy(ENEMY* pEnemy)
{
	float VecX = 0.0f;
	float VecZ = 0.0f;
	float Length = 0.0f;
	float Normal_X = 0.0f;
	float Normal_Z = 0.0f;

	VecX = g_Player.pos.x - pEnemy->pos.x;
	VecZ = g_Player.pos.z - pEnemy->pos.z;

	Length = sqrtf(VecX * VecX + VecZ * VecZ);

	Normal_X = VecX / Length;
	Normal_Z = VecZ / Length;

	Normal_X = Normal_X * ENEMY_REFLECT_POWER + g_Time * ENEMY_REFLECT_PLUS_POWER;
	Normal_Z = Normal_Z * ENEMY_REFLECT_POWER + g_Time * ENEMY_REFLECT_PLUS_POWER;

	g_PlayerVec += D3DXVECTOR3(Normal_X, 0.0f, Normal_Z);

	PlaySound(g_SE_Bumper, 0);
	SetVolume(g_SE_Bumper, 0.3f);

	return;
}

//=============================================================================
// �v���C���[�̔���(��)
//=============================================================================
void SetPlayerReflectWall(WALL* pWall)
{
	float VecX = 0.0f;
	float VecZ = 0.0f;
	float Length = 0.0f;
	float Normal_X = 0.0f;
	float Normal_Z = 0.0f;

	VecX = 0.0f - pWall->pos.x;
	VecZ = 0.0f - pWall->pos.z;

	Length = sqrtf(VecX * VecX + VecZ * VecZ);

	Normal_X = VecX / Length;
	Normal_Z = VecZ / Length;

	float PlayerVec = fabsf(g_PlayerVec.x) + fabsf(g_PlayerVec.z);


	Normal_X = Normal_X * fabsf(PlayerVec);
	Normal_Z = Normal_Z * fabsf(PlayerVec);

	g_PlayerVec = D3DXVECTOR3(Normal_X, 0.0f, Normal_Z);

	int SENo = 0;

	if (rand() % 2 == 0)
		SENo = g_SE_Wall_1;
	else
		SENo = g_SE_Wall_2;

	PlaySound(SENo, 0);
	SetVolume(SENo, 0.3f);

	return;
}

int GetTime(void)
{
	return g_Time;
}

