//-----------------------------------------
//
// 画面右端のUIを表示する処理の宣言 [GameUI.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _UI_BG_H_
#define _UI_BG_H_  //二重インクルード防止のマクロ


// UIの種類
typedef enum
{
	UI_LEFT_BG = 0,
	UI_RIGHT_BG,
	UI_BG_MAX

}GAMEUI_BG_TYPE;

//プロトタイプ宣言
void InitUI_BG(void);              //UI_BGの初期化処理
void UninitUI_BG(void);            //UI_BGの終了処理
void UpdateUI_BG(void);            //UI_BGの更新処理
void DrawUI_BG(void);              //UI_BGの描画処理


#endif