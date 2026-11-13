//--------------------------------
//
// スコア処理[score.cpp]
// Author：清野蒼翔
//
//--------------------------------
#include "main.h"
#include "score.h"
#include "player.h"
#include "enemy.h"



//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
D3DXVECTOR3 g_posScore;   // スコアの位置
int g_nScore;             // スコアの値


//----------------------------------
//  スコアの初期化処理
//----------------------------------
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//number000.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(NUM_HARF_WIDTH * 6, NUM_HARF_HEIGHT * 2, 0.0f);    //位置を初期化する
	g_nScore = 0;    //値を初期化する


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * NUM_PLACE
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//頂点情報の設定   ８桁分だね

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posScore.y - NUM_HARF_HEIGHT + 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posScore.y - NUM_HARF_HEIGHT + 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posScore.y + NUM_HARF_HEIGHT + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posScore.y + NUM_HARF_HEIGHT + 30.0f, 0.0f);

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
	g_pVtxBuffScore->Unlock();


}
//-----------------------------
//   スコアの終了処理
//-----------------------------
void UninitScore(void)               
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//---------------------
// スコアの更新処理
//---------------------
void UpdateScore(void)
{
	int nDev = 10000000;     // 一番最初の「割る」数　を設定     割られる数はこれに*10してやればよさそうね

	//頂点情報の設定   ８桁分だね
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev + 1), 1.0f);


		nDev /= 10;        //割る数・割られる数の桁を１減らす
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//---------------------
//  スコアの描画処理
//---------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);      //描画を開始する頂点インデックス
	}
}
//-------------------------
// スコアのセット処理
//-------------------------
void SetScore(D3DXVECTOR3 pos, int nScore)
{
	int nDev = 10000000;     // 一番最初の「割る」数　を設定     割られる数はこれに*10してやればよさそうね

	g_posScore = pos;

	//頂点情報の設定   ８桁分だね
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (g_nScore % (nDev * 10) / nDev + 1), 1.0f);


		nDev /= 10;        //割る数・割られる数の桁を１減らす
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}
//-------------------------
//  固定スコアを加える処理
//-------------------------
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE];        // 各桁の数字を格納

	int Dividend = 100000000;    // 割られる数(初期値：一億)
	int Divisor = 10000000;      // 割る数 (初期値：一千万)

	g_nScore += nValue;          // 引数に与えられた値を加算する処理

	if (g_nScore > 99999999)
	{
		g_nScore = 99999999;   //スコアをカンスト値でリセット
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		aTexU[nCnt] = (g_nScore / Divisor) % 10;
		Divisor /= 10;
	}
}
//-------------------------------------
//   敵をキルした際のスコア加算処理
//-------------------------------------
void AddKillScore(Enemy* pEnemy)
{
	Player* pPlayer = GetPlayer();

	int aTexU[NUM_PLACE];        // 各桁の数字を格納

	int Dividend = 100000000;    // 割られる数(初期値：一億)
	int Divisor = 10000000;      // 割る数 (初期値：一千万)
	float nScoreBonus;           // スコア倍率
	float nKillScore;            // キルスコア
		

	//コンボ数によるスコア倍率補正
	/*
	if (pPlayer->nCombo < 10)
	{
		nScoreBonus = 1.0f;
	}
	else if (pPlayer->nCombo < 20 && pPlayer->nCombo >= 10)
	{
		nScoreBonus = 1.1f;
	}
	else if (pPlayer->nCombo < 30 && pPlayer->nCombo >= 20)
	{
		nScoreBonus = 1.2f;
	}
	else if (pPlayer->nCombo < 40 && pPlayer->nCombo >= 30)
	{
		nScoreBonus = 1.3f;
	}
	else if (pPlayer->nCombo < 50 && pPlayer->nCombo >= 40)
	{
		nScoreBonus = 1.4f;
	}
	else if (pPlayer->nCombo < 60 && pPlayer->nCombo >= 50)
	{
		nScoreBonus = 1.5f;
	}
	else if (pPlayer->nCombo < 70 && pPlayer->nCombo >= 60)
	{
		nScoreBonus = 1.6f;
	}
	else if (pPlayer->nCombo < 80 && pPlayer->nCombo >= 70)
	{
		nScoreBonus = 1.7f;
	}
	else if (pPlayer->nCombo < 90 && pPlayer->nCombo >= 80)
	{
		nScoreBonus = 1.8f;
	}
	else if (pPlayer->nCombo < 100 && pPlayer->nCombo >= 90)
	{
		nScoreBonus = 1.9f;
	}
	*/
	if (pPlayer->nCombo < 100 && pPlayer->nCombo >= 0)
	{
		nScoreBonus = 1.0 + (0.1 * (pPlayer->nCombo / 10));
	}
	else if (pPlayer->nCombo < 200 && pPlayer->nCombo >= 100)
	{
		nScoreBonus = 2.0f;
	}
	else if (pPlayer->nCombo < 300 && pPlayer->nCombo >= 200)
	{
		nScoreBonus = 2.5f;
	}
	else if (pPlayer->nCombo < 400 && pPlayer->nCombo >= 300)
	{
		nScoreBonus = 3.0f;
	}
	else
	{
		nScoreBonus = 5.0f;
	}


	switch (pEnemy->nType)
	{
	case ENEMY_TYPE_A:
		nKillScore = 12010.0f;
		break;
	case ENEMY_TYPE_B:
		nKillScore = 33010.0f;
		break;
	case ENEMY_TYPE_C:
		nKillScore = 55050.0f;
		break;
	case ENEMY_TYPE_D:
		nKillScore = 33020.0f;
		break;
	case ENEMY_TYPE_E:
		nKillScore = 74030.0f;
		break;
	case ENEMY_TYPE_F:
		nKillScore = 51000.0f;
		break;
	}



	g_nScore += (int)(nKillScore * nScoreBonus);          //引数に与えられた値を加算する処理

	if (g_nScore > 99999999)
	{
		g_nScore = 99999999;   //スコアをカンスト値でリセット
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		aTexU[nCnt] = (g_nScore / Divisor) % 10;
		Divisor /= 10;
	}

}

//-------------------------------------------------
//  ボスに攻撃をヒットさせた際のスコア増加処理
//-------------------------------------------------
void AddBossHitScore(void)
{
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得
	
	int aTexU[NUM_PLACE];        // 各桁の数字を格納

	int Divisor = 10000000;      // 割る数 (初期値：一千万)
	int nAddScore;               // 加えるスコアの値
	float nScoreBonus;           // スコア倍率

	if (pPlayer->nCombo < 100 && pPlayer->nCombo >= 0)
	{
		nScoreBonus = 1.0 + (0.1 * (pPlayer->nCombo / 10));
	}
	else if (pPlayer->nCombo < 200 && pPlayer->nCombo >= 100)
	{
		nScoreBonus = 2.0f;
	}
	else if (pPlayer->nCombo < 300 && pPlayer->nCombo >= 200)
	{
		nScoreBonus = 2.5f;
	}
	else if (pPlayer->nCombo < 400 && pPlayer->nCombo >= 300)
	{
		nScoreBonus = 3.0f;
	}
	else
	{
		nScoreBonus = 5.0f;
	}

	nAddScore = 200;                                     // スコアを増やす処理(マクロ定義にいつかしよう)

	g_nScore += (int)(nAddScore * nScoreBonus);          // 引数に与えられた値を加算する処理

	if (g_nScore > 99999999)
	{
		g_nScore = 99999999;                             // スコアをカンスト値でリセット
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		aTexU[nCnt] = (g_nScore / Divisor) % 10;
		Divisor /= 10;
	}
}




//------------------------------
//      スコアを渡す
//------------------------------
int GetScore(void)
{
	return g_nScore;
}