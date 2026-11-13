//-----------------------------------------
//
// 画面右端のUIを表示する処理の宣言 [GameUI.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _GAMEUI_H_
#define _GAMEUI_H_  //二重インクルード防止のマクロ

#include "main.h"



// UIの種類
typedef enum
{
	UI_LIFE_STRING = 0,
	UI_BOMB_STRING,
	UI_COMBO_STRING,

	UI_HOW_TO_PLAY,

	UI_BULLET_STRING,
	UI_BULLET_GAUGE_FRAME,    // 残弾ゲージの枠
	
	UI_BURST_STRING,
	UI_BURST_GAUGE_FRAME,     // バーストゲージの枠

	UI_SCORE_STR_GAME,        // スコアの文字(ゲームの時用のテクスチャ)

	UI_RANKING_STR_GAME,      // ランキングの文字(ゲームの時用のテクスチャ)
	UI_RANKING_SIMBOL,        // 1,2,3位のエンブレム
	
	UI_MISSION_FRAME,
	UI_MISSION_STRING,
	UI_MISSION_LIST,          // ミッションリスト

	UI_BOSS_LIFE_GAUGE_FRAME,     //ボスの体力ゲージの枠


	UI_MAX

}GAMEUI_TYPE;

//プロトタイプ宣言
void InitUI(void);              //UIの初期化処理
void UninitUI(void);            //UIの終了処理
void UpdateUI(void);            //UIの更新処理
void DrawUI(void);              //UIの描画処理


#endif