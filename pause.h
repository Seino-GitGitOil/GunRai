//--------------------------------
//
// ポーズ処理の宣言[pause.h]
// Author：清野蒼翔
//
//--------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_  //二重インクルード防止のマクロ



//プロトタイプ宣言
void InitPause(void);            // ポーズの初期化処理
void UninitPause(void);          // ポーズの終了処理
void UpdatePause(void);          // ポーズの更新処理
void DrawPause(void);            // ポーズの描画処理
bool GetPause(void);             // ポーズの情報を取得
void TogglePause(void);          // ポーズの反転処理
void SetPause(void);          // ポーズの設定処理


#endif
