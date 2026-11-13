//---------------------------------------
//
// ヒットボックスの処理[hitbox.h]
// Author：清野蒼翔
//
//---------------------------------------
#ifndef _HITBOX_H_
#define _HITBOX_H_  //二重インクルード防止のマクロ

//マクロ定義
#define HITBOX_SIZE (7.5f)


//プロトタイプ宣言
void InitHitBox(void);              //当たり判定の初期化処理
void UninitHitBox(void);            //当たり判定の終了処理
void UpdateHitBox(void);            //当たり判定の更新処理
void DrawHitBox(void);              //当たり判定の描画処理


#endif