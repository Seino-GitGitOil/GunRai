//--------------------------------------
//
// リザルト画面処理[result.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "main.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "score.h"
#include "ResultUI.h"
#include "ResultScore.h"

#include "input.h"



//グローバル変数
float g_fResult_Cursor_X;                   // カーソルのX座標をずらす値
int g_nResultTimer;
bool g_bResultEnterChecker;

//----------------------------------
//   リザルト画面の初期化処理
//----------------------------------
void InitResult(void)
{
	g_fResult_Cursor_X = 0.0f;              // カーソルのX座標をずらす値
	g_nResultTimer = 0;
	g_bResultEnterChecker = false;

	PlaySound(SOUND_LABEL_BGM002);


	InitResultUI();
	InitResultScore();
}

//----------------------------------
//   リザルト画面の終了処理
//----------------------------------
void UninitResult(void)
{
	StopSound();


	UninitResultUI();

	UninitResultScore();

}

//----------------------------------
//   リザルト画面の更新処理
//----------------------------------
void UpdateResult(void)
{
	g_nResultTimer++;


	int nResultSelect = UpdateResultUI(g_nResultTimer);  // 更新処理をしながら、現在の選択肢を取得

	if (g_nResultTimer >= 100) 
	{
		UpdateResultScore();

	}

	if (g_nResultTimer > 390)
	{
		if (GetKeyboardTrigger(DIK_A) == true 
			|| GetKeyboardTrigger(DIK_LEFT) == true
			|| GetJoypadTrigger(JOYKEY_LEFT) == true
			|| GetJoypadStickLeftTrigger() == true)
		{

			ResultSelectMove(RESULT_SELECT_DEC);
		}
		else if (GetKeyboardTrigger(DIK_D) == true 
			|| GetKeyboardTrigger(DIK_RIGHT) == true
			|| GetJoypadTrigger(JOYKEY_RIGHT) == true
			|| GetJoypadStickRightTrigger() == true)
		{

			ResultSelectMove(RESULT_SELECT_INC);
		}

		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B)) && g_bResultEnterChecker == false)
		{//決定キーが押された

			switch (nResultSelect)
			{
			case RESULT_MENU_RETURN_TITLE:
				//タイトルへ
				SetFade(MODE_TITLE);

				g_bResultEnterChecker = true;
				PlaySound(SOUND_LABEL_SE_ENTER);

				break;

			case RESULT_MENU_RANKING:
				//タイトルへ
				SetFade(MODE_RANKING);

				g_bResultEnterChecker = true;
				PlaySound(SOUND_LABEL_SE_ENTER);

				break;


			case RESULT_MENU_RESTART:
				//もう一度ゲームへ
				SetFade(MODE_GAME);

				g_bResultEnterChecker = true;
				PlaySound(SOUND_LABEL_SE_ENTER);

				break;
			}
		}
	}
}

//----------------------------------
//   リザルト画面の描画処理
//----------------------------------
void DrawResult(void)
{
	DrawResultUI();
	
	if (g_nResultTimer >= 120)
	{
		DrawResultScore();
	}
}