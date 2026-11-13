//---------------------------------------
//
// ボスの処理 [Boss.cpp]
// Author：清野蒼翔
//
// NOTE:鈴木哲汰
//---------------------------------------
#include "boss.h"
#include "bullet.h"
#include "ActionEffect.h"
#include "explosion.h"
#include "score.h"
#include "combo.h"
#include "player.h"
#include "BossDirection.h"
#include "bomb.h"
#include "sound.h"

// プロトタイプ宣言
void BossActionPattern(void);  // ボスの行動パターン

// グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBoss[BOSS_MAX] = {};   // ボステクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;  // 頂点バッファへのポインタ
BOSS g_Boss;                 // ボスの情報

const char* g_aBossTexture_Path[BOSS_MAX] =
{
	"data//TEXTURE//Boss00.png",
	"data//TEXTURE//Boss01.png",
	"data//TEXTURE//Boss02.png"

};                 //ボステクスチャ

//-----------------------
// ボスの初期化処理
//---------------------
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャ読込
	for (int nCnt = 0; nCnt < BOSS_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_aBossTexture_Path[nCnt],
			&g_apTextureBoss[nCnt]);
	}

	// ボスデータの初期化
	g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.nLife = BOSS_MAX_HP;
	g_Boss.nMaxLife = BOSS_MAX_HP;
	g_Boss.state = BOSSSTATE_SPAWN;
	g_Boss.phase = BOSSPHASE_1;
	g_Boss.nCounterState = 0;

	g_Boss.nActionCounter = 0;
	g_Boss.nBulletCounter = 0;
	
	g_Boss.nType = BOSS_TYPE_A;
	g_Boss.bUse = false;

	//対角線と角度
	g_Boss.fLength = sqrtf((BOSS_SIZE_X * 2.0f) * (BOSS_SIZE_X * 2.0f) + (BOSS_SIZE_Y * 2.0f) * (BOSS_SIZE_Y * 2.0f)) * 0.5f;
	g_Boss.fAngle = atan2f(BOSS_SIZE_Y * 2.0f, BOSS_SIZE_X * 2.0f);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL);


	// 頂点情報の初期化
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック処理
	g_pVtxBuffBoss->Unlock();
}

//--------------------------
// ボスの終了処理
//--------------------------
void UninitBoss(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < BOSS_MAX; nCnt++)
	{
		if (g_apTextureBoss[nCnt] != NULL)
		{
			g_apTextureBoss[nCnt]->Release();
			g_apTextureBoss[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//-------------------------
//    ボスの更新処理
//-------------------------
void UpdateBoss(void)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(g_Boss.pos.x - BOSS_SIZE_X, g_Boss.pos.y - BOSS_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Boss.pos.x + BOSS_SIZE_X, g_Boss.pos.y - BOSS_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Boss.pos.x - BOSS_SIZE_X, g_Boss.pos.y + BOSS_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Boss.pos.x + BOSS_SIZE_X, g_Boss.pos.y + BOSS_SIZE_Y, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();


	if (g_Boss.state == BOSSSTATE_NORMAL)
	{
		g_Boss.nActionCounter++;    // 一定のタイミングで行いたい行動がある場合、それを定めるカウンター

		float fBasicY = BOSS_SIZE_Y + 20.0f; // ボスの基本Y座標（ボスに合わせて調整してくだしあ）
		g_Boss.pos.y = fBasicY + sinf(g_Boss.nActionCounter * 0.05f) * 10.0f;

		BossActionPattern();
		CollisionBomb_Boss();
	}
}

//-----------------------
//    ボスの描画処理
//-----------------------
void DrawBoss(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Boss.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBoss[g_Boss.nType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}

//-------------------------
//    ボスのセット処理
//-------------------------
void SetBoss(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	

	// ボスデータの初期化
	g_Boss.pos = pos;
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.nLife = BOSS_MAX_HP;
	g_Boss.nMaxLife = BOSS_MAX_HP;
	g_Boss.state = BOSSSTATE_SPAWN;  // 出現前の状態
	g_Boss.phase = BOSSPHASE_1;
	g_Boss.nCounterState = 0;             // 点滅カウンター
	g_Boss.bSufferBomb = false;           
 
	g_Boss.nActionCounter = 0;            // 行動を指示するカウンター変数
	g_Boss.nBulletCounter = 0;            // 弾連射の間隔を図るカウンター変数

	g_Boss.nType = nType;  // 種類
	g_Boss.bUse = true;    // 使用状態へ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(pos.x - BOSS_SIZE_X, pos.y - BOSS_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + BOSS_SIZE_X, pos.y - BOSS_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - BOSS_SIZE_X, pos.y + BOSS_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + BOSS_SIZE_X, pos.y + BOSS_SIZE_Y, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();

	StartBossAppearDirection();
}


//-------------------------
//    ボスの情報を取得
//-------------------------
BOSS* GetBoss(void)
{
	return &g_Boss;
}


//------------------------
//   ボスのヒット処理
//------------------------
void HitBoss(int nDamage, D3DXVECTOR3 pos)
{
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得

	if (g_Boss.state == BOSSSTATE_SPAWN)
	{//出現演出中は無効
		SetActionEffect(pos, ACTION_EFFECT_TEX_A, 25.0f);   //なんか無効化してるっぽいエフェクト出してえな
		return;
	}
	else if (g_Boss.state == BOSSSTATE_DEAD)
	{
		return;
	}

	SetActionEffect(pos, ACTION_EFFECT_TEX_A, 25.0f);
	g_Boss.nLife -= nDamage;

	g_Boss.nHitCount++;

	if (g_Boss.nHitCount >= 3)
	{

		g_Boss.nHitCount = 0;  //ヒットカウントをリセット
		AddCombo();
		AddBossHitScore();
	}

	if (g_Boss.nLife <= 0)
	{

		g_Boss.nLife = 0;
		
		pPlayer->nKill++;
		AddCombo();
		StartBossLeaveDirection();

		
	}

}

//-------------------------
//  ボスの行動パターン
//-------------------------
void BossActionPattern(void)
{
	Player* pPlayer = GetPlayer();

	switch (g_Boss.nType)
	{
	case BOSS_TYPE_A:
		//==================================
		//    Nルートボスの行動パターン
		//==================================
		
		// 通常弾
		if (g_Boss.nBulletCounter > 0)
		{//弾の発射クールタイム中
			g_Boss.nBulletCounter--;
		}
		else
		{
			float BulletMove_x = 0.5f;
			for (int nCnt = -1; nCnt <= 1; nCnt++)
			{
				SetBullet(g_Boss.pos, D3DXVECTOR3(BulletMove_x * nCnt, 3.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
			}

			g_Boss.nBulletCounter = 60;
		}

		if (g_Boss.nActionCounter % 30 == 0 && g_Boss.nActionCounter != 0)
		{
			D3DXVECTOR3 vDir = pPlayer->pos - g_Boss.pos;
			D3DXVec3Normalize(&vDir, &vDir);

			float fAngle = atan2f(vDir.y, vDir.x);
			//移動量を設定
			D3DXVECTOR3 move(cosf(fAngle) * 4.0f, sinf(fAngle) * 4.0f, 0.0f);

			SetBullet(g_Boss.pos, move, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);

		}
		else if (g_Boss.nLife < BOSS_MAX_HP * 0.6)
		{
			if (g_Boss.nBulletCounter == 0)
			{
				// 時間経過で少しずつ角度をずらす
				float fBasicAngle = (g_Boss.nActionCounter * 0.15f);
				int nWay = 5; // 5方向同時に打ち出しながら回転

				for (int nCnt = 0; nCnt < nWay; nCnt++)
				{
					// 角度の設定
					float fAngle = fBasicAngle + (D3DX_PI * 2.0f / nWay) * nCnt;
					
					// moveの設定
					D3DXVECTOR3 Speed;
					Speed.x = cosf(fAngle) * 3.5f;
					Speed.y = sinf(fAngle) * 3.5f;
					Speed.z = 0.0f;

					SetBullet(g_Boss.pos, Speed, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}

				g_Boss.nBulletCounter = 6; // 高速連射
			}
		}
		break;
	case BOSS_TYPE_B:
		//==================================
		//    αルートボスの行動パターン
		//==================================

		if (g_Boss.nBulletCounter > 0)
		{//弾の発射クールタイム中
			g_Boss.nBulletCounter--;		
		}

		if (g_Boss.nActionCounter < 420 && g_Boss.nBulletCounter <= 0)
		{//最初の７秒
			
			for (float fCntShot = -0.5f; fCntShot <= 0.5f; fCntShot += 1.0f)
			{
				D3DXVECTOR3 Shotpos = D3DXVECTOR3(g_Boss.pos.x + (225.0f * fCntShot), g_Boss.pos.y - 40.0f, g_Boss.pos.z);
				D3DXVECTOR3 vDir = pPlayer->pos - Shotpos;
				D3DXVec3Normalize(&vDir, &vDir);

				float fAngle = atan2f(vDir.y, vDir.x);
				float fSpread = 0.5f; // 拡散指せる具合

				for (float fCnt = -1.5f; fCnt <= 1.5f; fCnt += 1.0f)
				{// 自機に当たらない２way
					float fFinalAngle = fAngle + (fCnt * fSpread);
					//移動量を設定
					D3DXVECTOR3 move(cosf(fFinalAngle) * 15.0f, sinf(fFinalAngle) * 15.0f, 0.0f);

					SetBullet(Shotpos, move, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);

				}
			}
			if (g_Boss.nActionCounter % 30 == 0 && g_Boss.nActionCounter < 300)
			{
				float BulletMove_x = 0.5f;
				for (int nCnt = -1; nCnt <= 1; nCnt++)
				{
					SetBullet(g_Boss.pos, D3DXVECTOR3(BulletMove_x * nCnt, 3.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 30.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}
			}

			g_Boss.nBulletCounter = 1; // リロード時間

			
		}
		else if (g_Boss.nActionCounter == 420)
		{
			g_Boss.nBulletCounter = 0; // リロード時間リセット
		}

		if (g_Boss.nActionCounter > 420 && g_Boss.nActionCounter < 900)
		{
			// 16方向リング全方位弾
			if (g_Boss.nActionCounter % 120 == 0)
			{
				int nWay = 16;
				for (int nCnt = 0; nCnt < nWay; nCnt++)
				{
					float fAngle = (D3DX_PI * 2.0f / nWay) * nCnt;
					D3DXVECTOR3 Speed;
					Speed.x = cosf(fAngle) * 2.5f;
					Speed.y = sinf(fAngle) * 2.5f;
					Speed.z = 0.0f;

					SetBullet(g_Boss.pos, Speed, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 25.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}
			}

			// 5way自機狙い
			if (g_Boss.nBulletCounter <= 0 && pPlayer != NULL)
			{
				D3DXVECTOR3 vDir = pPlayer->pos - g_Boss.pos;
				D3DXVec3Normalize(&vDir, &vDir);

				float fAngle = atan2f(vDir.y, vDir.x);
				float fSpread = 0.15f; // 拡散具合

				for (int nCnt = -2; nCnt <= 2; nCnt++)
				{//－１から始めたらなんかいい感じになったんでこれで
					float fFinalAngle = fAngle + (nCnt * fSpread);
					//移動量を設定
					D3DXVECTOR3 move(cosf(fFinalAngle) * 4.0f, sinf(fFinalAngle) * 4.0f, 0.0f);

					SetBullet(g_Boss.pos, move, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}

				g_Boss.nBulletCounter = 15; // リロード時間
			}
		}
		else if (g_Boss.nActionCounter == 900)
		{
			g_Boss.nBulletCounter = 0; // クールリセット
		}

		if (g_Boss.nActionCounter > 900 && g_Boss.nActionCounter < 1800)
		{
			

			if (g_Boss.nActionCounter % 8 == 0)
			{
				// 時間経過で少しずつ角度をずらす
				float fBasicAngle = (g_Boss.nActionCounter * 0.15f);
				int nWay = 8; // 5方向同時に打ち出しながら回転

				for (int nCnt = 0; nCnt < nWay; nCnt++)
				{
					// 角度の設定
					float fAngle = fBasicAngle + (D3DX_PI * 2.0f / nWay) * nCnt;

					// moveの設定
					D3DXVECTOR3 Speed;
					Speed.x = cosf(fAngle) * 3.5f;
					Speed.y = sinf(fAngle) * 3.5f;
					Speed.z = 0.0f;

					SetBullet(g_Boss.pos, Speed, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}
			}
		}

		if (g_Boss.nActionCounter >= 1800)
		{
			g_Boss.nActionCounter = 0;
			g_Boss.nBulletCounter = 0;
		}

		break;


	case BOSS_TYPE_C:
		//==================================
		//    Ωルートボスの行動パターン
		//==================================

		if (g_Boss.nBulletCounter > 0)
		{//弾の発射クールタイム中
			g_Boss.nBulletCounter--;
		}
		else
		{
			// 時間経過で少しずつ角度をずらす
			float fBasicAngle = (g_Boss.nActionCounter * 0.15f);
			int nWay;

			// 一度に撃つ数をボスの体力次第で設定
			if (g_Boss.nLife > BOSS_MAX_HP * 0.5)
			{
				nWay = 1;
			}
			else if (g_Boss.nLife > BOSS_MAX_HP * 0.3 && g_Boss.nLife <= BOSS_MAX_HP * 0.5)
			{
				nWay = 4;
			}
			else
			{
				nWay = 6;
			}

			for (int nCnt = 0; nCnt < nWay; nCnt++)
			{
				// 角度
				float fAngle = fBasicAngle + (D3DX_PI * 2.0f / nWay) * nCnt;
				// move
				D3DXVECTOR3 Speed;
				Speed.x = cosf(fAngle) * 3.5f;
				Speed.y = sinf(fAngle) * 3.5f;
				Speed.z = 0.0f;

				SetBullet(D3DXVECTOR3(g_Boss.pos.x + 100.0f, g_Boss.pos.y - 37.5f, 0.0f),
					Speed,
					D3DXCOLOR(0.0f, 0.9f, 1.0f, 1.0f),
					20.0f,
					BULLETTYPE_ENEMY,
					BULLETKIND_NORMAL);

				SetBullet(D3DXVECTOR3(g_Boss.pos.x - 100.0f, g_Boss.pos.y - 37.5f, 0.0f),
					D3DXVECTOR3(-Speed.x, Speed.y, Speed.z),
					D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
					20.0f,
					BULLETTYPE_ENEMY,
					BULLETKIND_NORMAL);
			}

			g_Boss.nBulletCounter = 4; // 高速連射
		}

		if (g_Boss.nLife > BOSS_MAX_HP * 0.5)
		{
			if (g_Boss.nActionCounter % 60 == 0)
			{
				// 3wayの自機狙い
				D3DXVECTOR3 vDir = pPlayer->pos - g_Boss.pos;  // ボスから見たプレイヤーの座標(要はどんくらいX,Y,Zで離れてるかを数値化してんだわさ)
				D3DXVec3Normalize(&vDir, &vDir);    // 各ベクトルの比率をそのままに、斜辺の長さを１に固定した結果のX,Y,Z座標の値を上書きする

				float fAngle = atan2f(vDir.y, vDir.x);
				float fSpread = 0.15f;        // 拡散させる具合

				for (int nCnt = -1; nCnt <= 1; nCnt++)
				{// －１、０，１で３wayだな
					float fFinalAngle = fAngle + (nCnt * fSpread);
					// 移動量を設定
					D3DXVECTOR3 move(cosf(fFinalAngle) * 4.0f, sinf(fFinalAngle) * 4.0f, 0.0f);

					SetBullet(g_Boss.pos, move, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 20.0f, BULLETTYPE_ENEMY, BULLETKIND_NORMAL);
				}
			}
		}
		




		break;
	}
}

//----------------------------------------------
// ボムの範囲内にボスがいた場合の処理
//----------------------------------------------
void CollisionBomb_Boss(void)
{
	Bomb* pBomb = GetBomb();        // ボム情報のポインタを取得
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得

	if (pBomb->nLife < 5)
	{//消えかけの場合は処理しない(違和感あるから)

		g_Boss.bSufferBomb = false;
		return;
	}

	// すでにこのボムのダメージを受けていなければ判定
	if (g_Boss.bSufferBomb == false)
	{
		float dx = g_Boss.pos.x - pBomb->pos.x;
		float dy = g_Boss.pos.y - pBomb->pos.y;
		float fDistanceSq = (dx * dx) + (dy * dy);               // 距離の2乗
		float fRadiusSq = pBomb->fRadius * pBomb->fRadius;      // ボムの半径の2乗

		// 「距離の二乗」が「半径の二乗」以下の場合
		if (fRadiusSq >= fDistanceSq)
		{
			g_Boss.bSufferBomb = true; 

			HitBoss(30, g_Boss.pos);
		}
	}
}