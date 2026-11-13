//--------------------------------------------------
//
// ゲージ系のUIを表示する処理の宣言 [GaugeUI.h]
// Author：清野蒼翔
//
//--------------------------------------------------
#ifndef _GAUGEUI_H_
#define _GAUGEUI_H_  //二重インクルード防止のマクロ

#include "main.h"



// UIの種類
typedef enum
{
	BULLET_GAUGE = 0,      // 残弾ゲージ
	BURST_GAUGE,           // バーストゲージ
	BOSS_LIFE_GAUGE,       // ボスの体力ゲージ

	GAUGE_MAX

}GAUGE_TYPE;


void InitGauge(void);              // ゲージの初期化処理
void UninitGauge(void);            // ゲージの終了処理
void UpdateGauge(void);            // ゲージの更新処理
void DrawGauge(void);              // ゲージの描画処理


#endif
