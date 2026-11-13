//-------------------------------
//
// 入力処理の宣言[input.h]
// Author：清野蒼翔
//
//-------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//ジョイパッドのキー情報
typedef enum
{
	JOYKEY_UP = 0,
	JOYKEY_DOWN,
	JOYKEY_LEFT,
	JOYKEY_RIGHT,
	JOYKEY_START,

	

	JOYKEY_L1 = 8,
	JOYKEY_R1,
	JOYKEY_L2,	// Lトリガー (デジタル入力)
	JOYKEY_R2,
	
	JOYKEY_A = 12,
	JOYKEY_B,
	JOYKEY_X,
	JOYKEY_Y,

	// システムボタン
	


}JOYKEY;


//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);   //キーボード入力の初期化処理
void UninitKeyboard(void);            //キーボード入力の終了処理
void UpdateKeyboard(void);            //キーボード入力の更新処理
bool GetKeyboardPress(int nKey);      //プレス処理
bool GetKeyboardTrigger(int nKey);    //トリガー処理
bool GetKeyboardRelease(int nKey);    //リリース処理                (仮)

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);

float GetJoypadStickX(void);
float GetJoypadStickY(void);
bool GetJoypadStickUpTrigger(void);    // スティック上のトリガー取得
bool GetJoypadStickDownTrigger(void);  // スティック下のトリガー取得
bool GetJoypadStickLeftTrigger(void);    // スティック左のトリガー取得
bool GetJoypadStickRightTrigger(void);  // スティック右のトリガー取得

#endif
