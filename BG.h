//--------------------------------
//
// 背景にかかわる処理の宣言[BG.h]
// Author：清野蒼翔
//
//--------------------------------
#ifndef _BG_H_
#define _BG_H_  //二重インクルード防止のマクロ


// 背景テクスチャの列挙型定義
typedef enum
{
	BG_TEXTURE_0 = 0,
	BG_TEXTURE_1,
	BG_TEXTURE_2,
	BG_TEXTURE_MAX

}BG_TEXTURE_NAME;


//プロトタイプ宣言
void InitBG();            //背景の初期化処理
void UninitBG();          //背景の終了処理
void UpdateBG();          //背景の更新処理
void DrawBG();            //背景の描画処理

#endif