//-------------------------------------------------------------
//
// ボスの演出処理の宣言 [BossDirection.h]
// Author：清野蒼翔
//
//-------------------------------------------------------------
#ifndef _BOSS_DIRECTION_H_
#define _BOSS_DIRECTION_H_

#include "main.h"


// 演出の状態
typedef enum
{
    BOSS_DIR_NOTHING = 0,    // 演出なし（通常時）
    BOSS_DIR_APPEAR,         // 登場演出中（WARNING点滅）
    BOSS_DIR_SLIDE_IN,       // ボス画面外からのスライドイン
    BOSS_DIR_LEAVE,          // 退場（撃破爆発）演出中
    BOSS_DIR_MAX

} BOSS_DIR_STATE;

// プロトタイプ宣言
void InitBossDirection(void);
void UninitBossDirection(void);
void UpdateBossDirection(void);
void DrawBossDirection(void);

int GetBossDirState(void);           // ボス演出の状態を取得
void StartBossAppearDirection(void);  // 登場演出の開始
void StartBossLeaveDirection(void);   // 退場演出の開始


#endif