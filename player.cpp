//=============================================================================
//
// ポリゴン表示処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DX11_MODEL	g_Model;		// モデル読み込み
static PLAYER		g_Player;		// プレイヤー情報		
static float        g_Rot;			// キャラクターの向き
static D3DXVECTOR3	g_PlayerVec;	// プレイヤーにかかるベクトル
static int			g_SE_Bumper;	// バンパーSE
static int			g_SE_Wall_1;	// 壁SE1
static int			g_SE_Wall_2;	// 壁SE2

static int			g_Time;
static int			g_Timecnt;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	// 位置・回転・スケールの初期設定
	g_Player.pos = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.scl = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	g_Player.use = true;

	//当たり判定用サイズの初期設定
	g_Player.size = D3DXVECTOR3(90.0f, 90.0f, 90.0f);

	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/pinball_ball.obj", &g_Model);

	g_SE_Bumper = LoadSound("data/SE/Bumper.wav");
	g_SE_Wall_1 = LoadSound("data/SE/Wall_1.wav");
	g_SE_Wall_2 = LoadSound("data/SE/Wall_2.wav");

	//影の設定
	D3DXVECTOR3 sPos = g_Player.pos;
	sPos.y = 0.0f;
	g_Player.idxShadow = CreateShadow(sPos, 50.0f, 50.0f);

	// 並行光源の設定（世界を照らす光）
	LIGHT *pLight = NULL;
	pLight = GetLightData(0);
	pLight->Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);// 光の向き
	pLight->Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);	// 光の色
	pLight->Type = LIGHT_TYPE_DIRECTIONAL;					// 並行光源
	pLight->Enable = true;									// このライトをON
	SetLightData(0, pLight);								// これで設定している

	// 点光源の設定
	pLight = GetLightData(1);
	pLight->Position = D3DXVECTOR4(0.0f,500.0f, 0.0f, 0.0f);
	pLight->Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);// 光の向き
	pLight->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
	pLight->Attenuation = 1000.0f;							// 減衰距離
	pLight->Type = LIGHT_TYPE_POINT;						// 並行光源
	pLight->Enable = true;									// このライトをON
	SetLightData(1, pLight);								// これで設定している

	g_PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Time = 0;
	g_Timecnt = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//objモデルの解放
	UnloadModel(&g_Model);
	StopSoundAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	//移動速度の減衰処理
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

	// Y座標更新
	float XZVec = 0.0f;
	float XVec = 0.0f;
	float ZVec = 0.0f;

	XVec = (float)fabs(g_Player.pos.x);
	ZVec = (float)fabs(g_Player.pos.z);

	XZVec = sqrtf(XVec * XVec + ZVec * ZVec) / (MESH_WIDTH / 2.0f); // 真ん中から端までのどこの位置にいるか

	// 中心に向かって重力
	D3DXVECTOR3 Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f) - g_Player.pos;

	float Length = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);

	float Normal_X = Vec.x / Length;
	float Normal_Y = Vec.y / Length;
	float Normal_Z = Vec.z / Length;

	Vec = D3DXVECTOR3(Normal_X * M_CENTERGRAVITY, Normal_Y * 0.0f, Normal_Z * M_CENTERGRAVITY);

	// プレイヤーの移動量
	float PlayerVecLength = sqrtf(g_PlayerVec.x * g_PlayerVec.x + g_PlayerVec.z * g_PlayerVec.z);
	float PlayerVecNomalX = 0.0f;
	float PlayerVecNomalZ = 0.0f;

	if (PlayerVecLength > 0.0f)
	{
		PlayerVecNomalX = g_PlayerVec.x / PlayerVecLength;
		PlayerVecNomalZ = g_PlayerVec.z / PlayerVecLength;
	}
	
	// 真ん中のスピードアップ
	if (XZVec < 0.085f && 
		g_Time > M_CENTER_MOVE_START_TIME &&
		PlayerVecLength < CENTER_MAX_SPEED)
	{
		// 中心に向かって力を加える
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


	{	// 押した方向にプレイヤーを移動させる
		// 押した方向にプレイヤーを向かせている所
		g_Player.rot.y = g_Rot + cam->rot.y;

		/*g_Player.pos.x += sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z += cosf(g_Player.rot.y) * g_Player.spd;*/
	}

	if (XZVec <= 1.0f)
	{
		// 中心に向かって重力
		g_PlayerVec += Vec;
		// Y座標更新
		g_Player.pos.y = XZVec * MESH_EDGE_POS_Y + 10.0f;
		// 押した方向にプレイヤーを移動させる
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

	// 弾を撃つ
	/*if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(g_Player.pos);
	}*/

	//影の位置の更新
	D3DXVECTOR3 sPos = g_Player.pos;
	//sPos.y = 0.0f;
	SetPositionShadow(g_Player.idxShadow, sPos);

#ifdef _DEBUG	// デバッグ情報を表示する
	char* str = GetDebugStr();
	printf(&str[strlen(str)], " rotX:%.2f rotY:%.2f", g_Player.rot.x, g_Player.rot.y);

#endif
	
	// 時間
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
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化（単位行列を作る関数）
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);

	                   //第1引数 ＝          第2引数      ＊    第3引数
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&g_Player.mtxWorld);

	//カリングなし
	SetCullingMode(CULL_MODE_NONE);

	//objモデルの描画
	DrawModel(&g_Model);

	//カリングを元に戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// プレイヤーの反射(バンパー)
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
// プレイヤーの反射(壁)
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

