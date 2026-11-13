//----------------------------------------------------------
//
// エンド表示を表示する処理[RankingEnd.cpp]
// Author：清野蒼翔
//
//----------------------------------------------------------

#include "main.h"
#include "Ranking_End.h"
#include "RankSetter.h"
#include <stdio.h>

//マクロ定義
#define MAX_BUFFER_END (3)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureRanking_End = NULL;  //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking_End = NULL;       //頂点バッファへのポインタ

int g_aScore[MAX_BUFFER_END] = { 0, 0, 0 };  // スコアを保存
int g_aEndNumber[MAX_BUFFER_END] = { 0, 0, 0 };  // エンドを保存

const char* FILE_PATH_RANKING_END = "data//score.txt";


//-------------------------------------------
//    エンド表示の初期化処理
//-------------------------------------------
void InitRankingEnd(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//End_Alp.png",
		&g_apTextureRanking_End);


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BUFFER_END,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking_End,
		NULL);

	FILE* pFile = NULL;

	pFile = fopen(FILE_PATH_RANKING_END, "r");
	if (pFile != NULL)
	{//ファイルが開けた場合

		for (int nCnt = 0; nCnt < MAX_BUFFER_END; nCnt++)
		{
			//ファイルから文字列を読み込む
			fscanf(pFile, "%d", &g_aScore[nCnt]);
			fscanf(pFile, "%d", &g_aEndNumber[nCnt]);

		}
		//ファイルを閉じる
		fclose(pFile);
	}

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRanking_End->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BUFFER_END; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(1000.0f, 210.0f + 100.0f * nCnt, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1100.0f, 210.0f + 100.0f * nCnt, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1000.0f, 210.0f + 100.0f * (nCnt + 1), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1100.0f, 210.0f + 100.0f * (nCnt + 1), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.25f * g_aEndNumber[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f * (g_aEndNumber[nCnt] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.25f * g_aEndNumber[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f * (g_aEndNumber[nCnt] + 1), 1.0f);

		pVtx += 4;
	}
	

	//頂点バッファをアンロックする
	g_pVtxBuffRanking_End->Unlock();

}

//-------------------------------------------
//    エンド表示の終了処理
//-------------------------------------------
void UninitRankingEnd(void)
{
	//テクスチャの破棄
	if (g_apTextureRanking_End != NULL)
	{
		g_apTextureRanking_End->Release();
		g_apTextureRanking_End = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking_End != NULL)
	{
		g_pVtxBuffRanking_End->Release();
		g_pVtxBuffRanking_End = NULL;
	}
}

//--------------------------------------
// エンド表示の更新処理
//--------------------------------------
void UpdateRankingEnd(void)
{
	
}

//-------------------------------------------
//   エンド表示の描画処理
//-------------------------------------------
void DrawRankingEnd(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking_End, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureRanking_End);

	for (int nCnt = 0; nCnt < MAX_BUFFER_END; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);      //描画を開始する頂点インデックス

	}
}