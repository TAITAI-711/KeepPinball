//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
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
// マクロ定義
//*****************************************************************************

struct FIELD
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
	D3DXMATRIX  mtxWorld;
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11Buffer* g_IndexBuffer = NULL;	// インデックスバッファ

static ID3D11Buffer* g_VertexBuffer_2 = NULL;	// 頂点情報
static ID3D11Buffer* g_IndexBuffer_2 = NULL;	// インデックスバッファ

static ID3D11Buffer* g_VertexBuffer_3 = NULL;	// 頂点情報
static ID3D11Buffer* g_IndexBuffer_3 = NULL;	// インデックスバッファ

static FIELD	g_Field;	// フィールド情報
static int		g_Stage_TexNo;	// テクスチャ番号
static int		g_StageCenter_TexNo;	// テクスチャ番号
static int		g_Ground_TexNo;	// テクスチャ番号

static int numVertex = 4;
static int numIndex = 4;

static int g_ColorChangeCnt = 0;

static int g_SE_ColorChange_No = 0;
static bool g_SE_Flag = false;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(void)
{
	g_ColorChangeCnt = 0;

	// 位置・回転・スケールの初期設定
	g_Field.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Field.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャ生成
	g_StageCenter_TexNo = LoadTexture("data/TEXTURE/Pinball_StageCenter.png");
	g_Stage_TexNo = LoadTexture("data/TEXTURE/Pinball_Stage.png");
	g_Ground_TexNo = LoadTexture("data/TEXTURE/fade_white.png");

	g_SE_ColorChange_No = LoadSound("data/SE/colorchange.wav");

	g_SE_Flag = false;


	int yoko = 1;
	int tate = 1;

	//必要な頂点数を計算する
	numVertex = (yoko + 1) * (tate + 1);
	numIndex = (yoko + 1) * 2 * tate + (tate - 1) * 2;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);

	
	// ステージ
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成2
	D3D11_BUFFER_DESC bd_2;
	ZeroMemory(&bd_2, sizeof(bd_2));
	bd_2.Usage = D3D11_USAGE_DYNAMIC;
	bd_2.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX_2;
	bd_2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd_2, NULL, &g_VertexBuffer_2);

	// インデックスバッファ生成2
	ZeroMemory(&bd_2, sizeof(bd_2));
	bd_2.Usage = D3D11_USAGE_DYNAMIC;
	bd_2.ByteWidth = sizeof(unsigned short) * NUM_INDEX_2;
	bd_2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd_2, NULL, &g_IndexBuffer_2);

	// 地面
	ID3D11Device* pDevice3 = GetDevice();

	// 頂点バッファ生成3
	D3D11_BUFFER_DESC bd_3;
	ZeroMemory(&bd_3, sizeof(bd_3));
	bd_3.Usage = D3D11_USAGE_DYNAMIC;
	bd_3.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd_3.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice3->CreateBuffer(&bd_3, NULL, &g_VertexBuffer_3);

	// インデックスバッファ生成3
	ZeroMemory(&bd_3, sizeof(bd_3));
	bd_3.Usage = D3D11_USAGE_DYNAMIC;
	bd_3.ByteWidth = sizeof(unsigned short) * numIndex;
	bd_3.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_3.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice3->CreateBuffer(&bd_3, NULL, &g_IndexBuffer_3);


	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;
		

		// 頂点座標の設定
		pVtx[0].Position = D3DXVECTOR3(-200.0f, 0.0f, -200.0f);
		pVtx[1].Position = D3DXVECTOR3(-200.0f, 0.0f, 200.0f);
		pVtx[2].Position = D3DXVECTOR3(200.0f, 0.0f, -200.0f);
		pVtx[3].Position = D3DXVECTOR3(200.0f, 0.0f, 200.0f);
		// UV値の設定
		pVtx[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		// 法線の設定
		pVtx[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		pIdx[0] = 0;
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 3;
				
 		GetDeviceContext()->Unmap(g_IndexBuffer, 0);
	}

	{//頂点バッファの中身を埋める
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

			Rate = (float)(i - 1) / (NUM_VERTEX_2 - 2); // 割合

			x = Hankei * cosf(2.0f * (float)M_PI * Rate);
			z = Hankei * sinf(2.0f * (float)M_PI * Rate);

			float BaseAngle = atan2f(z, x);		// 中心点から頂点に対する角度
			D3DXVECTOR2 temp = D3DXVECTOR2(x, z);
			float Radius = D3DXVec2Length(&temp);	// 中心点から頂点に対する距離

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

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
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

	{//頂点バッファの中身を埋める

		// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer_3, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx_3 = (VERTEX_3D*)msr.pData;


		// 頂点座標の設定
		pVtx_3[0].Position = D3DXVECTOR3(-MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, -MESH_GROUND_WIDTH);
		pVtx_3[1].Position = D3DXVECTOR3(-MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, MESH_GROUND_WIDTH);
		pVtx_3[2].Position = D3DXVECTOR3(MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, -MESH_GROUND_WIDTH);
		pVtx_3[3].Position = D3DXVECTOR3(MESH_GROUND_WIDTH, MESH_GROUND_POS_Y, MESH_GROUND_WIDTH);
		// UV値の設定
		pVtx_3[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		pVtx_3[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		pVtx_3[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		pVtx_3[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		// 法線の設定
		pVtx_3[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx_3[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx_3[0].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[1].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[2].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);
		pVtx_3[3].Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.0f, 1.0f);

		GetDeviceContext()->Unmap(g_VertexBuffer_3, 0);
	}

	{//インデックスバッファの中身を埋める

		// インデックスバッファのポインタを取得
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
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	//インデックスバッファの解放
	if (g_IndexBuffer)
		g_IndexBuffer->Release();

	//頂点バッファの解放
	if (g_VertexBuffer)
		g_VertexBuffer->Release();


	//インデックスバッファの解放
	if (g_IndexBuffer_2)
		g_IndexBuffer_2->Release();

	// 頂点バッファの解放
	if (g_VertexBuffer_2)
		g_VertexBuffer_2->Release();

	//インデックスバッファの解放
	if (g_IndexBuffer_3)
		g_IndexBuffer_3->Release();

	// 頂点バッファの解放
	if (g_VertexBuffer_3)
		g_VertexBuffer_3->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
	// フィールドの色変更
	if (GetTime() % 10 == 0 && GetTime() > 0)
	{
		if (g_ColorChangeCnt == 0)
		{
			//{//頂点バッファの中身を埋める
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
			{//頂点バッファの中身を埋める
				D3D11_MAPPED_SUBRESOURCE msr;
				GetDeviceContext()->Map(g_VertexBuffer_2, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

				VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

				D3DXCOLOR Color = D3DXCOLOR((rand() % 101) * 0.01f, (rand() % 101) * 0.01f, (rand() % 101) * 0.01f, 1.0f); // 色変化

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

					Rate = (float)(i - 1) / (NUM_VERTEX_2 - 2); // 割合

					x = Hankei * cosf(2.0f * (float)M_PI * Rate);
					z = Hankei * sinf(2.0f * (float)M_PI * Rate);

					float BaseAngle = atan2f(z, x);		// 中心点から頂点に対する角度
					D3DXVECTOR2 temp = D3DXVECTOR2(x, z);
					float Radius = D3DXVec2Length(&temp);	// 中心点から頂点に対する距離

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
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	for (int i = 0; i < 3; i++)
	{
		SetCullingMode(CULL_MODE_BACK);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;

		ID3D11Buffer* VertexBuffer = NULL;	// 頂点バッファ
		ID3D11Buffer* IndexBuffer = NULL;	// インデックスバッファ

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

		// インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		// プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// マテリアル設定
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

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, GetTexture(TexNo));


		D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Field.mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScl, g_Field.scl.x, g_Field.scl.y, g_Field.scl.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxScl);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
		D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
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

		// ポリゴンの描画
		GetDeviceContext()->DrawIndexed(Index, 0, 0);
	}
}

