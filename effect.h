//-------------------------------------------
//
// エフェクトの処理に関する宣言[effect.h]
// Author：清野蒼翔
//
//-------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_  //二重インクルード防止のマクロ

#include "main.h"


//プロトタイプ宣言
void InitEffect(void);                      //エフェクトの初期化処理
void UninitEffect(void);                    //エフェクトの終了処理
void UpdateEffect(void);                    //エフェクトの更新処理
void DrawEffect(void);                      //エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);  //エフェクトのセット処理

#endif