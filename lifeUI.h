//------------------------------------------------
//
// 残りライフのUIを表示する処理の宣言 [lifeUII.h]
// Author：清野蒼翔
//
//------------------------------------------------
#ifndef _LIFEUI_H_
#define _LIFEUI_H_  //二重インクルード防止のマクロ

//マクロ定義
#define MAX_PLAYER_LIFE (2)  //プレイヤーの最大残機数


void InitLife(void);              // ライフの初期化処理
void UninitLife(void);            // ライフの終了処理
void UpdateLife(void);            // ライフの更新処理
void DrawLife(void);              // ライフの描画処理

#endif