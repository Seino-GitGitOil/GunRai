//---------------------------------------
//
// ボスの処理の宣言 [Boss.h]
// Author：清野蒼翔
//
//---------------------------------------
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"

// マクロ定義
#define BOSS_SIZE_X (250.0f)   // ボスの幅の半分
#define BOSS_SIZE_Y (100.0f)   // ボスの高さの半分
#define BOSS_MAX_HP (750)      // ボスの最大体力

// ボスの種類
typedef enum
{
	BOSS_TYPE_A = 0,   // 通常ボス
	BOSS_TYPE_B,       // αルートボス
	BOSS_TYPE_C,       // Ωルートボス
	BOSS_MAX

}BOSS_TYPE;


// ボスの状態
typedef enum
{
	BOSSSTATE_SPAWN = 0,   // 出現前の状態
	BOSSSTATE_NORMAL,           // 通常状態
	BOSSSTATE_DAMAGE,           // 被弾状態（点滅など）
	BOSSSTATE_DEAD,             // 撃破演出中
	BOSSSTATE_MAX

} BOSS_STATE;



// ボスの行動フェーズ（攻撃パターン切替）
typedef enum
{
	BOSSPHASE_1 = 0,      // フェーズ1 (HP高)
	BOSSPHASE_2,          // フェーズ2 (中)
	BOSSPHASE_3,          // フェーズ3 (低)
	BOSSPHASE_MAX

} BOSS_PHASE;

// ボス構造体
typedef struct
{
	D3DXVECTOR3 pos;             // 位置
	D3DXVECTOR3 rot;             // 角度
	int nLife;                   // 現在の体力
	int nMaxLife;                // 最大体力
	BOSS_STATE state;            // 状態
	BOSS_PHASE phase;            // 行動フェーズ
	int nCounterState;           // 被弾時点滅カウンター
	int nHitCount;               // 自身にヒットした弾の数
	bool bSufferBomb;           
	int nActionCounter;          // 行動・タイマー用カウンター
	int nBulletCounter;          // 弾発射クールダウンカウンター

	float fLength;               // 対角線
	float fAngle;                // 角度

	int nType;                   // ボスの種類
	bool bUse;                   // 使用中か否か

} BOSS;

// プロトタイプ宣言
void InitBoss(void);                //初期化処理
void UninitBoss(void);              //終了処理
void UpdateBoss(void);              //更新処理
void DrawBoss(void);                //描画処理
void SetBoss(D3DXVECTOR3 pos, int nType);           //セット処理
void HitBoss(int nDamage, D3DXVECTOR3 pos);         //ヒット時の処理
BOSS* GetBoss(void);                //初期化処理
void CollisionBomb_Boss(void);      //ボムの当たり判定


#endif
