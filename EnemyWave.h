//-----------------------------------------
//
// ウェーブ処理の宣言 [EnemyWave.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _ENEMYWAVE_H_
#define _ENEMYWAVE_H_  //二重インクルード防止のマクロ

#include "main.h"

//現在の「ウェーブ数」　の列挙型定義
typedef enum
{
	WAVE_ONE_FIRST = 0,
	WAVE_ONE_SECOND,
	WAVE_ONE_THIRD,
	WAVE_TWO_FIRST,
	WAVE_TWO_SECOND,
	WAVE_THREE_FIRST,
	WAVE_THREE_SECOND,
	WAVE_FOUR,
	WAVE_FIVE,
	WAVE_BOSS,
	WAVE_MAX

}NUM_WAVE;



//敵をスポーンさせるのに
typedef struct
{
	NUM_WAVE wave;        // 対象のウェーブ
	int nSpawnTime;       // ウェーブ開始からの経過フレーム数
	D3DXVECTOR3 pos;      // 敵の出現位置
	int nType;            // 敵の種類 (ENEMY_TYPE_A とかのenemy.hにあるやつ)

}ENEMY_SPAWN_INFO;

//ウェーブごとに設定を変えたいデータ　の構造体定義
typedef struct
{
	int nWaveTimer;        // そのウェーブごとにタイマーを設定して時間を管理する
	bool bWaveClear;       // そのウェーブをクリアしたかどうか判定するフラグ

}WAVE_INFO;

//プロトタイプ宣言
void InitWave(void);              // ウェーブの初期化処理
/*
void UninitWave(void);            // ウェーブの終了処理
*/
void UpdateWave(void);            // ウェーブの更新処理
int GetCurrentWave(void);         // 現在のウェーブ数を取得
WAVE_INFO GetWaveInfo(void);      // ウェーブ情報を取得　他の関数で使うと思うから一応作るが要らんかったら消せ
int GetEndRoute(void);            // 到達したエンドを取得    



#endif
