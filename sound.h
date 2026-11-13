//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	//BGM
	SOUND_LABEL_BGM000 = 0,          // BGM0 メニュー画面
	SOUND_LABEL_BGM_TUTORIAL,          // チュートリアル
	SOUND_LABEL_BGM001,              // BGM1 道中雑魚戦
	SOUND_LABEL_BOSSBGM_1,           //
	SOUND_LABEL_BOSSBGM_2,           //
	SOUND_LABEL_BOSSBGM_3,           //
	SOUND_LABEL_BGM002,			     // BGM2 リザルト画面
	
	// UIシステム等
	SOUND_LABEL_SE_CURSOR_MOVE,      // カーソル移動
	SOUND_LABEL_SE_ENTER,            // エンター
	SOUND_LABEL_SE_ERROR,            // エラー音
	SOUND_LABEL_SE_COUNTUP,          // カウントアップ音


	//プレイヤー関係
	SOUND_LABEL_SE_SHOT,		     // 弾発射音
	SOUND_LABEL_SE_SHIELD_BREAK,	 // シールドブレイク音
	SOUND_LABEL_SE_BOMB,             // ボム音
	SOUND_LABEL_SE_PLAYER_LOST,   	 // プレイヤー被弾音
	SOUND_LABEL_SE_RESPAWN_1,	     // リスポーン１
	SOUND_LABEL_SE_RESPAWN_2,	     // リスポーン２
	SOUND_LABEL_SE_RELOAD,	         // リロード 
	SOUND_LABEL_SE_BURST,	         // バースト
	SOUND_LABEL_SE_GRAZE,            // グレイズ時の音
	SOUND_LABEL_SE_BURST_MAX,        // バースト最大時
	
	//演出音
	SOUND_LABEL_SE_BOSS_ALERT1,
	SOUND_LABEL_SE_BOSS_ALERT2,

	//その他戦闘音
	SOUND_LABEL_SE_EXPLOSION,	     // 爆発音
	
	SOUND_LABEL_MAX,

} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
