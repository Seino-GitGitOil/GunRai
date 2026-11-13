//--------------------------------------
//
// リザルト画面処理の宣言[ResultUI.h]
// Author：清野蒼翔
//
//--------------------------------------

#ifndef _RESULT_UI_H_
#define _RESULT_UI_H_

//選択肢の列挙型定義
typedef enum
{
	RESULT_MENU_RETURN_TITLE = 0,     // タイトルに戻る
	RESULT_MENU_RANKING,              // ランキング
	RESULT_MENU_RESTART,              // リスタート(再プレイ)

}RESULT_MENU;

//選択肢を動かす際の関数へ渡す値の列挙型定義
typedef enum
{
	RESULT_SELECT_INC = 0,     // インクリメント
	RESULT_SELECT_DEC,         // デクリメント

}RESULT_SELECT_MOVE;

//プロトタイプ宣言
void InitResultUI(void);                      // リザルト画面の初期化処理
void UninitResultUI(void);                    // リザルト画面の終了処理
int UpdateResultUI(int pResultTimer);         // リザルト画面の更新処理
void DrawResultUI(void);                      // リザルト画面の描画処理
void ResultSelectMove(int nSelectMove);       // リザルトの選択肢を動かす処理


#endif
