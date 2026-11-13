//---------------------------------------
//
// シールドの処理[shield.cpp]
// Author：清野蒼翔
//
//---------------------------------------

#include "main.h"
#include "player.h"
#include "shield.h"

//マクロ定義
#define SHIELD_TEX_SIZE (30.0f)   //バリアのテクスチャの半径

//シールド構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;    //位置
	D3DXCOLOR col;      //色

}Shield;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureShield = NULL;       //シールドのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShield = NULL;  //シールドの頂点バッファへのポインタ

Shield g_shield;    //シールドの情報

//----------------------------------------
// シールドの初期化処理
//----------------------------------------
void InitShield(void)
{
	LPDIRECT3DDEVICE9 pDevice;  //デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//Ult_Shield.png",
		&g_pTextureShield);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShield,
		NULL);

	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報処理
    
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_shield.pos.x - SHIELD_TEX_SIZE, g_shield.pos.y - SHIELD_TEX_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_shield.pos.x + SHIELD_TEX_SIZE, g_shield.pos.y - SHIELD_TEX_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_shield.pos.x - SHIELD_TEX_SIZE, g_shield.pos.y + SHIELD_TEX_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_shield.pos.x + SHIELD_TEX_SIZE, g_shield.pos.y + SHIELD_TEX_SIZE, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);    //１÷テクスチャの分割数
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffShield->Unlock();
}

//----------------------------------------
// シールドの終了処理
//----------------------------------------
void UninitShield(void)
{
	//テクスチャの破棄
	if (g_pTextureShield != NULL)
	{
		g_pTextureShield->Release();
		g_pTextureShield = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShield != NULL)
	{
		g_pVtxBuffShield->Release();
		g_pVtxBuffShield = NULL;
	}
}

//----------------------------------------
// シールドの更新処理
//----------------------------------------
void UpdateShield(void)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->bBurstMode == true && pPlayer->bBurstShield == true)
	{//バースト状態かつウルトシールドがまだ残っている場合

		VERTEX_2D* pVtx;  //頂点情報へのポインタ

	    //頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShield->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の更新
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - SHIELD_TEX_SIZE, pPlayer->pos.y - SHIELD_TEX_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + SHIELD_TEX_SIZE, pPlayer->pos.y - SHIELD_TEX_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - SHIELD_TEX_SIZE, pPlayer->pos.y + SHIELD_TEX_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + SHIELD_TEX_SIZE, pPlayer->pos.y + SHIELD_TEX_SIZE, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		//頂点バッファをアンロックする
		g_pVtxBuffShield->Unlock();

	}//要するに壊れたら描画しないってわけだ
}

//----------------------------------------
// シールドの描画処理
//----------------------------------------
void DrawShield(void)
{
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShield, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShield);

	if (pPlayer->bBurstShield == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}





