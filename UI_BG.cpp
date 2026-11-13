//---------------------------------------
//
// ゲームUIの処理[GameUI.cpp]
// Author：清野蒼翔
//
//---------------------------------------

#include "UI_BG.h"
#include "game.h"  //マクロ使うためだけ


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureUI_BG[UI_BG_MAX] = {};  //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI_BG = NULL;       //頂点バッファへのポインタ

//-------------------------------------------
//    ＵＩの初期化処理
//-------------------------------------------
void InitUI_BG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();


	//テクスチャを読み込み
	for (int nCntUI_BG = 0; nCntUI_BG < UI_BG_MAX; nCntUI_BG++)
	{
		D3DXCreateTextureFromFile(pDevice,
			"data//TEXTURE//UI_BG.png",
			&g_apTextureUI_BG[nCntUI_BG]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UI_BG_MAX,                   //sizeof(VERTEX_2D) * [描画するのに必要な頂点数] * UI_MAX
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI_BG,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUI_BG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUI_BG = 0; nCntUI_BG < UI_BG_MAX; nCntUI_BG++)
	{
		switch (nCntUI_BG)   //各UIの初期化
		{
		case UI_LEFT_BG:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(GAME_DISPLAY_X, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);

			break;

		case UI_RIGHT_BG:
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffUI_BG->Unlock();

}

//-------------------------------------------
//  ＵＩの終了処理
//-------------------------------------------
void UninitUI_BG(void)
{
	//テクスチャの破棄
	for (int nCntUI_BG = 0; nCntUI_BG < UI_BG_MAX; nCntUI_BG++)
	{
		if (g_apTextureUI_BG[nCntUI_BG] != NULL)
		{
			g_apTextureUI_BG[nCntUI_BG]->Release();
			g_apTextureUI_BG[nCntUI_BG] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffUI_BG != NULL)
	{
		g_pVtxBuffUI_BG->Release();
		g_pVtxBuffUI_BG = NULL;
	}
}

//---------------------------
// ＵＩの更新処理
//---------------------------
void UpdateUI_BG(void)
{
	
}

//-------------------------------------------
//    ＵＩの描画処理
//-------------------------------------------
void DrawUI_BG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI_BG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntUI_BG = 0; nCntUI_BG < UI_BG_MAX; nCntUI_BG++)
	{

		//テクスチャの設定(０～３に合わせて設定する)
		pDevice->SetTexture(0, g_apTextureUI_BG[nCntUI_BG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI_BG * 4, 2);      //描画を開始する頂点インデックス

	}
}