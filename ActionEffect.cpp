//----------------------------------------------------------
//
// 派手さを出すためののエフェクト処理[ActionEffect.cpp]
// Author：清野蒼翔
//
//----------------------------------------------------------

#include "score.h"
#include "player.h"
#include "ActionEffect.h"
#include "game.h"
#include "sound.h"


//マクロ定義
#define TEX_PATTERN_A (8)  // EnergyBallの最大パターン数
#define TEX_PATTERN_B (6)  // BombExplosionの最大パターン数
#define TEX_PATTERN_C (8)  // GrazeEffectの最大パターン数
#define TEX_PATTERN_D (6)  // Lightning01の最大パターン数
#define TEX_PATTERN_E (2)  // BurstMaxIconの最大パターン数
#define MAX_ACTION_EFFECT (256)


//プロトタイプ宣言


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureActionEffect[ACTION_EFFECT_TEX_MAX] = {};  // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffActionEffect = NULL;                   // 頂点バッファへのポインタ
ACTION_EFFECT g_aActionEffect[MAX_ACTION_EFFECT];                        // エフェクトの情報

const char* g_aActionEffectTexture_Path[ACTION_EFFECT_TEX_MAX] =
{
	"data//TEXTURE//EnergyBall.png",
	"data//TEXTURE//BombExplosion.png",
	"data//TEXTURE//GrazeEffect.png",
	"data//TEXTURE//Lightning01.png",
	"data//TEXTURE//BurstMaxIcon.png",


};                 //エフェクトテクスチャ


//-------------------------------------------
//    エフェクトの初期化処理
//-------------------------------------------
void InitActionEffect(void)
{
	int nCntActionEffect;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < ACTION_EFFECT_TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aActionEffectTexture_Path[nCnt],
			&g_apTextureActionEffect[nCnt]);
	}

	// エフェクトの情報の初期化
	for (nCntActionEffect = 0; nCntActionEffect < MAX_ACTION_EFFECT; nCntActionEffect++)
	{
		g_aActionEffect[nCntActionEffect].nType = 0;              // 種類
		g_aActionEffect[nCntActionEffect].nCounterAnim = 0;       // アニメーションカウンター
		g_aActionEffect[nCntActionEffect].nNowPatternAnim = 0;    // 現在のパターン数
		g_aActionEffect[nCntActionEffect].nMaxPatternAnim = 0;    // テクスチャパターンの最大数
		g_aActionEffect[nCntActionEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 位置
		g_aActionEffect[nCntActionEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 角度(要らん気する。使わんかったら消そう)
		g_aActionEffect[nCntActionEffect].Tex_Hurfsize_x = 0.0f;              // テクスチャのx半径
		g_aActionEffect[nCntActionEffect].Tex_Hurfsize_y = 0.0f;              // テクスチャのy半径
		g_aActionEffect[nCntActionEffect].nMoveTexTime = 4;       // テクスチャパターンを進める間隔
		g_aActionEffect[nCntActionEffect].bUse = false;           // 使用されているか否か
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ACTION_EFFECT,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_ActionEffect
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffActionEffect,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffActionEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntActionEffect = 0; nCntActionEffect < MAX_ACTION_EFFECT; nCntActionEffect++)
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
	g_pVtxBuffActionEffect->Unlock();




}

//--------------------------------
//     エフェクトの終了処理
//--------------------------------
void UninitActionEffect(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < ACTION_EFFECT_TEX_MAX; nCnt++)
	{
		if (g_apTextureActionEffect[nCnt] != NULL)
		{
			g_apTextureActionEffect[nCnt]->Release();
			g_apTextureActionEffect[nCnt] = NULL;
		}
	}


	// 頂点バッファの破棄
	if (g_pVtxBuffActionEffect != NULL)
	{
		g_pVtxBuffActionEffect->Release();
		g_pVtxBuffActionEffect = NULL;
	}
}


//---------------------------
//  エフェクトの更新処理
//---------------------------
void UpdateActionEffect(void)
{
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffActionEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntActionEffect = 0; nCntActionEffect < MAX_ACTION_EFFECT; nCntActionEffect++, pVtx += 4)
	{
		if (g_aActionEffect[nCntActionEffect].bUse == true)
		{//そのエフェクトが使われていたら

			switch (g_aActionEffect[nCntActionEffect].nType)
			{
			case ACTION_EFFECT_TEX_B:
				// ボムエフェクトがゲーム内の枠を超えている場合に表示しないようにする処理
				
				if (g_aActionEffect[nCntActionEffect].pos.y >= SCREEN_HEIGHT
					|| g_aActionEffect[nCntActionEffect].pos.y <= 0.0f
					|| g_aActionEffect[nCntActionEffect].pos.x >= SCREEN_WIDTH - GAME_DISPLAY_X
					|| g_aActionEffect[nCntActionEffect].pos.x <= GAME_DISPLAY_X)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				else
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				break;
				

			case ACTION_EFFECT_TEX_C:
				// グレイズエフェクトがプレイヤーの上に常に出るようにする処理
				g_aActionEffect[nCntActionEffect].pos = pPlayer->pos;
				// 頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aActionEffect[nCntActionEffect].pos.x - g_aActionEffect[nCntActionEffect].Tex_Hurfsize_x, g_aActionEffect[nCntActionEffect].pos.y - g_aActionEffect[nCntActionEffect].Tex_Hurfsize_y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aActionEffect[nCntActionEffect].pos.x + g_aActionEffect[nCntActionEffect].Tex_Hurfsize_x, g_aActionEffect[nCntActionEffect].pos.y - g_aActionEffect[nCntActionEffect].Tex_Hurfsize_y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aActionEffect[nCntActionEffect].pos.x - g_aActionEffect[nCntActionEffect].Tex_Hurfsize_x, g_aActionEffect[nCntActionEffect].pos.y + g_aActionEffect[nCntActionEffect].Tex_Hurfsize_y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aActionEffect[nCntActionEffect].pos.x + g_aActionEffect[nCntActionEffect].Tex_Hurfsize_x, g_aActionEffect[nCntActionEffect].pos.y + g_aActionEffect[nCntActionEffect].Tex_Hurfsize_y, 0.0f);
				break;
			}


			//カウンタを進めて、パターンNo.を更新する
			g_aActionEffect[nCntActionEffect].nCounterAnim++;

			if ((g_aActionEffect[nCntActionEffect].nCounterAnim % g_aActionEffect[nCntActionEffect].nMoveTexTime) == 0)
			{// 一定時間経過

				g_aActionEffect[nCntActionEffect].nCounterAnim = 0;
				g_aActionEffect[nCntActionEffect].nNowPatternAnim++;

				if (g_aActionEffect[nCntActionEffect].nNowPatternAnim >= g_aActionEffect[nCntActionEffect].nMaxPatternAnim)
				{//総パターン数を超えた場合
					g_aActionEffect[nCntActionEffect].bUse = false;   //使用していない状態にする

					//----------------------------------------------------------------------------
					//         条件を満たすなら1ループで終了せず何度も実行したいテクスチャ
					//----------------------------------------------------------------------------
					if (g_aActionEffect[nCntActionEffect].nType == ACTION_EFFECT_TEX_D && (pPlayer->nBurstMater >= MAX_BURST_MATER || pPlayer->bBurstMode == true))
					{// 雷エフェクト

						g_aActionEffect[nCntActionEffect].nCounterAnim = 0;             // アニメーションカウンター
						g_aActionEffect[nCntActionEffect].nNowPatternAnim = 0;          // 現在のテクスチャパターン
						g_aActionEffect[nCntActionEffect].bUse = true;                  // 使用状態にする

					}
					if (g_aActionEffect[nCntActionEffect].nType == ACTION_EFFECT_TEX_E && pPlayer->nBurstMater >= MAX_BURST_MATER)
					{// バーストゲージアイコン

						g_aActionEffect[nCntActionEffect].nCounterAnim = 0;             // アニメーションカウンター
						g_aActionEffect[nCntActionEffect].nNowPatternAnim = 0;          // 現在のテクスチャパターン
						g_aActionEffect[nCntActionEffect].bUse = true;                  // 使用状態にする

					}
				}
				
				if (g_aActionEffect[nCntActionEffect].nNowPatternAnim < g_aActionEffect[nCntActionEffect].nMaxPatternAnim)
				{//総パターン数に満たない場合

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aActionEffect[nCntActionEffect].nNowPatternAnim * (1.0f / g_aActionEffect[nCntActionEffect].nMaxPatternAnim), 0.0f);          //　x,yに該当する値はテクスチャを変えた際に必ず変更すること(0.125にしてるが実際に作るときは1.0/パターン数とかにしてもいいかも)
					pVtx[1].tex = D3DXVECTOR2((g_aActionEffect[nCntActionEffect].nNowPatternAnim + 1) * (1.0f / g_aActionEffect[nCntActionEffect].nMaxPatternAnim), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aActionEffect[nCntActionEffect].nNowPatternAnim * (1.0f / g_aActionEffect[nCntActionEffect].nMaxPatternAnim), 1.0f);
					pVtx[3].tex = D3DXVECTOR2((g_aActionEffect[nCntActionEffect].nNowPatternAnim + 1) * (1.0f / g_aActionEffect[nCntActionEffect].nMaxPatternAnim), 1.0f);
				}
			}	
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffActionEffect->Unlock();

}


//---------------------------
//  エフェクトの描画処理
//---------------------------
void DrawActionEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffActionEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntActionEffect = 0; nCntActionEffect < MAX_ACTION_EFFECT; nCntActionEffect++)
	{
		if (g_aActionEffect[nCntActionEffect].bUse == true)
		{//敵が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureActionEffect[g_aActionEffect[nCntActionEffect].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntActionEffect * 4, 2);      //描画を開始する頂点インデックス
		}
	}
}


//------------------------------
//  エフェクトのセット処理
//------------------------------
void SetActionEffect(D3DXVECTOR3 pos, int nType, float fsize)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffActionEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ACTION_EFFECT; nCnt++)
	{
		if (g_aActionEffect[nCnt].bUse == false)
		{//敵が使用されていない場合

			//敵の情報の設定    g_aActionEffect
			g_aActionEffect[nCnt].nCounterAnim = 0;             // アニメーションカウンター
			g_aActionEffect[nCnt].nNowPatternAnim = 0;          // 現在のテクスチャパターン
			g_aActionEffect[nCnt].pos = pos;                    // 位置を設定
			g_aActionEffect[nCnt].nType = nType;                // 指定された種類のエフェクトを生成
			g_aActionEffect[nCnt].Tex_Hurfsize_x = fsize;       // テクスチャの半径
			g_aActionEffect[nCnt].Tex_Hurfsize_y = fsize;       // テクスチャの半径
			g_aActionEffect[nCnt].nMoveTexTime = 4;             // テクスチャパターンを進める間隔
			g_aActionEffect[nCnt].bUse = true;                  // 使用している状態に


			switch (nType)
			{

			case ACTION_EFFECT_TEX_A:
				g_aActionEffect[nCnt].nMaxPatternAnim = TEX_PATTERN_A;  //テクスチャパターンの最大数
				break;

			case ACTION_EFFECT_TEX_B:
				g_aActionEffect[nCnt].nMaxPatternAnim = TEX_PATTERN_B;

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case ACTION_EFFECT_TEX_C:
				g_aActionEffect[nCnt].nMaxPatternAnim = TEX_PATTERN_C;
				g_aActionEffect[nCnt].nMoveTexTime = 3;             // テクスチャパターンを進める間隔

				break;

			case ACTION_EFFECT_TEX_D:
				g_aActionEffect[nCnt].nMaxPatternAnim = TEX_PATTERN_D;
				g_aActionEffect[nCnt].Tex_Hurfsize_y = 30.0f;         // テクスチャのy半径

				break;

			case ACTION_EFFECT_TEX_E:
				g_aActionEffect[nCnt].nMaxPatternAnim = TEX_PATTERN_E;
				g_aActionEffect[nCnt].nMoveTexTime = 24;             // テクスチャパターンを進める間隔


				break;

			}




			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aActionEffect[nCnt].pos.x - g_aActionEffect[nCnt].Tex_Hurfsize_x, g_aActionEffect[nCnt].pos.y - g_aActionEffect[nCnt].Tex_Hurfsize_y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aActionEffect[nCnt].pos.x + g_aActionEffect[nCnt].Tex_Hurfsize_x, g_aActionEffect[nCnt].pos.y - g_aActionEffect[nCnt].Tex_Hurfsize_y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aActionEffect[nCnt].pos.x - g_aActionEffect[nCnt].Tex_Hurfsize_x, g_aActionEffect[nCnt].pos.y + g_aActionEffect[nCnt].Tex_Hurfsize_y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aActionEffect[nCnt].pos.x + g_aActionEffect[nCnt].Tex_Hurfsize_x, g_aActionEffect[nCnt].pos.y + g_aActionEffect[nCnt].Tex_Hurfsize_y, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f / g_aActionEffect[nCnt].nMaxPatternAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f / g_aActionEffect[nCnt].nMaxPatternAnim, 1.0f);

			break;
		}
		pVtx += 4;        //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffActionEffect->Unlock();
}