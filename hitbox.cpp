//----------------------------------------------------------
//
// プレイヤーの当たり判定を表示する処理[hitbox.cpp]
// Author：清野蒼翔
//
//----------------------------------------------------------

#include "main.h"
#include "player.h"
#include "hitbox.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureHitBox = NULL;  //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHitBox = NULL;       //頂点バッファへのポインタ

//-------------------------------------------
//    プレイヤーの当たり判定の初期化処理
//-------------------------------------------
void InitHitBox(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//HitBox4.png",
		&g_apTextureHitBox);

	// 情報の初期化
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHitBox,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHitBox->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);   //(r,g,b,a)
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	g_pVtxBuffHitBox->Unlock();

}

//-------------------------------------------
//    プレイヤーの当たり判定Iの終了処理
//-------------------------------------------
void UninitHitBox(void)
{
	//テクスチャの破棄
	if (g_apTextureHitBox != NULL)
	{
		g_apTextureHitBox->Release();
		g_apTextureHitBox = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHitBox != NULL)
	{
		g_pVtxBuffHitBox->Release();
		g_pVtxBuffHitBox = NULL;
	}
}

//--------------------------------------
// プレイヤーの当たり判定の更新処理
//--------------------------------------
void UpdateHitBox(void)
{
	Player* pPlayer = GetPlayer();


	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHitBox->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - HITBOX_SIZE, pPlayer->pos.y - HITBOX_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + HITBOX_SIZE, pPlayer->pos.y - HITBOX_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - HITBOX_SIZE, pPlayer->pos.y + HITBOX_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + HITBOX_SIZE, pPlayer->pos.y + HITBOX_SIZE, 0.0f);
	
	//頂点バッファをアンロックする
	g_pVtxBuffHitBox->Unlock();
}

//-------------------------------------------
//   プレイヤーの当たり判定の描画処理
//-------------------------------------------
void DrawHitBox(void)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->PlayerState == PLAYERSTATE_DEATH || pPlayer->PlayerState == PLAYERSTATE_GAMEOVER)
	{//もしプレイヤーが死亡状態なら即座にこの関数を抜ける(描写をやめる)

		return;
	}


	if (pPlayer->PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{
		// ４フレ毎に描画をスキップしてみる　　　　あとこれヒットボックスにも似たようなの作ってあげてね...このままだとヒットボックスが点滅しない
		if ((pPlayer->nSpawnImmuneTimer / 4) % 2 == 0)
		{
			return;
		}
	}


	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHitBox, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureHitBox);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);      //描画を開始する頂点インデックス

}