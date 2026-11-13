//-------------------------------------------
//
// エフェクトの処理[effect.cpp]
// Author：清野蒼翔
//
//-------------------------------------------

#include "effect.h"


//マクロ定義
#define MAX_EFFECT (4096)   //エフェクトの最大数

//エフェクト構造体
typedef struct
{
	D3DXVECTOR3 pos;   //位置
	D3DXCOLOR col;     //色
	float fRadius;     //半径
	int nLife;         //寿命(表示時間)
	int nLifeMax;      //寿命のセット時点での初期値
	bool bUse;         //使用しているかどうか
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;        //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;   //頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];                      //エフェクトの情報

//--------------------------
// エフェクトの初期化処理
//--------------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//effect000.jpg",
		&g_pTextureEffect);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //位置
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);     //色
		g_aEffect[nCntEffect].fRadius = 10;                                //初期半径のサイズ
		g_aEffect[nCntEffect].nLife = 10;                                  //体力
		g_aEffect[nCntEffect].bUse = false;                                //使用されているかどうか
	}

	//頂点バッファへの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_EFFECT
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 10, g_aEffect[nCntEffect].pos.y - 10, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 10, g_aEffect[nCntEffect].pos.y - 10, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - 10, g_aEffect[nCntEffect].pos.y + 10, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + 10, g_aEffect[nCntEffect].pos.y + 10, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;   //(r,g,b,a)
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;
		

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
//---------------------------
//  エフェクトの終了処理
//---------------------------
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//--------------------------
//  エフェクトの更新処理
//--------------------------
void UpdateEffect(void)
{
	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//弾が使用されている場合

			g_aEffect[nCntEffect].nLife--;    //エフェクトの寿命を減少させる


			if (g_aEffect[nCntEffect].nLife <= 0)
			{//寿命が尽きた場合
				g_aEffect[nCntEffect].bUse = false;
			}
			else if (g_aEffect[nCntEffect].nLife > 0)
			{//寿命が残っている場合
				/*
				g_aEffect[nCntEffect].fRadius = g_aEffect[nCntEffect].fRadius - g_aEffect[nCntEffect].nLife;  //半径を狭める
				*/
				g_aEffect[nCntEffect].fRadius -= (g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLifeMax);
				//g_aEffect[nCntEffect].fRadius -= 0.1f;

				//頂点カラーの透明度を下げる処理
				g_aEffect[nCntEffect].col.a -= 1.0f / g_aEffect[nCntEffect].nLife;


			    //頂点カラー
				pVtx[0].col = g_aEffect[nCntEffect].col;   //(r,g,b,a)
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;


				//頂点座標の更新
				pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			}
		}
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
//-------------------------
// エフェクトの描画処理
//-------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);      //描画を開始する頂点インデックス
		}
	}
	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------
// 弾のエフェクトのセット処理
//--------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nLifeMax = nLife;    //エフェクトの寿命を減らすときの計算式に使用
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
	}
}