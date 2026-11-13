//----------------------------------------------------------
//
// ボムの処理[bomb.cpp]
// Author：清野蒼翔
//
//----------------------------------------------------------

#include "main.h"
#include "bomb.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "ActionEffect.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBomb = NULL;           //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBomb = NULL;       //頂点バッファへのポインタ
Bomb g_Bomb;                                         //ボムの情報


//-------------------------------------------
//    ボムの初期化処理
//-------------------------------------------
void InitBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//BombEffect2.png",
		&g_apTextureBomb);

	//情報の初期化
	g_Bomb.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 位置
	g_Bomb.fRadius = 0.0f;                          // 現在の半径
	g_Bomb.fMaxRadius = 400.0f;                     // 爆発半径の最大値
	g_Bomb.nAttack = 5;                             // 攻撃力
	g_Bomb.nLife = 40;                              // 寿命
	g_Bomb.bUse = false;                            // 使用中であるかどうかのフラグ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBomb,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);   //(r,g,b,a)
	pVtx[1].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.7f, 0.3f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBomb->Unlock();

}

//-------------------------------------------
//    ボムの終了処理
//-------------------------------------------
void UninitBomb(void)
{
	//テクスチャの破棄
	if (g_apTextureBomb != NULL)
	{
		g_apTextureBomb->Release();
		g_apTextureBomb = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBomb != NULL)
	{
		g_pVtxBuffBomb->Release();
		g_pVtxBuffBomb = NULL;
	}
}

//--------------------------------------
// ボムの更新処理
//--------------------------------------
void UpdateBomb(void)
{
	Player* pPlayer = GetPlayer();


	//ちらつかせるエフェクト効果の処理
	if (g_Bomb.bUse == true)
	{//ボムが使用されているなら実行


		g_Bomb.nLife--;  // ボムの寿命を減らす
		g_Bomb.fRadius += (g_Bomb.fMaxRadius - g_Bomb.fRadius) * 0.1f;   // ボムを徐々にでかくする処理
		
		for (int nCntEffect = 0; nCntEffect < 4; nCntEffect++)
		{//複数個のエフェクトをちらつかせる

			//ランダムな大きさに設定
			float EffectRadius = (float)(rand() % 20 + 10);

			//ランダムに位置をずらす　　　　　　　    範囲
			float EffectSetX = (float)(rand() % 551 - 275);
			float EffectSetY = (float)(rand() % 631 - 315);

			D3DXVECTOR3 effectPos = g_Bomb.pos;   // エフェクトの初期位置
			effectPos.x += EffectSetX;     //  x 座標を調整
			effectPos.y += EffectSetY;     //  y 座標を調整

			if (nCntEffect % 2 == 0)
			{
				// エフェクト
				SetEffect(effectPos, D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f), EffectRadius, 12);
			}
			else
			{
				//爆発
				/*
				SetExplosion(effectPos, D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f));
				*/

				SetActionEffect(effectPos, ACTION_EFFECT_TEX_B, EffectRadius + 4.0f);

			}
			
		}

		//ボムの寿命が切れた場合の処理
		if (g_Bomb.nLife <= 0)
		{
			g_Bomb.bUse = false;
			return;
		}

		//透明度を下げていく
		float alpha = 1.0f;
		if (g_Bomb.nLife <= 10)
		{
			alpha = (float)g_Bomb.nLife / 10.0f;
		}
		

		// ボム本体の輪っかのエフェクトの処理
		
		//頂点情報の設定
		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffBomb->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Bomb.pos.x - g_Bomb.fRadius, g_Bomb.pos.y - g_Bomb.fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Bomb.pos.x + g_Bomb.fRadius, g_Bomb.pos.y - g_Bomb.fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Bomb.pos.x - g_Bomb.fRadius, g_Bomb.pos.y + g_Bomb.fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Bomb.pos.x + g_Bomb.fRadius, g_Bomb.pos.y + g_Bomb.fRadius, 0.0f);
		
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, alpha);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, alpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, alpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, alpha);

		//頂点バッファをアンロックする
		g_pVtxBuffBomb->Unlock();

	}


	
}

//-------------------------------------------
//   ボムの描画処理
//-------------------------------------------
void DrawBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBomb, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBomb);

	if (g_Bomb.bUse == true)
	{//使用されているなら描画

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);      //描画を開始する頂点インデックス
	}
	

}
//---------------------------------
//   ボムの情報を取得する処理
//---------------------------------
Bomb* GetBomb(void)
{
	return &g_Bomb;
}



//-------------------------
//   ボムのセット処理
//-------------------------
void SetBomb(D3DXVECTOR3 pos)
{
	

	// ボムを使用中でなければ実行(これはあとでプレイヤーの方に入れといて良いかも？どっちがいいとかあんのかな)
	if (g_Bomb.bUse == false)
	{
		g_Bomb.bUse = true;                 // 使用フラグをON
		g_Bomb.pos = pos;                   // 発動位置をプレイヤーの現在地に設定
		g_Bomb.fMaxRadius = 600.0f;                     // 爆発半径の最大値
		g_Bomb.fRadius = 2.0f;             // 初期サイズ
		g_Bomb.nLife = 40;                  // 爆発の寿命	

		
		
	}
	else
	{//使用中なら
		
		//エラー音鳴らす処理(後で追加する事)
		PlaySound(SOUND_LABEL_SE_ERROR);
	}
}