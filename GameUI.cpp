//---------------------------------------
//
// ゲームUIの処理[GameUI.cpp]
// Author：清野蒼翔
//
//---------------------------------------

#include "GameUI.h"
#include "score.h"
#include "bullet.h"
#include "player.h"
#include "combo.h"
#include "Boss.h"
#include "BossDirection.h"
#include "game.h"  //マクロ使うためだけ


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureUI[UI_MAX] = {};  //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;       //頂点バッファへのポインタ
int g_nZoomCounter;   // ズームするタイミングを定めるカウンター
const char* g_aUITexture_Path[UI_MAX] = 
{

	"data//TEXTURE//Life_str.png",
	"data//TEXTURE//Bomb_str.png",
	
	"data//TEXTURE//Combo.png",

	"data//TEXTURE//HowToPlay.png",

	"data//TEXTURE//BULLET_str.png",    
	"data//TEXTURE//BulletGaugeFrame.png",     // 残弾数ゲージの枠
	
	"data//TEXTURE//BURST_str.png",
	"data//TEXTURE//BurstGaugeFrame.png",      // バーストゲージの枠

	"data//TEXTURE//Score_str_Game.png",       // スコアの文字

	"data//TEXTURE//Ranking_str_Game.png",     // ランキングの文字
	"data//TEXTURE//RankSimbol_Game.png",      // ランクシンボル

	"data//TEXTURE//MissionFrame.png",         // Missionの枠
	"data//TEXTURE//Mission_str.png",          // Missionの文字
	"data//TEXTURE//MissionList.png",          // ミッション一覧

	"data//TEXTURE//BossLifeGaugeFrame.png",   // ボスのHPゲージの枠

};                 //UIテクスチャのパス

//-------------------------------------------
//    ＵＩの初期化処理
//-------------------------------------------
void InitUI(void)
{
	g_nZoomCounter = 0;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャを読み込み
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aUITexture_Path[nCntUI],
			&g_apTextureUI[nCntUI]);
	}
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UI_MAX,                   //sizeof(VERTEX_2D) * [描画するのに必要な頂点数] * UI_MAX
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		switch (nCntUI)   //各UIの初期化
		{

		case UI_LIFE_STRING:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(960.0f, 370.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1040.0f, 370.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 410.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1040.0f, 410.0f, 0.0f);
			
			break;

		case UI_BOMB_STRING:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(960.0f, 420.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1040.0f, 420.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 460.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1040.0f, 460.0f, 0.0f);
			
			break;

		case UI_COMBO_STRING:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(845.0f, 170.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(920.0f, 170.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(845.0f, 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(920.0f, 200.0f, 0.0f);
			break;

		case UI_HOW_TO_PLAY:
			// 操作説明
			pVtx[0].pos = D3DXVECTOR3(960.0f, 5.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1260.0f, 5.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 380.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1260.0f, 380.0f, 0.0f);
			break;

		case UI_BULLET_STRING:
			//BULLETの文字
			pVtx[0].pos = D3DXVECTOR3(960.0f, 480.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1110.0f, 480.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 520.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1110.0f, 520.0f, 0.0f);
			break;

		case UI_BULLET_GAUGE_FRAME:
			// 残弾数ゲージの枠
			pVtx[0].pos = D3DXVECTOR3(960.0f, 520.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1260.0f, 520.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 570.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1260.0f, 570.0f, 0.0f);
			break;

		case UI_BURST_STRING:
			//「BURST」の文字
			pVtx[0].pos = D3DXVECTOR3(960.0f, 580.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1110.0f, 580.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 620.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1110.0f, 620.0f, 0.0f);
			break;

		case UI_SCORE_STR_GAME:
			// スコアの文字
			pVtx[0].pos = D3DXVECTOR3(30.0f, 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(160.0f, 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(30.0f, 70.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(160.0f, 70.0f, 0.0f);
			break;

		case UI_RANKING_STR_GAME:
			// ランキングの文字
			pVtx[0].pos = D3DXVECTOR3(70.0f, 170.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(300.0f, 170.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(70.0f, 240.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(300.0f, 240.0f, 0.0f);
			break;

		case UI_RANKING_SIMBOL:
			// ランキングシンボル
			pVtx[0].pos = D3DXVECTOR3(20.0f, 230.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(100.0f, 230.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f, 412.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(100.0f, 412.0f, 0.0f);
			break;

		
		case UI_MISSION_FRAME:
			// ミッションの枠
			pVtx[0].pos = D3DXVECTOR3(0.0f, 465.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(360.0f, 465.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(360.0f, SCREEN_HEIGHT, 0.0f);
			break;

		case UI_MISSION_STRING:
			//「Mission」の文字
			pVtx[0].pos = D3DXVECTOR3(73.0f, 402.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(273.0f, 402.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(88.0f, 482.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(288.0f, 482.0f, 0.0f);
			break;

		case UI_MISSION_LIST:
			// ミッション一覧
			pVtx[0].pos = D3DXVECTOR3(20.0f, 460.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(425.0f, 460.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f, SCREEN_HEIGHT - 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(425.0f, SCREEN_HEIGHT - 20.0f, 0.0f);
			break;


		case UI_BURST_GAUGE_FRAME:
			// バーストゲージの枠
			pVtx[0].pos = D3DXVECTOR3(960.0f, 620.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1230.0f, 620.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(960.0f, 670.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1230.0f, 670.0f, 0.0f);

			break;

		case UI_BOSS_LIFE_GAUGE_FRAME:

			//ボスの体力ゲージ
			pVtx[0].pos = D3DXVECTOR3(360.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(920.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(360.0f, 25.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(920.0f, 25.0f, 0.0f);
			
			break;

		}

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
	g_pVtxBuffUI->Unlock();

}

//-------------------------------------------
//  ＵＩの終了処理
//-------------------------------------------
void UninitUI(void)
{
	//テクスチャの破棄
	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		if (g_apTextureUI[nCntUI] != NULL)
		{
			g_apTextureUI[nCntUI]->Release();
			g_apTextureUI[nCntUI] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//---------------------------
// ＵＩの更新処理
//---------------------------
void UpdateUI(void)
{

	Player* pPlayer = GetPlayer();
	float nAlpha = GetComboAlpha();
	int nCntUI;

	//頂点情報の設定
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		switch (nCntUI)
		{
		case UI_COMBO_STRING:


			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, nAlpha);   //(r,g,b,a)
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, nAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, nAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, nAlpha);

			break;

		case UI_BULLET_GAUGE_FRAME:
			//残り弾数ゲージの枠

			break;
			

		case UI_BURST_GAUGE_FRAME:
			//バーストゲージの枠

			if (pPlayer->bBurstGaugeMax == true)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);   //(r,g,b,a)
				pVtx[1].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			break;


		}
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}	
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

//-------------------------------------------
//    ＵＩの描画処理
//-------------------------------------------
void DrawUI(void)
{
	BOSS* pBoss = GetBoss();  // ボスの情報を取得
	int BossDirState = GetBossDirState();  // ボス演出の情報を取得

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI = 0; nCntUI < UI_MAX; nCntUI++)
	{
		if (nCntUI == UI_BOSS_LIFE_GAUGE_FRAME && pBoss->bUse == false)
		{//ボスが使われていない時は描画しない

			continue;    // for文の初めに戻る
		}
		else if (nCntUI == UI_BOSS_LIFE_GAUGE_FRAME && BossDirState != BOSS_DIR_NOTHING)
		{//またはボス演出中なら描画しない

			continue;

		}

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureUI[nCntUI]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);      //描画を開始する頂点インデックス
		
	}
}