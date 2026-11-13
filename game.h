//--------------------------------
//
// ゲーム画面処理の宣言[game.h]
// Author：清野蒼翔
//
//--------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"


//マクロ定義
#define GAME_DISPLAY_X (360.0f)   // ゲーム背景を張るX座標の境界


//プロトタイプ宣言
void InitGame(void);                      //ゲーム画面の初期化処理
void UninitGame(void);                    //ゲーム画面の終了処理
void UpdateGame(void);                    //ゲーム画面の更新処理
void DrawGame(void);                      //ゲーム画面の描画処理

#endif