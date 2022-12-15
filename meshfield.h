//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX_2 (100 + 2)				//必要な頂点の数 (4以上)
#define NUM_INDEX_2 (13 + ((NUM_VERTEX_2 - 6) * 3))
#define MESH_WIDTH		3000.0f				// 直径
#define MESH_WIDTH_HALF	(MESH_WIDTH / 2.0f)	// 半径
// 中心座標
#define MESH_POS_X		0.0f
#define MESH_POS_Y		-10.0f
#define MESH_POS_Z		0.0f
// 端の高さ
#define MESH_EDGE_POS_Y	100.0f

// 地面の高さ
#define MESH_GROUND_POS_Y	-250.0f
#define MESH_GROUND_WIDTH	3500.0f

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

