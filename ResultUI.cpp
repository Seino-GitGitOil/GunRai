//--------------------------------------
//
// リザルト画面処理[resultUI.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "main.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "score.h"
#include "RankSetter.h"
#include "EnemyWave.h"
#include "input.h"

// マクロ定義
#define SCORE_RANK_BORDER_S (20000000)
#define SCORE_RANK_BORDER_A (10000000)
#define SCORE_RANK_BORDER_B (5000000)




// テクスチャの列挙型定義
typedef enum
{
	RESULT_TEXTURE_SCREENBOARD = 0,      // リザルトを表示するスクリーンみたいな画像 
	RESULT_TEXTURE_SCORE_STRING,         // Scoreの文字列
	RESULT_TEXTURE_END_STRING,           // END の文字列 
	RESULT_TEXTURE_END_ALPHABET,         // たどった√を示す文字
	RESULT_TEXTURE_RANK_STRING,          // Rankの文字列
	RESULT_TEXTURE_RANK_ALPHABET,        // ランクを示す文字
	RESULT_TEXTURE_RETURN_TITLE_STRING,  // ReturnTitle　の文字
	RESULT_TEXTURE_RANKING_STRING,       // Ranking　の文字
	RESULT_TEXTURE_RESTART_STRING,       // Restart　の文字
	RESULT_TEXTURE_CURSOR,               // カーソル
	RESULT_TEXTURE_FRAME,                // 選択肢を囲う用の枠
	RESULT_TEXTURE_MAX

}RESULT_TEXTURE_NAME;

//グローバル変数

LPDIRECT3DTEXTURE9 g_apTextureResult[RESULT_TEXTURE_MAX] = {};          // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;                        // 頂点バッファへのポインタ
static bool g_bEnterChecker = false;                                    // 遷移中かどうかのフラグ
int g_nResultSelect;                                                    // 選択肢
float g_Color_a[RESULT_TEXTURE_MAX];                                    // 画像の透明度
int g_End_Tex;                                                          // エンド分岐のテクスチャ座標調整
const char* g_aResultTexture_Path[RESULT_TEXTURE_MAX] =
{
	"data//TEXTURE//ResultScreen.png",
	"data//TEXTURE//ResultScore_str.png",
	"data//TEXTURE//End_str.png",
	"data//TEXTURE//End_Alp.png",
	"data//TEXTURE//Rank_str.png",
	"data//TEXTURE//Rank_Alp.png",
	"data//TEXTURE//ReturnToTitle_str.png",
	"data//TEXTURE//Ranking_str_result.png",
	"data//TEXTURE//Restart_str.png",
	"data//TEXTURE//Cursor2.png",
	"data//TEXTURE//ResultFrame.png",

};                 //背景テクスチャ

//----------------------------------
//   リザルト画面の初期化処理
//----------------------------------
void InitResultUI(void)
{
	// 全てのテクスチャの透明度を0.0fにする
	for (int nCnt = 0; nCnt < RESULT_TEXTURE_MAX; nCnt++)
	{
		g_Color_a[nCnt] = 0.0f;
	}

	int nCntResult;
	g_End_Tex = END_NOTHING;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (nCntResult = 0; nCntResult < RESULT_TEXTURE_MAX; nCntResult++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aResultTexture_Path[nCntResult],
			&g_apTextureResult[nCntResult]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RESULT_TEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RESULT_TEXTURE_MAX; nCnt++)
	{
		switch (nCnt)
		{

		case RESULT_TEXTURE_SCREENBOARD:

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;


		case RESULT_TEXTURE_SCORE_STRING:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(200.0f, 130.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(480.0f, 130.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(200.0f, 230.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(480.0f, 230.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_END_STRING:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(260.0f, 250.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450.0f, 250.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(260.0f, 350.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450.0f, 350.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_END_ALPHABET:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(450.0f, 250.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(600.0f, 250.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f, 350.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 350.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_RANK_STRING:

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(200.0f, 375.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(500.0f, 375.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(200.0f, 475.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(500.0f, 475.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_RANK_ALPHABET:

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(450.0f, 375.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(600.0f, 375.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f, 475.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 475.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		

		case RESULT_TEXTURE_RETURN_TITLE_STRING:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_RANKING_STRING:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(490.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(790.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(490.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(790.0f, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_RESTART_STRING:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;

		case RESULT_TEXTURE_CURSOR:
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(350.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(350.0f, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


			break;

		case RESULT_TEXTURE_FRAME:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


			break;





		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	PlaySound(SOUND_LABEL_BGM002);
}

//----------------------------------
//   リザルト画面の終了処理
//----------------------------------
void UninitResultUI(void)
{

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < RESULT_TEXTURE_MAX; nCnt++)
	{
		if (g_apTextureResult[nCnt] != NULL)
		{
			g_apTextureResult[nCnt]->Release();
			g_apTextureResult[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

}

//----------------------------------
//   リザルト画面の更新処理
//----------------------------------
int UpdateResultUI(int pResultTimer)
{
	//頂点情報の設定
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	int nScore = GetScore();
	g_End_Tex = GetEndRoute();


	float Rank_Tex;   // 達成ランクのアルファベットを表示
	
	


	if (pResultTimer >= 0 && pResultTimer <= 60)
	{
		if ((pResultTimer) % 6 == 0)
		{
			g_Color_a[RESULT_TEXTURE_SCREENBOARD] += 0.1f;     // 背景
			g_Color_a[RESULT_TEXTURE_SCORE_STRING] += 0.1f;   //「Score」の文字
			g_Color_a[RESULT_TEXTURE_RANK_STRING] += 0.1f;   //「Rank」の文字
			g_Color_a[RESULT_TEXTURE_END_STRING] += 0.1f;   //「End」の文字
		}
	}

	if (pResultTimer == 300)
	{
		g_Color_a[RESULT_TEXTURE_END_ALPHABET] = 1.0f;   //Endのアルファベット
	}

	if (pResultTimer == 360)
	{
		g_Color_a[RESULT_TEXTURE_RANK_ALPHABET] = 1.0f;   //Rankのアルファベット
	}

	if (pResultTimer == 390)
	{
		g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING] = 0.7f;
		g_Color_a[RESULT_TEXTURE_RANKING_STRING] = 0.7f;
		g_Color_a[RESULT_TEXTURE_RESTART_STRING] = 0.7f;

		g_Color_a[RESULT_TEXTURE_CURSOR] = 1.0f;
		g_Color_a[RESULT_TEXTURE_FRAME] = 1.0f;

	}
	if (pResultTimer > 390)
	{
		switch (g_nResultSelect)
		{
		case RESULT_MENU_RETURN_TITLE:
			g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING] = 1.0f;
			g_Color_a[RESULT_TEXTURE_RANKING_STRING] = 0.7f;
			g_Color_a[RESULT_TEXTURE_RESTART_STRING] = 0.7f;
			break;

		case RESULT_MENU_RANKING:
			g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING] = 0.7f;
			g_Color_a[RESULT_TEXTURE_RANKING_STRING] = 1.0f;
			g_Color_a[RESULT_TEXTURE_RESTART_STRING] = 0.7f;
			break;

		case RESULT_MENU_RESTART:
			g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING] = 0.7f;
			g_Color_a[RESULT_TEXTURE_RANKING_STRING] = 0.7f;
			g_Color_a[RESULT_TEXTURE_RESTART_STRING] = 1.0f;
			break;
		}
	}



	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RESULT_TEXTURE_MAX; nCnt++)
	{

		switch (nCnt)
		{
		case RESULT_TEXTURE_SCREENBOARD:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCREENBOARD]);
			break;

		case RESULT_TEXTURE_SCORE_STRING:


			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_SCORE_STRING]);

			break;

		case RESULT_TEXTURE_END_STRING:
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_STRING]);
			break;

		case RESULT_TEXTURE_END_ALPHABET:




			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_END_ALPHABET]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.25f * g_End_Tex, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f * (g_End_Tex + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.25f * g_End_Tex, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f * (g_End_Tex + 1), 1.0f);
			break;


		case RESULT_TEXTURE_RANK_STRING:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_STRING]);

			break;

		case RESULT_TEXTURE_RANK_ALPHABET:

			if (nScore >= SCORE_RANK_BORDER_S)
			{
				Rank_Tex = 0;
			}
			else if (nScore >= SCORE_RANK_BORDER_A && nScore < SCORE_RANK_BORDER_S)
			{
				Rank_Tex = 1;

			}
			else if (nScore >= SCORE_RANK_BORDER_B && nScore < SCORE_RANK_BORDER_A)
			{
				Rank_Tex = 2;

			}
			else if (nScore < SCORE_RANK_BORDER_B)
			{
				Rank_Tex = 3;

			}


			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANK_ALPHABET]);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.25f * Rank_Tex, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f * (Rank_Tex + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.25f * Rank_Tex, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f * (Rank_Tex + 1), 1.0f);

			break;

		case RESULT_TEXTURE_RETURN_TITLE_STRING:
			//選択肢の「Return Title」を表示

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RETURN_TITLE_STRING]);

			break;

		case RESULT_TEXTURE_RANKING_STRING:
			//選択肢の「Ranking」を表示

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RANKING_STRING]);


			break;

		case RESULT_TEXTURE_RESTART_STRING:
			//選択肢の「Restart」を表示

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_RESTART_STRING]);

			break;

		case RESULT_TEXTURE_CURSOR:
			// カーソル表示処理

			switch (g_nResultSelect)
			{
			case RESULT_MENU_RETURN_TITLE:
				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(175.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(125.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(175.0f, SCREEN_HEIGHT - 100.0f, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;

			case RESULT_MENU_RANKING:
				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(615.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(665.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(615.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(665.0f, SCREEN_HEIGHT - 100.0f, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;




			case RESULT_MENU_RESTART:
				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 175.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT - 200.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 175.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 125.0f, SCREEN_HEIGHT - 100.0f, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;

			
			}
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_CURSOR]);
			break;



		case RESULT_TEXTURE_FRAME:

			switch (g_nResultSelect)
			{
			case RESULT_MENU_RETURN_TITLE:
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				break;

			case RESULT_MENU_RANKING:
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(490.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(790.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(490.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(790.0f, SCREEN_HEIGHT, 0.0f);
				break;

			case RESULT_MENU_RESTART:
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Color_a[RESULT_TEXTURE_FRAME]);
				break;

			}

		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();


	return g_nResultSelect;   //現在の選択肢を返す(result.cppへ)
}

//----------------------------------
//   リザルト画面の描画処理
//----------------------------------
void DrawResultUI(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < RESULT_TEXTURE_MAX; nCnt++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}



}

//---------------------------------
// リザルトの選択肢を動かす処理
//---------------------------------
void ResultSelectMove(int nSelectMove)
{
	switch (nSelectMove)
	{
	case RESULT_SELECT_INC:
		if (g_nResultSelect == RESULT_MENU_RESTART)
		{//選択肢が一番左の場合
			g_nResultSelect = RESULT_MENU_RETURN_TITLE;
		}
		else
		{
			g_nResultSelect++;
		}

		PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
		break;

	case RESULT_SELECT_DEC:
		if (g_nResultSelect == RESULT_MENU_RETURN_TITLE)
		{//選択肢が一番右の場合
			g_nResultSelect = RESULT_MENU_RESTART;
		}
		else
		{
			g_nResultSelect--;
		}

		PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
		break;
	}
}
