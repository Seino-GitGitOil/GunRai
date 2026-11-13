//---------------------------------------
//
// 残りライフのUIを表示する処理[lifeUI.cpp]
// Author：清野蒼翔
//
//---------------------------------------
#include "main.h"
#include "player.h"
#include "lifeUI.h"



//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;       //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;  //頂点バッファへのポインタ


//----------------------
//  ライフの初期化処理
//----------------------
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//Life.png",
		&g_pTextureLife);

	//頂点バッファへの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffLife->Unlock();
}
//----------------------
//  ライフの終了処理
//----------------------
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}
//----------------------
//  ライフの更新処理
//----------------------
void UpdateLife(void)
{
	Player* pPlayer = GetPlayer();

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDown = pPlayer->nLifeStock; nCntDown > 0; nCntDown--)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(1015.0f + (40 * nCntDown), 370.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1015.0f + (40 * (nCntDown + 1)), 370.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1005.0f + (40 * nCntDown), 410.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1005.0f + (40 * (nCntDown + 1)), 410.0f, 0.0f);

		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}
//----------------------
//  ライフの描画処理
//----------------------
void DrawLife(void)
{
	Player* pPlayer = GetPlayer();    //プレイヤー情報のポインタを取得

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < pPlayer->nLifeStock; nCntLife++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);      //描画を開始する頂点インデックス
	}
}