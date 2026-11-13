//--------------------------------
//
// コンボ処理に関する宣言[combo.h]
// Author：清野蒼翔
//
//--------------------------------
#ifndef _COMBO_H_
#define _COMBO_H_  //二重インクルード防止のマクロ


//プロトタイプ宣言
void InitCombo(void);                      // コンボの初期化処理
void UninitCombo(void);                    // コンボの終了処理
void UpdateCombo(void);                    // コンボの更新処理
void DrawCombo(void);                      // コンボの描画処理
void AddCombo(void);                       // コンボを増やす処理
float GetComboAlpha(void);                 // コンボの透明度を取得


#endif