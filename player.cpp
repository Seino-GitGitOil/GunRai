//---------------------------------------
//
// プレイヤー(自機)の処理[player.cpp]
// Author：清野蒼翔
//
//---------------------------------------
#include "game.h"
#include "player.h"
#include "shield.h"
#include "input.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "explosion.h"
#include "ActionEffect.h"
#include "BossDirection.h"
#include "bomb.h"
#include "Boss.h"
#include "hitbox.h"
#include "sound.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;       //プレイヤーのテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;  //プレイヤーの頂点バッファへのポインタ

Player g_player;  //プレイヤーの情報

//----------------------------------------
// プレイヤーの初期化処理
//----------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;  //デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//player001.png",
		&g_pTexturePlayer);



	//プレイヤーのステータスを初期化
	g_player.nRespawnTimer = 0;                   // リスポーンタイマー
	g_player.nSpawnImmuneTimer = 0;               // 復活後の無敵時間カウントダウン
	g_player.nLifeStock = 2;                      // 残機数
	g_player.nBulletCounter = 0.0f;               // 弾の発射間隔
	g_player.PlayerState = PLAYERSTATE_NORMAL;    // プレイヤーの状態
	g_player.nBombStock = 3;                      // ボム数  (いじる際はbombUI.hのマクロの数も変えること)
	g_player.nBulletStock = MAX_BULLET_GAUGE;     // 残弾ゲージ値
	g_player.nBurstMater = 0;                     // バーストゲージ値
	g_player.nCombo = 0;                          // コンボ数
	g_player.bBurstMode = false;                  // バーストモードのON / OFF
	g_player.bBurstShield = false;                // バーストシールドの有無
	g_player.nKill = 0;                           // キル数
	g_player.nOverheat = 0;                       // オーバーヒート回数
	g_player.nGraze = 0;                          // グレイズ成功回数
	g_player.bBurstGaugeMax = false;              // バーストゲージが最大ならtrue


	//ポリゴンのステータスを初期化
	g_player.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);   //ポリゴンの位置を初期化する
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      //ポリゴンの移動量を初期化する
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //向きを初期化

	//対角線の長さを算出
	g_player.fLengthPlayer = sqrtf((TEXTURE_WIDTH * 2) * (TEXTURE_WIDTH * 2) + (TEXTURE_HEIGHT * 2) * (TEXTURE_HEIGHT * 2)) * 0.5f;  //  √幅^2 ＋ 高さ^2 ÷２ = 対角線の半分

	//対角線の角度を算出
	g_player.fAnglePlayer = atan2f((TEXTURE_WIDTH * 2), (TEXTURE_HEIGHT * 2));

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);



	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();



}

//-------------------------------
// プレイヤーの終了処理
//-------------------------------
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//-------------------------------
// プレイヤーの更新処理
//-------------------------------
void UpdatePlayer(void)
{

	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//
	if (g_player.PlayerState == PLAYERSTATE_DEATH)
	{//死亡状態の場合


		g_player.nRespawnTimer--;   //リスポーンタイマーを減少
		

		//----------------------//
		//       復活処理       //
		//----------------------//

		if (g_player.nRespawnTimer <= 0)
		{//リスポーンタイマーが０以下になった場合
			if (g_player.nLifeStock > 0)
			{
				g_player.nLifeStock--;   //残機を一減らす

				//プレイヤーを再設置
				g_player.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);   //位置
				g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      //移動量
				g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //向き

				//頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - TEXTURE_WIDTH, g_player.pos.y - TEXTURE_HEIGHT, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + TEXTURE_WIDTH, g_player.pos.y - TEXTURE_HEIGHT, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - TEXTURE_WIDTH, g_player.pos.y + TEXTURE_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + TEXTURE_WIDTH, g_player.pos.y + TEXTURE_HEIGHT, 0.0f);


				//頂点バッファをアンロックする
				g_pVtxBuffPlayer->Unlock();


				//一部ステータスをリセット
				g_player.nBombStock = 2;                     //ボム数
				g_player.nBulletStock = MAX_BULLET_GAUGE;    //残弾数
				g_player.bBurstMode = false;                 //バーストモードをリセット

				PlaySound(SOUND_LABEL_SE_RESPAWN_2);         //リスポーン時の音

				g_player.PlayerState = PLAYERSTATE_SPAWN_IMMUNE;  //プレイヤーをリスポーン直後の無敵状態へ
				g_player.nSpawnImmuneTimer = 180;                 //３秒間の無敵時間を付与
			}
			else
			{
				g_player.PlayerState = PLAYERSTATE_GAMEOVER;
			}
			
		}
		return;   //一応コメント残しとく。これは「死亡状態の時に弾の当たり判定の処理とかを実行したくない」からここで区切っているんだよい。
	}

	if (g_player.PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{//スポーン直後の状態だった場合
		g_player.nSpawnImmuneTimer--;

		if (g_player.nSpawnImmuneTimer <= 0)
		{
			g_player.PlayerState = PLAYERSTATE_NORMAL; // 通常状態に復帰
		}
	}



	//-------------------------------//
	//    １フレーム毎に行う処理     //
	//-------------------------------//

	if (g_player.nBulletCounter > 0.0f)
	{//弾のクールタイムがまだあるなら実行

		g_player.nBulletCounter -= 0.7f;   // 弾のクールタイムの値をデクリメント(０になったら発射可能)
	}

	if (g_player.bBurstGaugeMax == false && g_player.bBurstMode == false)
	{//バーストメーターが最大値に達していない、もしくはプレイヤーがバースト状態では無いのなら実行

		g_player.nBurstMater += 0.2f;      // バーストメーターの値を増加
	}

	if ((g_player.bBurstGaugeMax == true || g_player.bBurstMode == true) && g_player.PlayerState != PLAYERSTATE_GAMEOVER)
	{//バースト状態かバーストゲージが最大なら
		SetEffect(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 15.0f, 18);
	}

	if (g_player.bBurstMode == true)
	{//プレイヤーがバースト状態なら
		g_player.nBurstMater -= 0.4f;

		if (g_player.nBurstMater <= 0)
		{
			g_player.bBurstMode = false;
			g_player.bBurstShield = false;
		}
	}

	if (g_player.nBurstMater >= MAX_BURST_MATER && g_player.bBurstGaugeMax == false)
	{//バーストゲージが最大の時に一度だけ実行
		SetActionEffect(D3DXVECTOR3(1095.0f, 650.0f, 0.0f), ACTION_EFFECT_TEX_D, 135.0f); //バーストゲージが最大の時のエフェクト
		SetActionEffect(D3DXVECTOR3(1100.0f, 655.0f, 0.0f), ACTION_EFFECT_TEX_E, 50.0f);  //バーストゲージが最大の時のアイコン
		PlaySound(SOUND_LABEL_SE_BURST_MAX);
		g_player.bBurstGaugeMax = true;
	}
	else if(g_player.nBurstMater < MAX_BURST_MATER && g_player.bBurstGaugeMax == true)
	{
		g_player.bBurstGaugeMax = false;
	}

	//---------------------------//
	//    入力された処理を実行   //
	//---------------------------//
	
	int BossDir = GetBossDirState();

	if (g_player.PlayerState != PLAYERSTATE_GAMEOVER)
	{
		if (BossDir != BOSS_DIR_APPEAR)
		{// ボス演出中は移動キー以外使用不可に

			//ショット処理
			if (GetKeyboardPress(DIK_SPACE) == true || GetJoypadPress(JOYKEY_B) == true)
			{//SPACEキーが押された場合
				if (g_player.bBurstMode == false)
				{//通常時の場合
					if (g_player.PlayerState == PLAYERSTATE_OVERHEAT)
					{//プレイヤーがオーバーヒート状態なら
						PlaySound(SOUND_LABEL_SE_ERROR);
					}
					else
					{
						//弾の設定
						if (g_player.nBulletCounter <= 0.0f && g_player.nBulletStock > 0)
						{
							g_player.nBulletStock--;           // 残弾数減少
							PlaySound(SOUND_LABEL_SE_SHOT);    // 弾撃った時の音
							//正面
							SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
								D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI) * 25.0f, cosf(g_player.rot.z + D3DX_PI) * 25.0f, 0.0f),     //弾の移動量
								D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    // 色
								10,                                   // 弾の寿命
								BULLETTYPE_PLAYER,                    // 弾の主
								BULLETKIND_NORMAL);                   // 弾の挙動
							//右上
							SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
								D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI - 0.05f) * 25.0f, cosf(g_player.rot.z + D3DX_PI - 0.05f) * 25.0f, 0.0f),     //弾の移動量
								D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    // 色
								10,                                   // 弾の寿命
								BULLETTYPE_PLAYER,                    // 弾の主
								BULLETKIND_NORMAL);                   // 弾の挙動
							//左上
							SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
								D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI + 0.05f) * 25.0f, cosf(g_player.rot.z + D3DX_PI + 0.05f) * 25.0f, 0.0f),     //弾の移動量
								D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
								10,                                   //弾の寿命
								BULLETTYPE_PLAYER,                    //弾の主
								BULLETKIND_NORMAL);                   //弾の挙動

							//現在の残りの弾数に応じて連射力を変更
							if (g_player.nBulletStock <= MAX_BULLET_GAUGE && g_player.nBulletStock > 45.0f)
							{
								g_player.nBulletCounter = 13.0f;
							}
							else if (g_player.nBulletStock <= 45.0f && g_player.nBulletStock > 35.0f)
							{
								g_player.nBulletCounter = 10.0f;
							}
							else if (g_player.nBulletStock <= 35.0f && g_player.nBulletStock > 15.0f)
							{
								g_player.nBulletCounter = 7.0f;
							}
							else if (g_player.nBulletStock <= 15.0f && g_player.nBulletStock > 0.0f)
							{
								g_player.nBulletCounter = 6.0f;
							}
							else if (g_player.nBulletStock <= 0.0f)
							{// オーバーヒート時の処理
								PlaySound(SOUND_LABEL_SE_SHIELD_BREAK);
								g_player.PlayerState = PLAYERSTATE_OVERHEAT;
								g_player.nOverheat++;
							}


						}
					}

				}

				if (g_player.bBurstMode == true)
				{//バーストモードの場合
					if (g_player.nBulletCounter <= 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT);
						//正面
						SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),  //プレイヤーの位置
							D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI) * 25.0f, cosf(g_player.rot.z + D3DX_PI) * 25.0f, 0.0f),     //弾の移動量
							D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
							10,                                   //弾の寿命
							BULLETTYPE_PLAYER,                    //弾の主
							BULLETKIND_NORMAL);                   //弾の挙動
						//右上
						SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
							D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI - 0.035f) * 25.0f, cosf(g_player.rot.z + D3DX_PI - 0.035f) * 25.0f, 0.0f),     //弾の移動量
							D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
							10,                                   //弾の寿命
							BULLETTYPE_PLAYER,                    //弾の主
							BULLETKIND_NORMAL);                   //弾の挙動
						//右上２
						SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
							D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI - 0.07f) * 25.0f, cosf(g_player.rot.z + D3DX_PI - 0.07f) * 25.0f, 0.0f),     //弾の移動量
							D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
							10,                                   //弾の寿命
							BULLETTYPE_PLAYER,                    //弾の主
							BULLETKIND_NORMAL);                   //弾の挙動
						//左上
						SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
							D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI + 0.035f) * 25.0f, cosf(g_player.rot.z + D3DX_PI + 0.035f) * 25.0f, 0.0f),     //弾の移動量
							D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
							10,                                   //弾の寿命
							BULLETTYPE_PLAYER,                    //弾の主
							BULLETKIND_NORMAL);                   //弾の挙動
						//左上２
						SetBullet(D3DXVECTOR3(g_player.pos.x + sinf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, g_player.pos.y + cosf(g_player.rot.z + D3DX_PI) * g_player.fLengthPlayer, 0),    //プレイヤーの位置
							D3DXVECTOR3(sinf(g_player.rot.z + D3DX_PI + 0.07f) * 25.0f, cosf(g_player.rot.z + D3DX_PI + 0.07f) * 25.0f, 0.0f),     //弾の移動量
							D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),    //色
							10,                                   //弾の寿命
							BULLETTYPE_PLAYER,                    //弾の主
							BULLETKIND_NORMAL);                   //弾の挙動

						g_player.nBulletCounter = 4;  // クールタイム
					}
				}
			}
			else if (GetKeyboardPress(DIK_SPACE) == false || GetJoypadPress(JOYKEY_B) || g_player.PlayerState == PLAYERSTATE_OVERHEAT)
			{//弾が発射されていない場合の処理

				if (g_player.nBulletStock < MAX_BULLET_GAUGE)
				{//残弾数が最大でない場合

					g_player.nBulletStock += 0.2f;

					//オーバーヒート状態の場合
					if (g_player.PlayerState == PLAYERSTATE_OVERHEAT && g_player.nBulletStock >= MAX_BULLET_GAUGE * 0.4f)
					{
						g_player.PlayerState = PLAYERSTATE_NORMAL;
						PlaySound(SOUND_LABEL_SE_RELOAD);
					}
				}
			}

			//バーストモードへ移行
			if (GetKeyboardTrigger(DIK_N) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{// Nキーが押された場合

				if (g_player.bBurstMode == false && g_player.nBurstMater >= MAX_BURST_MATER)
				{//既にバースト状態でないかつバーストゲージが最大なら、処理を実行

					if (g_player.nBurstMater >= MAX_BURST_MATER)
					{
						PlaySound(SOUND_LABEL_SE_BURST);    // バースト起動音

						g_player.bBurstMode = true;         // プレイヤーのバーストモードを有効化
						g_player.bBurstShield = true;       // プレイヤーのバーストシールドを有効化
					}
				}
			}

			//ボムを起動
			if (GetKeyboardTrigger(DIK_B) == true || GetJoypadTrigger(JOYKEY_Y) == true)
			{// Bキーが押された場合

				Bomb* pBomb = GetBomb();  //ボムの情報を取得

				if (g_player.nBombStock > 0 && pBomb->bUse == false)
				{// ボムストックがまだ残っているなら実行

					// ボムのセット処理 (ボムが既に使用状態かどうかはbomb.cpp側で確認してるよん)
					SetBomb(g_player.pos);
					PlaySound(SOUND_LABEL_SE_BOMB);
					g_player.nBombStock--;
				}
				else
				{// ボムストックがもう０なら

					//エラー音を鳴らす処理
					PlaySound(SOUND_LABEL_SE_ERROR);

				}
			}
		}



		if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
			{
				if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{// Aキーが押されたら分岐する

					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{// 同時にWキーが押された場合
						g_player.move.x += sinf(-D3DX_PI * 0.75) * PLAYER_OFFSET_SHIFT;
						g_player.move.y += cosf(-D3DX_PI * 0.75) * PLAYER_OFFSET_SHIFT;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{// 同時にSキーが押された場合
						g_player.move.x += sinf(-D3DX_PI * 0.25) * PLAYER_OFFSET_SHIFT;
						g_player.move.y += cosf(-D3DX_PI * 0.25) * PLAYER_OFFSET_SHIFT;
					}
					else
					{// 左キーのみが押された場合
						g_player.move.x += -PLAYER_OFFSET_SHIFT;
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{// Dキーが押されたら分岐する

					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{// 同時にWキーが押された場合
						g_player.move.x += sinf(D3DX_PI * 0.75) * PLAYER_OFFSET_SHIFT;
						g_player.move.y += cosf(D3DX_PI * 0.75) * PLAYER_OFFSET_SHIFT;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{// 同時にSキーが押された場合
						g_player.move.x += sinf(D3DX_PI * 0.25) * PLAYER_OFFSET_SHIFT;
						g_player.move.y += cosf(D3DX_PI * 0.25) * PLAYER_OFFSET_SHIFT;
					}
					else
					{// Dキーのみが押されている場合
						g_player.move.x += PLAYER_OFFSET_SHIFT;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
				{// Wキーが押された

					g_player.move.y += -PLAYER_OFFSET_SHIFT;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{// Sキーが押された

					g_player.move.y += PLAYER_OFFSET_SHIFT;
				}
			}
			else
			{//SHIFTが押されていない場合(通常速度)
				if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{// Aキーが押されたら分岐する
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{// 同時にWキーが押された場合
						g_player.move.x += sinf(-D3DX_PI * 0.75) * PLAYER_OFFSET;
						g_player.move.y += cosf(-D3DX_PI * 0.75) * PLAYER_OFFSET;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{// 同時にSキーが押された場合
						g_player.move.x += sinf(-D3DX_PI * 0.25) * PLAYER_OFFSET;
						g_player.move.y += cosf(-D3DX_PI * 0.25) * PLAYER_OFFSET;
					}
					else
					{// 左キーのみが押された場合
						g_player.move.x += -PLAYER_OFFSET;
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{// Dキーが押されたら分岐する

					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{// 同時にWキーが押された場合
						g_player.move.x += sinf(D3DX_PI * 0.75) * PLAYER_OFFSET;
						g_player.move.y += cosf(D3DX_PI * 0.75) * PLAYER_OFFSET;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{// 同時にSキーが押された場合
						g_player.move.x += sinf(D3DX_PI * 0.25) * PLAYER_OFFSET;
						g_player.move.y += cosf(D3DX_PI * 0.25) * PLAYER_OFFSET;
					}
					else
					{// Dキーのみが押されている場合
						g_player.move.x += PLAYER_OFFSET;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
				{// Wキーが押された

					g_player.move.y += -PLAYER_OFFSET;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{// Sキーが押された

					g_player.move.y += PLAYER_OFFSET;
				}
			}
		}
		else
		{
			D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			float fStickX = GetJoypadStickX();
			float fStickY = GetJoypadStickY();

			// スティックに傾きが入っている場合はスティックの入力を優先（Y軸は画面座標に合わせて反転）
			if (fStickX != 0.0f || fStickY != 0.0f)
			{
				moveDir.x = fStickX;
				moveDir.y = -fStickY; // 上入力(+sY)で画面の上方向(-Y)へ動かすため反転

				float fLength = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
				if (fLength > 1.0f)
				{
					moveDir.x /= fLength;
					moveDir.y /= fLength;
				}
			}

			// 低速移動などの切り替え
			float fSpeed = PLAYER_OFFSET;
			if (GetKeyboardPress(DIK_LSHIFT) || GetJoypadPress(JOYKEY_L1)) // L1押しで低速
			{
				fSpeed = PLAYER_OFFSET_SHIFT;
			}

			// プレイヤーの位置計算
			g_player.move.x += moveDir.x * fSpeed;
			g_player.move.y += moveDir.y * fSpeed;
		}



		//自機がプレイ画面の端を越えた際の処理

		if (g_player.pos.x - TEXTURE_WIDTH <= 360)
		{//自機が左端を超えた場合
			g_player.pos.x = TEXTURE_WIDTH + 360;
		}

		if (g_player.pos.x + TEXTURE_WIDTH >= SCREEN_WIDTH - 360)
		{//右端
			g_player.pos.x = SCREEN_WIDTH - TEXTURE_WIDTH - 360;
		}

		if (g_player.pos.y - TEXTURE_HEIGHT <= 0)
		{//上端
			g_player.pos.y = TEXTURE_HEIGHT;
		}

		if (g_player.pos.y + TEXTURE_HEIGHT >= SCREEN_HEIGHT)
		{//下端
			g_player.pos.y = SCREEN_HEIGHT - TEXTURE_HEIGHT;
		}

		//敵との接触判定確認処理
		CheckCollisionEnemy();
		CheckCollisionBoss();


	}
	
	
    //頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);          //　x,yに該当する値はテクスチャを変えた際に必ず変更すること(0.125にしてるが実際に作るときは1.0/パターン数とかにしてもいいかも)
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	//位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//移動量を更新
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;      // 減速する処理　　　 式：(目的の値 - 現在の値) * 減衰係数
	g_player.move.y += (0.0f - g_player.move.y) * 0.2f;      // 減速する処理　　   減衰係数は少ない方がよりツルツル滑るぞ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}


//-------------------------------
// プレイヤーの描画処理
//-------------------------------
void DrawPlayer(void)
{
	if (g_player.PlayerState == PLAYERSTATE_DEATH || g_player.PlayerState == PLAYERSTATE_GAMEOVER)
	{//もしプレイヤーが死亡状態なら即座にこの関数を抜ける(描写をやめる)

		return;
	}

	if (g_player.PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{
		// ４フレ毎に描画をスキップしてみる　　　　
		if ((g_player.nSpawnImmuneTimer / 4) % 2 == 0)
		{
			return;
		}
	}
	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//----------------------------
//     プレイヤーの取得
//----------------------------
Player* GetPlayer(void)
{
	return &g_player;
}

//-------------------------------
//       自機のヒット処理
//-------------------------------
void HitPlayer(void)
{//被弾時の処理

	if (g_player.PlayerState == PLAYERSTATE_DEATH || g_player.PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{//プレイヤーが死亡状態またはスポーン後の無敵状態中だった場合

		return;
	}


	SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));    // 爆発の設定

	if (g_player.bBurstShield == true)
	{//バーストシールドが残っていた場合

		g_player.bBurstShield = false;       //ウルトシールドを消失
		PlaySound(SOUND_LABEL_SE_SHIELD_BREAK);
	}
	else
	{//シールドがなかった場合

		PlaySound(SOUND_LABEL_SE_PLAYER_LOST);        // 自機撃墜音

		g_player.nCombo = 0;                          // コンボ数リセット
		g_player.PlayerState = PLAYERSTATE_DEATH;     // デス状態に移行
		g_player.nRespawnTimer = 120;                 // 120フレーム(２秒後)にリスポーン

		PlaySound(SOUND_LABEL_SE_RESPAWN_1);          // リスポーンまでの待機効果音
	}
}

//-------------------------------
//   敵とのあたり判定
//-------------------------------
void CheckCollisionEnemy(void)
{
	// プレイヤーが既に死亡中or無敵時間中の場合は判定しなくていいんでね
	if (g_player.PlayerState == PLAYERSTATE_DEATH || g_player.PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{
		return;
	}

	Enemy* pEnemy = GetEnemy();  // 敵情報を取得

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// 敵が使用中（画面上に存在している）場合のみ判定
		if (pEnemy[nCnt].bUse == true)
		{
			// 自機と敵の距離ベクトルを算出
			float dx = g_player.pos.x - pEnemy[nCnt].pos.x;
			float dy = g_player.pos.y - pEnemy[nCnt].pos.y;
			float fDistanceSq = dx * dx + dy * dy;         // 実際の離れ具合
			float fHitRadius = HITBOX_SIZE + ENEMY_SIZE - 0.5f; // 当たり判定のギリギリライン

			// 距離の2乗が当たり判定の2乗以下の場合は接触とする
			if (fDistanceSq <= (fHitRadius * fHitRadius))
			{
				HitPlayer();  
				break;
			}
		}
	}
}
//-------------------------------------
//   ボスとの当たり判定
//-------------------------------------
void CheckCollisionBoss(void)
{
	// プレイヤーが既に死亡中or無敵時間中の場合は判定しなくていいんでね
	if (g_player.PlayerState == PLAYERSTATE_DEATH || g_player.PlayerState == PLAYERSTATE_SPAWN_IMMUNE)
	{
		return;
	}

	BOSS* pBoss = GetBoss();  // ボス情報を取得
	if (g_player.pos.x >= pBoss->pos.x - BOSS_SIZE_X
		&& g_player.pos.x <= pBoss->pos.x + BOSS_SIZE_X
		&& g_player.pos.y >= (pBoss->pos.y - BOSS_SIZE_Y - 30.0f)
		&& g_player.pos.y <= (pBoss->pos.y + BOSS_SIZE_Y - 30.0f))
	{
		HitPlayer();
	}

	
}