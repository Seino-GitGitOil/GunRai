//--------------------------------------
//
// ゲーム画面処理[game.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "game.h"         
#include "player.h"       // プレイヤー
#include "hitbox.h"       // 当たり判定
#include "bomb.h"         // ボム
#include "shield.h"       // シールド
#include "BG.h"           // 背景
#include "bullet.h"       // 弾
#include "Boss.h"         // ボス
#include "BossDirection.h"// ボスの演出
#include "explosion.h"    // 爆発
#include "enemy.h"        // 敵
#include "score.h"        // スコア
#include "effect.h"       // エフェクト
#include "ActionEffect.h" // アニメーション付きのエフェクト系
#include "mission.h"      // ミッション
#include "sound.h"        // 音楽、効果音
#include "fade.h"         // フェード
#include "UI_BG.h"        // UIの背景
#include "RankSetter.h"   // ランキング管理
#include "GaugeUI.h"      // ゲージ系のUI
#include "GameUI.h"       // ゲームUI
#include "LifeUI.h"       // ライフUI

#include "combo.h"        // コンボ

#include "BombUI.h"       // ボムUI
#include "EnemyWave.h"    // ウェーブ(敵のスポーンに関する奴)
#include "pause.h"        // ポーズ画面

#include "input.h"  //終了条件をいったん「Fキー押したら」にしてるから入れてるけど最終的にはいらんと思う

//グローバル変数
int g_nTime = 0;   //時間(一秒ずつ)カウントする変数
int g_nGameTimeCounter = 0;   //時間(一秒ずつ)カウントする変数
bool g_bGameOverChecker;  //ゲーム終了になっているかどうかを感知するフラグ メジャーな名づけとか先生に聞けたら聞いてみよう





//-------------------------------------
//   ゲーム画面における初期化処理
//-------------------------------------
void InitGame(void)
{
	g_nTime = 0;
	g_bGameOverChecker = false;  //ゲームオーバーフラグを初期化

	//背景の初期化処理
	InitBG();

	//プレイヤーの初期化処理
	InitPlayer();

	//シールドの初期化処理
	InitShield();

	//当たり判定の初期化処理
	InitHitBox();
	
	//弾の初期化処理
	InitBullet();

	//ボスの初期化処理
	InitBoss();

	//ボムの初期化処理
	InitBomb();

	//爆発の初期化処理
	InitExplosion();

	//敵の初期化処理
	InitEnemy();

	//スコアの初期化処理
	InitScore();

	//エフェクトの初期化処理
	InitEffect();

	//アクションエフェクトの初期化処理
	InitActionEffect();



	//UIの背景の初期化処理
	InitUI_BG();

	//ランキングのスコアを表示
	SetRankingScore(D3DXVECTOR3(106.0f, 267.0f, 0.0f), 15.0f, 21.0f, 18.0f);

	//ゲージ系UIの初期化処理
	InitGauge();

	//ゲームUIの初期化処理
	InitUI();

	//ミッションチェックの初期化処理
	InitCheckMission();
	
	//ライフの初期化処理
	InitLife();

	//ボムUIの初期化処理
	InitBombUI();

	//コンボの初期化処理
	InitCombo();

	//ウェーブの初期化処理
	InitWave();

	//ボス演出の初期化処理
	InitBossDirection();
	
	//ポーズ画面の初期化処理
	InitPause();
	
	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);      //再生したいサウンドを指定

	LoadRanking();
}

//----------------------------------
//   ゲーム画面における終了処理
//----------------------------------
void UninitGame(void)
{
	//サウンドの停止
	StopSound();
	
	//背景の終了処理
	UninitBG();

	//プレイヤーの終了処理
	UninitPlayer();

	//シールドの終了処理
	UninitShield();

	//当たり判定の終了処理
	UninitHitBox();

	//弾の終了処理
	UninitBullet();

	//ボスの終了処理
	UninitBoss();

	//ボムの終了処理
	UninitBomb();

	//爆発の終了処理
	UninitExplosion();

	//敵の終了処理
	UninitEnemy();

	//スコアの終了処理               1111111111111111111
	UninitScore();

	//エフェクトの終了処理
	UninitEffect();

	//アクションエフェクトの終了処理
	UninitActionEffect();

	//UIの背景の終了処理
	UninitUI_BG();

	

	//ゲージ系UIの終了処理
	UninitGauge();

	//ゲームUIの終了処理
	UninitUI();

	//ミッションチェックの終了処理
	UninitCheckMission();

	//ライフUIの終了処理
	UninitLife();

	//ボムUIの終了処理
	UninitBombUI();

	//コンボの終了処理
	UninitCombo();

	/*
	//ウェーブの終了処理
	UninitWave();
	*/

	//ボス演出の終了処理
	UninitBossDirection();

	//ポーズ画面の終了処理
	UninitPause();
	
}

//----------------------------------
//   ゲーム画面における更新処理
//----------------------------------
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	bool bPause = GetPause();


	if (bPause == false)
	{
		//背景の更新処理
		UpdateBG();

		//プレイヤーの更新処理
		UpdatePlayer();

		//シールドの更新処理
		UpdateShield();

		//当たり判定の更新処理
		UpdateHitBox();

		//弾の更新処理
		UpdateBullet();

		//ボスの更新処理
		UpdateBoss();

		//ボムの更新処理
		UpdateBomb();

		//爆発の更新処理
		UpdateExplosion();

		//敵の更新処理
		UpdateEnemy();

		//スコアの更新処理
		UpdateScore();

		//エフェクトの更新処理
		UpdateEffect();

		//アクションエフェクトの更新処理
		UpdateActionEffect();




		//UIの背景の更新処理
		UpdateUI_BG();

		//ミッションチェックの更新処理
		UpdateCheckMission();

		//ゲージ系UIの更新処理
		UpdateGauge();

		//ゲームUIの更新処理
		UpdateUI();

		//ライフUIの更新処理
		UpdateLife();

		//ボムUIの更新処理
		UpdateBombUI();

		//コンボの更新処理
		UpdateCombo();

		//ボス演出の更新処理
		UpdateBossDirection();

		//ウェーブの更新処理
		UpdateWave();
	}
	else if (bPause == true)
	{//ポーズ画面の場合
		
		//ポーズ画面の更新処理
		UpdatePause();
	}
	

	//ゲーム終了を感知する処理
	if(pPlayer->PlayerState == PLAYERSTATE_GAMEOVER)
	{//プレイヤーがゲームオーバー状態なら

		if (g_bGameOverChecker == false)
		{//まだ一度もリザルト移行処理を通っていなければ

			//モード設定  リザルト画面に移行
			SetFade(MODE_RESULT);
			g_bGameOverChecker = true;
		}
	}
	else if (int NowWave = GetCurrentWave() == WAVE_MAX)
	{//ウェーブが全て終わったら

		//モード設定  リザルト画面に移行
		SetFade(MODE_RESULT);
		g_bGameOverChecker = true;
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START))
	{// Pキーが押された

		SetPause();       //ポーズの設定処理
		TogglePause();    //ポーズ状態の反転処理
	}

}

//----------------------------------
//   ゲーム画面における描画処理
//----------------------------------
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//プレイヤーの描画処理
	DrawPlayer();

	//シールドの描画処理
	DrawShield();

	//当たり判定の描画処理
	DrawHitBox();

	//ボスの描画処理
	DrawBoss();

	//弾の描画処理
	DrawBullet();

	

	//ボムの描画処理
	DrawBomb();

	

	//敵の描画処理
	DrawEnemy();

	//エフェクトの描画処理
	DrawEffect();

	

	//UIの背景の描画処理
	DrawUI_BG();

	//ランキングスコアの描画処理
	DrawRankingScore();

	//ゲージ系UIの描画処理
	DrawGauge();

	//ゲームUIの描画処理
	DrawUI();

	//ミッションチェックの描画処理
	DrawCheckMission();

	//アクションエフェクトの描画処理
	DrawActionEffect();

	//爆発の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();

	//ライフUIの描画処理
	DrawLife();

	//ボムUIの描画処理
	DrawBombUI();

	//コンボの描画処理
	DrawCombo();
	
	//ボス演出の描画処理
	DrawBossDirection();
	
	if (bool bPause = GetPause() == true)
	{
		//ポーズ画面の描画
		DrawPause();
	}

}