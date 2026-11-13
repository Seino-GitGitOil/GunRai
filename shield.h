//-----------------------------------------
//
// シールド処理の宣言 [shiled.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _SHIELD_H_
#define _SHIELD_H_  //二重インクルード防止のマクロ

//プロトタイプ宣言
void InitShield(void);              //シールドの初期化処理
void UninitShield(void);            //シールドの終了処理
void UpdateShield(void);            //シールドの更新処理
void DrawShield(void);              //シールドの描画処理


#endif