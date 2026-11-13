//--------------------------------
//
// 入力処理[input.cpp]
// Author：清野蒼翔
//
//--------------------------------
#include "input.h"

 //マクロ定義
#define NUM_KEY_MAX (256)   //キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;                //DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;    //入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];                 //キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];          //キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];          //キーボードのリリース情報         (仮)

XINPUT_STATE g_joyKeyState;  //ジョイパッドのプレス情報
XINPUT_STATE g_joyKeyStateTrigger;  // トリガー情報
XINPUT_STATE g_joyKeyStateRelease;  // リリース情報

// 左スティックの書く方向に対するトリガー処理
static bool g_bStickUpOld = false;
static bool g_bStickDownOld = false;
static bool g_bStickLeftOld = false;
static bool g_bStickRightOld = false;

//-------------------------------
//  キーボードの初期化処理
//-------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	return S_OK;
}
//--------------------------------
// キーボードの終了処理
//--------------------------------
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();  //キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

}

//--------------------------------
//  キーボードの更新処理
//--------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];  //キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];  //キーボードのトリガー情報を保存

			// (前回の状態 ^ 今回の状態) & 前回の状態  => 「前回は押されていて、今回は押されていない」状態を抽出しているらしい    調べたうえで未だに理解できていないので先生に聞いてみよう
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey]; 

			g_aKeyState[nCntKey] = aKeyState[nCntKey];  //キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();    //キーボードへのアクセス権を獲得
	}
}

//----------------------------------
//  キーボードのプレス情報を取得
//----------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
//----------------------------------
//  キーボードのトリガー情報を取得
//----------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
/*
//----------------------------------
//  キーボードのリリース情報を取得
//----------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
*/

//--------------------------------
//    ジョイパッドの初期化処理
//--------------------------------
HRESULT InitJoypad(void)
{
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&g_joyKeyStateRelease, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	return S_OK;
}

//--------------------------------
//    ジョイパッドの終了処理
//--------------------------------
void UninitJoypad(void)
{
	//
	XInputEnable(false);
}

//--------------------------------
//    ジョイパッドの更新処理
//--------------------------------
void UpdateJoypad(void)
{
	XINPUT_STATE joyKeyState;   //ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joyKeyState) == ERROR_SUCCESS)
	{
		//ジョイパッドのトリガー情報を保存
		g_joyKeyStateTrigger.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons) & joyKeyState.Gamepad.wButtons;
		
		//ジョイパッドのリリース情報を保存
		g_joyKeyStateRelease.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons) & g_joyKeyState.Gamepad.wButtons;
		
		//ジョイパッドのプレス情報を保存
		g_joyKeyState = joyKeyState;
	}
	else
	{
		// 接続が切れた場合はクリアしておく方がいいんじゃないかと思ったがこれUninitの持ってくるってのでよかったのかは不明
		memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
		memset(&g_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));
		memset(&g_joyKeyStateRelease, 0, sizeof(XINPUT_STATE));

		g_bStickUpOld = false;
		g_bStickDownOld = false;
		g_bStickLeftOld = false;
		g_bStickRightOld = false;
	}
}

//--------------------------------
//    ジョイパッドのプレス処理
//--------------------------------
bool GetJoypadPress(JOYKEY key)
{
	//return (g_joyKeyState.Gamepad.wButtons & (1 << key)) ? true : false;  
	// 16進数で書いた方が望ましいのかどうかは先生に聞いてみよう。
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//--------------------------------
//    ジョイパッドのトリガー処理
//--------------------------------
bool GetJoypadTrigger(JOYKEY key)
{
	//return (g_joyKeyStateTrigger.Gamepad.wButtons & (1 << key)) ? true : false;
	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;

}

//--------------------------------
//    ジョイパッドのリリース処理
//--------------------------------
bool GetJoypadRelease(JOYKEY key)
{
	//return (g_joyKeyStateRelease.Gamepad.wButtons & (1 << key)) ? true : false;
	return (g_joyKeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
}


//----------------------------------
//  左スティックのX軸値を取得 (-1.0f ～ 1.0f)
//----------------------------------
float GetJoypadStickX(void)
{
	SHORT sX = g_joyKeyState.Gamepad.sThumbLX;

	// XInput公式のデッドゾーン（8689）判定
	if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return (float)sX / 32767.0f;
	}
	else if (sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return (float)sX / 32768.0f;
	}

	return 0.0f; // デッドゾーン内は入力なし扱い
}

//----------------------------------------------
//  左スティックのY軸値を取得 (-1.0f ～ 1.0f)
//----------------------------------------------
float GetJoypadStickY(void)
{
	SHORT sY = g_joyKeyState.Gamepad.sThumbLY;

	// XInput公式のデッドゾーン判定
	if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return (float)sY / 32767.0f;
	}
	else if (sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return (float)sY / 32768.0f;
	}

	return 0.0f;
}


//------------------------------------------
//  左スティック上入力のトリガー判定
//------------------------------------------
bool GetJoypadStickUpTrigger(void)
{
	float fStickY = GetJoypadStickY();
	bool bNow = (fStickY > 0.5f); // 0.5f以上傾けたら「押された」と判定

	// トリガー判定
	bool bTrigger = (bNow && !g_bStickUpOld);

	g_bStickUpOld = bNow; // 今回の状態を記憶
	return bTrigger;
}

//---------------------------------------
//  左スティック下入力のトリガー判定
//---------------------------------------
bool GetJoypadStickDownTrigger(void)
{
	float fStickY = GetJoypadStickY();
	bool bNow = (fStickY < -0.5f); // -0.5f以下傾けたら「押された」と判定

	// トリガー判定
	bool bTrigger = (bNow && !g_bStickDownOld);

	g_bStickDownOld = bNow; // 今回の状態を記憶
	return bTrigger;
}

//---------------------------------------
//  左スティック左入力のトリガー判定
//---------------------------------------
bool GetJoypadStickLeftTrigger(void)
{
	float fStickX = GetJoypadStickX();
	bool bNow = (fStickX < -0.5f); // -0.5f以下傾けたら「押された」と判定

	// トリガー判定
	bool bTrigger = (bNow && !g_bStickLeftOld);

	g_bStickLeftOld = bNow; // 今回の状態を記憶
	return bTrigger;
}

//---------------------------------------
//  左スティック右入力のトリガー判定
//---------------------------------------
bool GetJoypadStickRightTrigger(void)
{
	float fStickX = GetJoypadStickX();
	bool bNow = (fStickX > 0.5f); // 0.5f以上傾けたら「押された」と判定

	// トリガー判定
	bool bTrigger = (bNow && !g_bStickRightOld);

	g_bStickRightOld = bNow; // 今回の状態を記憶
	return bTrigger;
}



