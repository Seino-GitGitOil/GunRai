//---------------------------------------
//
// 敵の処理[enemy.cpp]
// Author：清野蒼翔
//
//---------------------------------------
#include "enemy.h"
#include "bullet.h"
#include "bomb.h"
#include "explosion.h"
#include "ActionEffect.h"
#include "score.h"
#include "combo.h"
#include "player.h"
#include "sound.h"


// 敵のテクスチャの列挙型定義
typedef enum
{
	ENEMY_TEXTURE_A = 0,
	ENEMY_TEXTURE_B,
	ENEMY_TEXTURE_C,
	ENEMY_TEXTURE_D,
	ENEMY_TEXTURE_E,
	ENEMY_TEXTURE_MAX

}ENEMY_TEXTURE_NAME;



//プロトタイプ宣言
void EnemyActionPattern(int nCntEnemy); //敵の行動パターン


//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMY_TEXTURE_MAX] = {};  // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;       // 頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];                            // 敵の情報

const char* g_aEnemyTexture_Path[ENEMY_TEXTURE_MAX] =
{
	"data//TEXTURE//EnemyA.png",
	"data//TEXTURE//EnemyB.png",
	"data//TEXTURE//EnemyC.png",
	"data//TEXTURE//EnemyD.png",
	"data//TEXTURE//EnemyE.png",

};                 //背景テクスチャ


//-------------------------------------------
//    敵の初期化処理
//-------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (nCntEnemy = 0; nCntEnemy < ENEMY_TEXTURE_MAX; nCntEnemy++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aEnemyTexture_Path[nCntEnemy],
			&g_apTextureEnemy[nCntEnemy]);
	}

	//敵の情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 位置
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 角度
		g_aEnemy[nCntEnemy].nType = 0;          // 種類を初期化
		g_aEnemy[nCntEnemy].nLife = 3;          // 体力を初期化(仮)
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;  //敵の状態
		g_aEnemy[nCntEnemy].nCounterState = 0;  // 被弾時の点滅間隔
		g_aEnemy[nCntEnemy].bUse = false;       // 使用していない状態(false)にする
		g_aEnemy[nCntEnemy].nEnemyBulletCounter = 0;            // 敵の弾の発射間隔(クールダウンを初期化)
		g_aEnemy[nCntEnemy].nEnemyActionCounter = 0;            // 敵の行動の間隔

		g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf((ENEMY_SIZE * 2) * (ENEMY_SIZE * 2) + (ENEMY_SIZE * 2) * (ENEMY_SIZE * 2)) * 0.5f;  //対角線の長さを算出
		g_aEnemy[nCntEnemy].fAngleEnemy = atan2f((ENEMY_SIZE * 2), (ENEMY_SIZE * 2));  //対角線の角度を算出
		g_aEnemy[nCntEnemy].bSufferBomb = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_ENEMY
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 頂点座標の設定
		
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[3].pos.z = 0.0f;

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
	g_pVtxBuffEnemy->Unlock();

}
//-------------------------------------------
//    敵の終了処理
//-------------------------------------------
void UninitEnemy(void)
{
	int nCntEnemy;
	
	//テクスチャ(４枚分)の破棄
	for (nCntEnemy = 0; nCntEnemy < ENEMY_TEXTURE_MAX; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}
//---------------------------
// 敵の更新処理
//---------------------------
void UpdateEnemy(void)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//その敵が使われていたら

			EnemyActionPattern(nCntEnemy);
			CollisionBomb_Enemy(&g_aEnemy[nCntEnemy]);



			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
			{
				g_aEnemy[nCntEnemy].nCounterState--;  //HitEnemyでnCountStateを増やしたのでここで減らしていく

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{//nCounterStateが０以下になったら

					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					VERTEX_2D* pVtx;

					//頂点バッファをロックし、頂点データへのポインタを取得
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

					pVtx += 4 * nCntEnemy;   //対応した敵の頂点データのポインタを取得

					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//頂点バッファをアンロックする
					g_pVtxBuffEnemy->Unlock();
				}
			}

			//敵が画面の奥端まで到達した際に敵を消す処理
			if (g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE <= 360 - DEATH_BORDER_LINE * 2                      // 左端
				|| g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE >= SCREEN_WIDTH - 360 + DEATH_BORDER_LINE * 2    // 右端
				|| g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE <= - DEATH_BORDER_LINE * 2                       // 上端
				|| g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE >= SCREEN_HEIGHT + DEATH_BORDER_LINE * 2)        // 下端
			{
				g_aEnemy[nCntEnemy].bUse = false;   // 敵を消す
			}

		}
	}
}

//-------------------------------------------
//    敵の描画処理
//-------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている

			//テクスチャの設定(０～３に合わせて設定する)
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);      //描画を開始する頂点インデックス
		}
	}
}
//-------------------------------------------
//    敵のセット処理
//-------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, int nType)  
{
	int nCntEnemy;
	VERTEX_2D* pVtx;  //頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない場合

			//敵の情報の設定    g_aEnemy
			g_aEnemy[nCntEnemy].pos = pos;                    // 指定された位置に敵のposを設定
			g_aEnemy[nCntEnemy].nType = nType;                // 指定された種類の敵を生成
			g_aEnemy[nCntEnemy].bUse = true;                  // 使用している状態にする
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;    // 敵の状態
			g_aEnemy[nCntEnemy].nCounterState = 0;            // 点滅させる
			g_aEnemy[nCntEnemy].nEnemyActionCounter = 0;      // 敵の行動の間隔
			g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 角度
			g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf((ENEMY_SIZE * 2) * (ENEMY_SIZE * 2) + (ENEMY_SIZE * 2) * (ENEMY_SIZE * 2)) * 0.5f;  //対角線の長さを算出
			g_aEnemy[nCntEnemy].fAngleEnemy = atan2f((ENEMY_SIZE * 2), (ENEMY_SIZE * 2));  //対角線の角度を算出
			g_aEnemy[nCntEnemy].bSufferBomb = false;

			switch (nType)
			{

			case ENEMY_TYPE_A:
				g_aEnemy[nCntEnemy].nLife = 3;                      // 体力を初期化
				g_aEnemy[nCntEnemy].nEnemyBulletCounter = 0;        // 敵の弾の発射間隔(クールダウンを初期化)
				break;
			
			case ENEMY_TYPE_B:
				g_aEnemy[nCntEnemy].nLife = 1;              
				g_aEnemy[nCntEnemy].nEnemyBulletCounter = 0;     
				break;
			
			case ENEMY_TYPE_C:
				g_aEnemy[nCntEnemy].nLife = 1;
				g_aEnemy[nCntEnemy].nEnemyBulletCounter = 0;
				break;
			
			}




			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			
			//頂点カラーの初期化
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx += 4;        //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//-------------------------------------------
//    敵の先頭アドレスを返す処理
//-------------------------------------------
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];    //「敵の情報」の"先頭アドレス"を返す
}

//-------------------------------------------
//            敵のヒット処理
//-------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	Player* pPlayer = GetPlayer();  //コンボ数やそれに応じた得点処理に必要

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//敵の体力がなくなった場合(敵を殺した)

		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));    //爆発の設定


		AddCombo();                             // コンボ加算
		AddKillScore(&g_aEnemy[nCntEnemy]);            // キルスコア加算
		pPlayer->nKill++;                              // キル数加算(画面に表示する予定はなし、ルート分岐に使用)
		 
		
		
		g_aEnemy[nCntEnemy].bUse = false;   //敵を使われていない状態にする
	}
	else
	{//撃たれた上で敵の体力が尽きていない場合

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;  //敵を被ダメージ状態に移行
		g_aEnemy[nCntEnemy].nCounterState = 5;          //ダメージ状態を保つ時間を「５」フレームとして設定

		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += 4 * nCntEnemy;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();
	}
}

//-------------------------
//  敵の行動パターン
//-------------------------
void EnemyActionPattern(int nCntEnemy)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ
	Player* pPlayer = GetPlayer();    // プレイヤーの情報を取得

	// XとYの差を計算
	float dx = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
	float dy = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;

	float length = sqrtf(dx * dx + dy * dy);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx += 4 * nCntEnemy;   //対応した敵の頂点データのポインタを取得

	
	switch (g_aEnemy[nCntEnemy].nType)
	{
	case ENEMY_TYPE_A:

		g_aEnemy[nCntEnemy].pos.y += 3.0f;           // 位置を更新
		
		break;

	case ENEMY_TYPE_B:

		g_aEnemy[nCntEnemy].pos.y += 1.25f;           // 位置を更新
		g_aEnemy[nCntEnemy].nEnemyActionCounter++;    // 行動タイミングを決めるカウンター

		// 弾の発射間隔
		if (g_aEnemy[nCntEnemy].nEnemyBulletCounter > 0)
		{
			g_aEnemy[nCntEnemy].nEnemyBulletCounter--;
		}
		else
		{
			SetBullet(g_aEnemy[nCntEnemy].pos,
				D3DXVECTOR3(0.0f, 5.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				10,
				BULLETTYPE_ENEMY,
				BULLETKIND_NORMAL);

			g_aEnemy[nCntEnemy].nEnemyBulletCounter = 60;
		}
		break;
	
	case ENEMY_TYPE_C:
		
		g_aEnemy[nCntEnemy].pos.y += 1.0f; // ゆっくり下降

		// プレイヤーの方を向く
		g_aEnemy[nCntEnemy].rot.z = atan2f(dx, dy);


		if (g_aEnemy[nCntEnemy].nEnemyBulletCounter > 0)
		{
			g_aEnemy[nCntEnemy].nEnemyBulletCounter--;
		}
		else
		{
			

			SetBullet(g_aEnemy[nCntEnemy].pos,
				D3DXVECTOR3((dx / length) * 6.0f, (dy / length) * 6.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				10,
				BULLETTYPE_ENEMY,
				BULLETKIND_NORMAL);

			g_aEnemy[nCntEnemy].nEnemyBulletCounter = 120;
		}
		break;
	
		
	
	case ENEMY_TYPE_D:
		g_aEnemy[nCntEnemy].pos.y += 1.5f; // ゆっくり下降

		if (g_aEnemy[nCntEnemy].nEnemyBulletCounter > 0)
		{
			g_aEnemy[nCntEnemy].nEnemyBulletCounter--;
		}
		else
		{


			SetBullet(g_aEnemy[nCntEnemy].pos,
				D3DXVECTOR3(0.0f, 3.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				10,
				BULLETTYPE_ENEMY,
				BULLETKIND_BIG);

			g_aEnemy[nCntEnemy].nEnemyBulletCounter = 120;
		}
		break;

	case ENEMY_TYPE_E:
		g_aEnemy[nCntEnemy].nEnemyActionCounter++; // 敵の特徴的な行動をカウントする変数
		

		if (g_aEnemy[nCntEnemy].nEnemyActionCounter < 60)
		{
			g_aEnemy[nCntEnemy].pos.y += 1.5f; // ゆっくり下降

		}
		else if (g_aEnemy[nCntEnemy].nEnemyActionCounter >= 60 && g_aEnemy[nCntEnemy].nEnemyActionCounter <= 300)
		{
			if (pPlayer->PlayerState == PLAYERSTATE_NORMAL)
			{
				// プレイヤーの方を向く
				g_aEnemy[nCntEnemy].rot.z = atan2f(dx, dy);
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3((dx / length) * 3.0f, (dy / length) * 3.0f, 0.0f);
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			}
			else
			{
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move / 4;
			}
		}
		else
		{
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
		}
		
		

		
		
		break;
	
	}

	// 頂点座標の設定    pVtx
	pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//----------------------------------------------
// ボムの範囲内に敵がいた場合の処理
//----------------------------------------------
void CollisionBomb_Enemy(Enemy* pEnemy)
{
	Bomb* pBomb = GetBomb();        // ボム情報のポインタを取得
	Player* pPlayer = GetPlayer();  // プレイヤー情報を取得

	if (pBomb->nLife < 5)
	{//消えかけの場合は処理しない(違和感あるから)

		return;
	}
	if (pEnemy->bSufferBomb == false)
	{
		float dx = pEnemy->pos.x - pBomb->pos.x;
		float dy = pEnemy->pos.y - pBomb->pos.y;
		float fDistanceSq = (dx * dx) + (dy * dy);              // 距離の2乗 (Sqは２乗ってgoogle先生が言ってた)
		float fRadiusSq = pBomb->fRadius * pBomb->fRadius;      // ボムの半径の2乗

		if (fRadiusSq >= fDistanceSq)  //「距離の二乗」が「半径の二乗」以下の場合 
		{
			pEnemy->nLife -= 5;
			pEnemy->bSufferBomb = true;
			if (pEnemy->nLife <= 0)
			{//敵の体力がなくなった場合(敵を殺した)

				SetExplosion(pEnemy->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));    //爆発の設定


				AddCombo();                             // コンボ加算
				AddKillScore(pEnemy);            // キルスコア加算
				pPlayer->nKill++;                              // キル数加算(画面に表示する予定はなし、ルート分岐に使用)

				pEnemy->bUse = false;   //敵を使われていない状態にする
			}
		}
	}
	
}