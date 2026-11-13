//--------------------------------------
//
// タイトル画面処理の宣言[Ranking.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "main.h"
#include "Ranking.h"
#include "sound.h"
#include "fade.h"
#include "ResultScore.h"
#include "RankSetter.h"
#include "score.h"
#include "Ranking_End.h"
#include "input.h"



// テクスチャの列挙型定義
typedef enum
{
	RANKING_TEXTURE_BG = 0,               // 背景
	RANKING_TEXTURE_RANKING_STRING,       // ランキングの文字
	RANKING_TEXTURE_RETURN_TITLE,         // タイトルへ
	RANKING_TEXTURE_RANK_NUMBER,          // 数字１～３とYouを書いた四つを一枚のテクスチャに
	RANKING_TEXTURE_MAX

}RANKING_TEXTURE_NAME;






//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureRanking[RANKING_TEXTURE_MAX] = {};          // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;                       // 頂点バッファへのポインタ
bool g_bRankingEnterChecker = false;                                  // 遷移中かどうかのフラグ

const char* g_aRankingTexture_Path[RANKING_TEXTURE_MAX] =
{
	"data//TEXTURE//RankingBG.jpg",
	"data//TEXTURE//Ranking_str_result.png",
	"data//TEXTURE//ReturnToTitle_str.png",
	"data//TEXTURE//RankSimbol02.png",

};                 //背景テクスチャ

//----------------------------------
//   ランキング画面の初期化処理
//----------------------------------
void InitRanking(void)
{
	g_bRankingEnterChecker = false;   //エンターチェックを初期化

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < RANKING_TEXTURE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aRankingTexture_Path[nCnt],
			&g_apTextureRanking[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANKING_TEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RANKING_TEXTURE_MAX; nCnt++)
	{
		//頂点座標の設定
		switch (nCnt)
		{
		case RANKING_TEXTURE_BG:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;

		case RANKING_TEXTURE_RANKING_STRING:
			pVtx[0].pos = D3DXVECTOR3(460.0f, 50.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(860.0f, 50.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(460.0f, 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(860.0f, 200.0f, 0.0f);
			break;

		case RANKING_TEXTURE_RETURN_TITLE:
			pVtx[0].pos = D3DXVECTOR3(360.0f, SCREEN_HEIGHT - 80.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(920.0f, SCREEN_HEIGHT - 80.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(360.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(920.0f, SCREEN_HEIGHT, 0.0f);
			break;

		case RANKING_TEXTURE_RANK_NUMBER:
			pVtx[0].pos = D3DXVECTOR3(250.0f, 200.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450.0f, 200.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(250.0f, 600.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450.0f, 600.0f, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	UpdateRankingScore(0, 0);  //あえて空のスコアを渡して入れ替えのみを実行する

	LoadRanking();             //ゲーム未プレイでも問題なく起こすためのロード処理
	
	SetRankingScore(D3DXVECTOR3(500.0f, 260.0f, 0.0f), 30.0f, 40.0f, 20.0f);
	InitRankingEnd();

	PlaySound(SOUND_LABEL_BGM_TUTORIAL);

}

//----------------------------------
//   タイトル画面の終了処理
//----------------------------------
void UninitRanking(void)
{
	//サウンドの停止
	StopSound();

	//
	UninitRankingEnd();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < RANKING_TEXTURE_MAX; nCnt++)
	{
		if (g_apTextureRanking[nCnt] != NULL)
		{
			g_apTextureRanking[nCnt]->Release();
			g_apTextureRanking[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

}

//----------------------------------
//   タイトル画面の更新処理
//----------------------------------
void UpdateRanking(void)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();



	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B) == true) 
	{//決定キーが押された

		//モード設定
		SetFade(MODE_TITLE);
		PlaySound(SOUND_LABEL_SE_ENTER);
		g_bRankingEnterChecker = true;
	}
}

//----------------------------------
//   タイトル画面の描画処理
//----------------------------------
void DrawRanking(void)
{
	

	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < RANKING_TEXTURE_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureRanking[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	//ランキングスコアの描画処理
	DrawRankingScore();

	//
	DrawRankingEnd();
}