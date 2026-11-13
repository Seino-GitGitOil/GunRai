//------------------------------------------------
//
// 残りライフのUIを表示する処理の宣言 [bombUI.h]
// Author：清野蒼翔
//
//------------------------------------------------
#ifndef _BOMBUI_H_
#define _BOMBUI_H_  //二重インクルード防止のマクロ

//マクロ定義
#define MAX_PLAYER_BOMBSTOCK (14)  //プレイヤーの最大ボム数(初期は２)


void InitBombUI(void);              // ライフの初期化処理
void UninitBombUI(void);            // ライフの終了処理
void UpdateBombUI(void);            // ライフの更新処理
void DrawBombUI(void);              // ライフの描画処理

#endif