//--------------------------------
//
// 弾の処理[bullet.cpp]
// Author：清野蒼翔
//
//--------------------------------

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "ActionEffect.h"
#include "enemy.h"
#include "Boss.h"
#include "effect.h"
#include "bomb.h"
#include "hitbox.h"   //プレイヤーの当たり判定範囲適用
#include "score.h"
#include "combo.h"
#include "sound.h"


//マクロ定義
#define MAX_BULLET (1024)  //弾の最大数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;       //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;  //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];                     //弾の情報


//-------------------
// 弾の初期化処理
//-------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//bullet000.png",
		&g_pTextureBullet);


	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 位置
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 移動量
		g_aBullet[nCntBullet].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 色
		g_aBullet[nCntBullet].nLife = 100;                               // 寿命(使用予定なし)
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;                  // 弾の種類
		g_aBullet[nCntBullet].kind = BULLETKIND_NORMAL;                  // 弾の挙動

		g_aBullet[nCntBullet].fBulletSize = 0.0f;                       // 弾の大きさ

		g_aBullet[nCntBullet].bGraze = false;                            // 敵の弾がグレイズした際にtrueにするので初期設定はfalse
		g_aBullet[nCntBullet].bUse = false;                              // 使用していない状態(false)にする
	}

	//頂点バッファへの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,      // sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 15, g_aBullet[nCntBullet].pos.y - 15, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 15, g_aBullet[nCntBullet].pos.y - 15, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 15, g_aBullet[nCntBullet].pos.y + 15, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 15, g_aBullet[nCntBullet].pos.y + 15, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//-----------------
// 弾の終了処理
//-----------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//--------------------
//  弾の更新処理
//--------------------
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている場合

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾

				//弾の軌跡にエフェクトを付ける
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 15, 9);

				//敵が被弾したかの確認
				CollisionEnemy(&g_aBullet[nCntBullet]);
				CollisionBoss(&g_aBullet[nCntBullet]);
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の弾

				Bomb* pBomb = GetBomb();     //ボムの情報を取得

				if (pBomb->bUse == true)
				{//ボムが使用状態である時のみ実行

					//敵の弾がボムの範囲内にあるかどうかの確認
					CollisionBomb(&g_aBullet[nCntBullet]);
				}

				//自機が被弾したかの確認
				CollisionPlayer(&g_aBullet[nCntBullet]);
			}



			//弾の位置の更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);


			//頂点カラー
			pVtx[0].col = g_aBullet[nCntBullet].col;   //(r,g,b,a)
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;

			if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT + g_aBullet[nCntBullet].fBulletSize
				|| g_aBullet[nCntBullet].pos.y <= -g_aBullet[nCntBullet].fBulletSize
				|| g_aBullet[nCntBullet].pos.x >= (SCREEN_WIDTH - GAME_DISPLAY_X) + g_aBullet[nCntBullet].fBulletSize
				|| g_aBullet[nCntBullet].pos.x <= GAME_DISPLAY_X - g_aBullet[nCntBullet].fBulletSize)
			{// 弾の位置が画面外に出た場合
				g_aBullet[nCntBullet].bUse = false;  //使用していない状態にする
			}

			if (g_aBullet[nCntBullet].nLife <= 0)
			{// 寿命が尽きた場合

				//爆発の設定
				SetExplosion(g_aBullet[nCntBullet].pos,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;  //使用していない状態にする
			
			}
		}
		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------
//  弾の描画処理
//-----------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4 , 2);      //描画を開始する頂点インデックス
		}
	}
}
//---------------------------------
//        弾のセット処理
//---------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, BULLETTYPE type, BULLETKIND kind)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += 4)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;   //プレイヤーの位置と同じ位置に弾のposを設定

			
			g_aBullet[nCntBullet].move = move;    // 移動量
			g_aBullet[nCntBullet].col = col;      // 色
			g_aBullet[nCntBullet].nLife = nLife;  // 寿命(多分使わん)
			g_aBullet[nCntBullet].type = type;	  // 自機の弾か敵の弾か
			g_aBullet[nCntBullet].kind = kind;	  // 弾の挙動
			g_aBullet[nCntBullet].fBulletSize = 10.0f;	  // 弾の大きさ
			g_aBullet[nCntBullet].bUse = true;    // 使用している状態にする
			g_aBullet[nCntBullet].bGraze = false; // 未グレイズ状態にする

			switch (kind)
			{
			case BULLETKIND_SONIC:
				g_aBullet[nCntBullet].move.x *= 1.5f;
				g_aBullet[nCntBullet].move.y *= 1.5f;
				break;
			case BULLETKIND_BIG:
				g_aBullet[nCntBullet].fBulletSize = 25.0f;
				break;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fBulletSize, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fBulletSize, 0.0f);

			//頂点カラーの反映
			pVtx[0].col = g_aBullet[nCntBullet].col;
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;

			break;  //ここでfor文を抜ける
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------
//  自機が被弾した際の処理
//-----------------------------
void CollisionPlayer(Bullet* pBullet)
{
	Player* pPlayer = GetPlayer();    //プレイヤー情報のポインタを取得

	if (pPlayer->PlayerState == PLAYERSTATE_DEATH || pPlayer->PlayerState == PLAYERSTATE_GAMEOVER)
	{//死亡状態かゲームオーバー状態なら

		return;    //この関数を抜ける
	}


	if (pBullet->pos.x >= pPlayer->pos.x - HITBOX_SIZE
		&& pBullet->pos.x <= pPlayer->pos.x + HITBOX_SIZE
		&& pBullet->pos.y >= pPlayer->pos.y - HITBOX_SIZE
		&& pBullet->pos.y <= pPlayer->pos.y + HITBOX_SIZE)
	{//プレイヤーに弾が当たった場合
		
		HitPlayer();             //プレイヤーのヒット処理

		if (pBullet->bGraze == true)
		{//グレイズした球で死んだ場合
			pPlayer->nGraze--;
		}
		pBullet->bUse = false;   //使用されていない状態にする
		
	}
	else
	{
		GrazePlayer(pBullet);
	}
}

//-----------------------------------------
//    敵の弾をグレイズしたかの確認処理
//-----------------------------------------
void GrazePlayer(Bullet* pBullet)
{
	Player* pPlayer = GetPlayer();    //プレイヤー情報のポインタを取得

	if (pPlayer->PlayerState == PLAYERSTATE_DEATH 
		|| pPlayer->PlayerState == PLAYERSTATE_GAMEOVER 
		|| pPlayer->PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{//死亡状態かゲームオーバー状態、またはリスポーン後の無敵時間中なら

		return;    //この関数を抜ける
	}



	float dx = pBullet->pos.x - pPlayer->pos.x;
	float dy = pBullet->pos.y - pPlayer->pos.y;
	float fDistanceSq = (dx * dx) + (dy * dy);              // 距離の2乗
	float fRadiusSq = GRAZE_RADIUS * GRAZE_RADIUS;          // グレイズ判定の半径の2乗


	if (fRadiusSq >= fDistanceSq && pBullet->bGraze == false)  //「距離の二乗が半径の二乗以下」 かつ 「その弾が一度もグレイズされていない」場合 
	{//以下グレイズ成功時の処理


		PlaySound(SOUND_LABEL_SE_ENTER);
		PlaySound(SOUND_LABEL_SE_GRAZE);

		pPlayer->nCombo++;
		pPlayer->nGraze++;

		SetActionEffect(pPlayer->pos, ACTION_EFFECT_TEX_C, 40.0f);  //グレイズエフェクト
		SetEffect(pPlayer->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30.0f, 12);

		if (pPlayer->nBurstMater <= MAX_BURST_MATER && pPlayer->bBurstMode == false)
		{
			if (pPlayer->nBurstMater >= MAX_BURST_MATER - 5)
			{//ゲージ増加によって上限を超えた場合リセットする処理
				pPlayer->nBurstMater = MAX_BURST_MATER;
			}
			else
			{
				pPlayer->nBurstMater += 5;  //バーストゲージをチャージ
			}
		}

		AddScore(30000);
		pBullet->bGraze = true;
	}
}

//------------------------------
// 敵に弾が当たった場合の処理
//------------------------------
void CollisionEnemy(Bullet* pBullet)
{
	Enemy* pEnemy = GetEnemy();    //敵の取得
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている場合
			if (pBullet->pos.x >= pEnemy->pos.x - ENEMY_SIZE
				&& pBullet->pos.x <= pEnemy->pos.x + ENEMY_SIZE
				&& pBullet->pos.y >= pEnemy->pos.y - ENEMY_SIZE
				&& pBullet->pos.y <= pEnemy->pos.y + ENEMY_SIZE)
			{//敵に弾が当たった場合

				//敵のヒット処理
				HitEnemy(nCntEnemy, 1);
				pBullet->bUse = false;   //使用されていない状態にする

			}
		}
	}
}
//--------------------------------
//   ボスとのあたり判定処理
//--------------------------------
void CollisionBoss(Bullet* pBullet)
{
	BOSS* pBoss = GetBoss();  //ボス情報を取得
	
	if (pBoss->bUse == true)
	{//ボスが使用されている場合
		if (pBullet->pos.x >= pBoss->pos.x - BOSS_SIZE_X
			&& pBullet->pos.x <= pBoss->pos.x + BOSS_SIZE_X
			&& pBullet->pos.y >= pBoss->pos.y - BOSS_SIZE_Y - 30.0f
			&& pBullet->pos.y <= pBoss->pos.y + BOSS_SIZE_Y - 30.0f)
		{//ボスに弾が当たった場合

			//敵のヒット処理
			HitBoss(1, pBullet->pos);
			pBullet->bUse = false;   //使用されていない状態にする
			SetActionEffect(pBullet->pos, ACTION_EFFECT_TEX_A, 20.0f);

		}
	}
}




//----------------------------------------------
// ボムの範囲内に敵の弾があった場合の処理
//----------------------------------------------
void CollisionBomb(Bullet* pBullet)
{
	int nCntSound = 0;  // 音ならしすぎないようにする為の変数
	Bomb* pBomb = GetBomb();    //ボム情報のポインタを取得
	Player* pPlayer = GetPlayer();  //プレイヤー情報を取得

	if (pBomb->nLife < 5)
	{//消えかけの場合は処理しない(違和感あるから)

		return;
	}

	float dx = pBullet->pos.x - pBomb->pos.x;
	float dy = pBullet->pos.y - pBomb->pos.y;
	float fDistanceSq = (dx * dx) + (dy * dy);              // 距離の2乗 (Sqは２乗ってgoogle先生が言ってた)
	float fRadiusSq = pBomb->fRadius * pBomb->fRadius;      // ボムの半径の2乗

	if (fRadiusSq  >= fDistanceSq)  //「距離の二乗」が「半径の二乗」以下の場合 
	{
		pBullet->bUse = false;   //使用されていない状態にする
		SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		AddCombo();   //コンボ数加算

		if (nCntSound <= 10)
		{
			nCntSound++;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);  // 爆発音
		}

	}
}


//---------------------------
//  全ての弾を消す処理
//---------------------------
void DeleteBullet(void)
{
	int nCntSound = 0;  // 音ならしすぎないようにする為の変数

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].bUse = false;
			SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (nCntSound <= 10)
	{
		nCntSound++;
		PlaySound(SOUND_LABEL_SE_EXPLOSION);  // 爆発音
	}
}