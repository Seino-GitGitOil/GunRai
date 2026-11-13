//--------------------------------------------------
//
// リザルトでのスコア表示処理[ResultScore.cpp]
// Author：清野蒼翔
//
//--------------------------------------------------

#include "ResultScore.h"
#include "RankSetter.h"
#include "score.h"
#include "sound.h"

//マクロ定義
#define NUM_RESULT_SCORE_HURF_WIDTH (25.0f)  // スコアの数字の横の長さの半分
#define NUM_RESULT_SCORE_HURF_HEIGHT (50.0f) // スコアの数字の縦の長さの半分


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;
D3DXVECTOR3 g_posResultScore;   // スコアの位置
int g_nResultScore;             // リザルト表示用のスコア
int g_nDisplayScore;            // 増加途中のスコア


//---------------------------
//  最終スコアの初期化処理
//---------------------------
void InitResultScore(void)
{
    g_nResultScore = GetScore();  // プレイした最終スコアを取得
	g_nDisplayScore = 0;          // 増加途中のスコア



	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//number000.png",
		&g_pTextureResultScore);

	g_posResultScore = D3DXVECTOR3(520.0f, 185.0f, 0.0f);    //位置を初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * NUM_PLACE
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posResultScore.x + (NUM_RESULT_SCORE_HURF_WIDTH * 2 * nCnt) - NUM_RESULT_SCORE_HURF_WIDTH, g_posResultScore.y - NUM_RESULT_SCORE_HURF_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResultScore.x + (NUM_RESULT_SCORE_HURF_WIDTH * 2 * nCnt) + NUM_RESULT_SCORE_HURF_WIDTH, g_posResultScore.y - NUM_RESULT_SCORE_HURF_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResultScore.x + (NUM_RESULT_SCORE_HURF_WIDTH * 2 * nCnt) - NUM_RESULT_SCORE_HURF_WIDTH - 15.0f, g_posResultScore.y + NUM_RESULT_SCORE_HURF_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResultScore.x + (NUM_RESULT_SCORE_HURF_WIDTH * 2 * nCnt) + NUM_RESULT_SCORE_HURF_WIDTH - 15.0f, g_posResultScore.y + NUM_RESULT_SCORE_HURF_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();

}
//---------------------------
//  最終スコアの終了処理
//---------------------------
void UninitResultScore(void)
{
	//テクスチャの破棄
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//---------------------------
//  最終スコアの更新処理
//---------------------------
void UpdateResultScore(void)
{
	int nDev = 10000000;     // 一番最初の「割る」数　を設定     割られる数はこれに*10してやればよさそうね
	int nAdd;
	
	//スコアのロール処理
	if (g_nDisplayScore < g_nResultScore)
	{
		nAdd = (g_nResultScore - g_nDisplayScore) / 10;

		if (nAdd < 1)
		{//増加量が１を下回る場合

			nAdd = 1;
		}

		//加算
		g_nDisplayScore += nAdd;

		PlaySound(SOUND_LABEL_SE_COUNTUP);

		// 加算し過ぎを防止
		if (g_nDisplayScore > g_nResultScore)
		{
			g_nDisplayScore = g_nResultScore;
		}
	}

	//頂点情報の設定   ８桁分だね
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nDisplayScore % (nDev * 10) / nDev), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (g_nDisplayScore % (nDev * 10) / nDev + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nDisplayScore % (nDev * 10) / nDev), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (g_nDisplayScore % (nDev * 10) / nDev + 1), 1.0f);


		nDev /= 10;        //割る数・割られる数の桁を１減らす
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//---------------------------
//  最終スコアの描画処理
//---------------------------
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultScore);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);      //描画を開始する頂点インデックス
	}
}



