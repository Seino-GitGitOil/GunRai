//--------------------------------
//
// スコア処理[Combo.cpp]
// Author：清野蒼翔
//
//--------------------------------
#include "game.h"
#include "player.h"
#include "BossDirection.h"
#include "combo.h"

#define NUM_PLACE_COMBO (3)        // スコアの最大桁数
#define MAX_COMBO (999)            // スコアの最大数
#define NUM_HARF_WIDTH (15.0f)     // 数字の横幅の半分の長さ
#define NUM_HARF_HEIGHT (30.0f)    // 数字の縦幅の半分の長さ

// コンボの桁数
typedef enum
{
	NUM_DIGIT_ONE = 1,
	NUM_DIGIT_TWO,
	NUM_DIGIT_THREE,

}COMBO_DIGIT;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureCombo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;
D3DXVECTOR3 g_posCombo;         // コンボ数を描画する左上の座標
int g_nCombo;                   // コンボ数 (プレイヤーの情報から抜き取る)
int g_nComboResetTimer;         // コンボリセットを確認するタイマー
float g_nCombo_Alpha;           // コンボのα値

//----------------------------
// コンボの初期化処理
//----------------------------
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//Number000.png",
		&g_pTextureCombo);

	g_posCombo = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 位置
	g_nCombo = 0;                                  // コンボ数
	g_nComboResetTimer = 0;                        // コンボリセットタイマー
	g_nCombo_Alpha = 0.0f;                                // コンボ数の透明度


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_COMBO,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * NUM_PLACE
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_PLACE_COMBO; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posCombo.y - NUM_HARF_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posCombo.y - NUM_HARF_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posCombo.y + NUM_HARF_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posCombo.y + NUM_HARF_HEIGHT, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}
//----------------------------
// コンボの終了処理
//----------------------------
void UninitCombo(void)
{
	//テクスチャの破棄
	if (g_pTextureCombo != NULL)
	{
		g_pTextureCombo->Release();
		g_pTextureCombo = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//----------------------------
// コンボの更新処理
//----------------------------
void UpdateCombo(void)
{
	Player* pPlayer = GetPlayer();
	int nBossDirState = GetBossDirState();

	int nDev = 100;
	int nComboGidit;   //現在のコンボ数の桁数


	//プレイヤー構造体から「コンボ数」を取得
	g_nCombo = pPlayer->nCombo;


	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);
	

	if (nBossDirState == BOSS_DIR_NOTHING)
	{//ボス演出中でない時のみ実行

		// 時間経過でコンボ数をリセットする処理
		g_nComboResetTimer++;

		if (g_nComboResetTimer % 30 == 0)
		{
			g_nCombo_Alpha -= 0.1f;   //１秒に一回透明度を下げる

			if (g_nCombo_Alpha <= 0.0f)
			{//透明度が０以下になったら
				pPlayer->nCombo = 0;   //コンボ数をリセット
			}
		}
	}
	


	if (g_nCombo / 100 > 0)
	{//コンボ数が三桁

		g_posCombo = D3DXVECTOR3(950.0f - NUM_HARF_WIDTH * 7, 150.0f, 0.0f);
		nComboGidit = NUM_DIGIT_THREE;

	}
	else if (g_nCombo / 10 > 0)
	{//コンボ数が二桁

		g_posCombo = D3DXVECTOR3(960.0f - NUM_HARF_WIDTH * 8, 150.0f, 0.0f);
		nComboGidit = NUM_DIGIT_TWO;
	}
	else if (g_nCombo / 1 > 1)
	{//コンボ数が一桁であり2以上

		g_posCombo = D3DXVECTOR3(960.0f - NUM_HARF_WIDTH * 9, 150.0f, 0.0f);
		nComboGidit = NUM_DIGIT_ONE;

	}
	else
	{//コンボ数が０か１なら
		nComboGidit = 0;
	}


	

	for (int nCnt = 0; nCnt < NUM_PLACE_COMBO; nCnt++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posCombo.y - NUM_HARF_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posCombo.y - NUM_HARF_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posCombo.y + NUM_HARF_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posCombo.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posCombo.y + NUM_HARF_HEIGHT, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * (g_nCombo % (nDev * 10) / nDev), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (g_nCombo % (nDev * 10) / nDev + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * (g_nCombo % (nDev * 10) / nDev), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (g_nCombo % (nDev * 10) / nDev + 1), 1.0f);

		if (nComboGidit < (NUM_PLACE_COMBO - nCnt))
		{//
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);   //(r,g,b,a)
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else
		{
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);   //(r,g,b,a)
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_nCombo_Alpha);
		}

		nDev /= 10;        //割る数の桁を１減らす
		pVtx += 4;
	}
}


//----------------------------
// コンボの描画処理
//----------------------------
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCombo);

	for (int nCnt = 0; nCnt < NUM_PLACE_COMBO; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);      //描画を開始する頂点インデックス
	}
}


//-------------------------
//  コンボを増やす処理
//-------------------------
void AddCombo(void)
{
	Player* pPlayer = GetPlayer();

	pPlayer->nCombo++;
	g_nComboResetTimer = 0;
	g_nCombo_Alpha = 1.0f;
	
	
	if (pPlayer->nCombo > 999)
	{
		pPlayer->nCombo = 999;
	}

}

//-------------------------
//  透明度を取得
//-------------------------
float GetComboAlpha(void)
{
	if (g_nCombo > 1)
	{
		return g_nCombo_Alpha;
	}
	else
	{
		return 0.0f;
	}
}