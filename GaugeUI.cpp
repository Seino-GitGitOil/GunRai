//-----------------------------------------
//
// ゲージ系のGaugeを表示する処理 [GaugeUI.cpp]
// Author：清野蒼翔
//
//-----------------------------------------
#include "GaugeUI.h"
#include "score.h"
#include "bullet.h"
#include "player.h"
#include "Boss.h"
#include "BossDirection.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureGauge[GAUGE_MAX] = {};        //テクスチャ(３枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;  //頂点バッファへのポインタ

const char* g_aGaugeTexture_Path[GAUGE_MAX] =
{
	"data//TEXTURE//BulletGauge.png",
	"data//TEXTURE//BurstGauge.png",
	"data//TEXTURE//BossLifeGauge.png",

};                 //ゲージテクスチャのパス


//-------------------------------------------
//    ゲージの初期化処理
//-------------------------------------------
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャを読み込み
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aGaugeTexture_Path[nCnt],
			&g_apTextureGauge[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * GAUGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		switch (nCntGauge)   //各Gaugeの初期化
		{

		case BULLET_GAUGE:
			//残弾数ゲージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			

			break;

		case BURST_GAUGE:
			//バーストゲージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			
			break;

		case BOSS_LIFE_GAUGE:
			//ボスのライフゲージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			

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
	g_pVtxBuffGauge->Unlock();


}

//-------------------------------------------
//  ゲージの終了処理
//-------------------------------------------
void UninitGauge(void)
{
	//テクスチャの破棄
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		if (g_apTextureGauge[nCntGauge] != NULL)
		{
			g_apTextureGauge[nCntGauge]->Release();
			g_apTextureGauge[nCntGauge] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}
//---------------------------
// ゲージの更新処理
//---------------------------
void UpdateGauge(void)
{
	Player* pPlayer = GetPlayer();
	BOSS* pBoss = GetBoss();

	int nCntGauge;

	//頂点情報の設定
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		switch (nCntGauge)
		{
		case BULLET_GAUGE:
			//残り弾数ゲージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(980.0f, 536.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(980.0f + ((1241.0f - 980.0f) * (float)pPlayer->nBulletStock / MAX_BULLET_GAUGE), 536.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(980.0f, 555.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(980.0f + ((1241.0f - 980.0f) * (float)pPlayer->nBulletStock / MAX_BULLET_GAUGE), 555.0f, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)pPlayer->nBulletStock / MAX_BULLET_GAUGE, 0.0f);   //  残弾数÷最大弾数
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)pPlayer->nBulletStock / MAX_BULLET_GAUGE, 1.0f);

			switch (pPlayer->PlayerState)
			{
			case PLAYERSTATE_OVERHEAT:
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);   //(r,g,b,a)
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
				break;

			default:
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}
			

			break;

		case BURST_GAUGE:
			//バーストゲージ

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(961.5f, 615.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(961.5f + ((1224.5f - 958.0f) * (float)pPlayer->nBurstMater / MAX_BURST_MATER), 615.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(961.5f, 675.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(961.5f + ((1224.5f - 958.0f) * (float)pPlayer->nBurstMater / MAX_BURST_MATER), 675.0f, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)pPlayer->nBurstMater / MAX_BURST_MATER, 0.0f);   //  残弾数÷最大弾数
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)pPlayer->nBurstMater / MAX_BURST_MATER, 1.0f);
			break;

		case BOSS_LIFE_GAUGE:
			//ボスの体力ゲージ
			pVtx[0].pos = D3DXVECTOR3(363.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(363.0f + ((917.0f - 363.0f) * (float)pBoss->nLife / pBoss->nMaxLife), 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(363.0f, 25.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(363.0f + ((917.0f - 363.0f) * (float)pBoss->nLife / pBoss->nMaxLife), 25.0f, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)pBoss->nLife / pBoss->nMaxLife, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)pBoss->nLife / pBoss->nMaxLife, 1.0f);

		}
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//-------------------------------------------
//    ゲージの描画処理
//-------------------------------------------
void DrawGauge(void)
{
	BOSS* pBoss = GetBoss();  // ボスの情報を取得
	int nBossDirState = GetBossDirState();  // ボスの演出情報を取得

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX; nCntGauge++)
	{
		if (nCntGauge == BOSS_LIFE_GAUGE && pBoss->bUse == false)
		{// ボスがいない場合は描画しない
			continue;
		}
		else if (nCntGauge == BOSS_LIFE_GAUGE && nBossDirState != BOSS_DIR_NOTHING)
		{//またはボス演出中なら描画しない

			continue;

		}
		
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureGauge[nCntGauge]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
	}
}