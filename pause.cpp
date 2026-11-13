//--------------------------------
//
// ポーズ処理[pause.cpp]
// Author：清野蒼翔
//
//--------------------------------
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"

//選択肢の列挙型定義
typedef enum
{
	PAUSE_RESUME = 0,     // ゲーム再開
	PAUSE_RETRY,          // リトライ
	PAUSE_EXIT,           // タイトルへ

}PAUSE_SELECT;

// ポーズ画面のUIテクスチャ
typedef enum
{
	PAUSE_TEX_MASK = 0,     // 画面を薄くするための半透明のテクスチャ
	PAUSE_TEX_CAPTION,      // 見出し
	PAUSE_TEX_RESUME,       // ゲーム再開
	PAUSE_TEX_RETRY,        // はじめから
	PAUSE_TEX_EXIT,         // タイトルへ戻る
	PAUSE_TEX_CURSOR,       // カーソル
	PAUSE_TEX_MAX

}PAUSE_TEXTURE;

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_TEX_MAX] = {};        //テクスチャ(３枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;  //頂点バッファへのポインタ


int g_nPauseSelect;       //ポーズ画面の選択肢
bool g_bEnterChecker;     //Enterを押したかのチェック
float g_fPause_Cursor_X;             // カーソルのX座標をずらす値
int g_nPauseTimer;                   // ポーズ画面の間数え続けるタイマー
bool g_bPause;     // ポーズ状態のON/OFF

const char* g_aPauseTexture_Path[PAUSE_TEX_MAX] =
{
	"data//TEXTURE//Pause_Mask.png",
	"data//TEXTURE//Pause_Caption.png",
	"data//TEXTURE//Pause_Resume.png",
	"data//TEXTURE//Pause_Retry.png",
	"data//TEXTURE//Pause_Retire.png",
	"data//TEXTURE//Cursor.png",

};                 //ポーズ画面テクスチャのパス

//----------------------------------------
// ポーズ画面の初期化処理
//----------------------------------------
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;  // デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	int nCntPause;
	g_nPauseSelect = PAUSE_RESUME;         // 選択肢を初期化
	g_bEnterChecker = false;    // エンターチェッカーを初期化
	g_bPause = false;           // ポーズモードをリセット
	g_fPause_Cursor_X = 0.0f;             // カーソルのX座標をずらす値
	g_nPauseTimer = 0;                   // ポーズ画面の間数え続けるタイマー

	//テクスチャを読み込み
	for (nCntPause = 0; nCntPause < PAUSE_TEX_MAX; nCntPause++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aPauseTexture_Path[nCntPause],
			&g_apTexturePause[nCntPause]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_TEX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;  //頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_TEX_MAX; nCntPause++)
	{
		switch (nCntPause)
		{
		case PAUSE_TEX_MASK:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			break;

		case PAUSE_TEX_CAPTION:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(450.0f, 25.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(830.0f, 25.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f, 225.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(830.0f, 225.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case PAUSE_TEX_RESUME:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(400.0f, 275.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 275.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 350.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 350.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case PAUSE_TEX_RETRY:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(400.0f, 375.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 375.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 450.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 450.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case PAUSE_TEX_EXIT:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(400.0f, 475.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 475.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 550.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 550.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case PAUSE_TEX_CURSOR:
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
			
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//-------------------------------
// ポーズ画面の終了処理
//-------------------------------
void UninitPause(void)
{
	int nCntPause;
	//テクスチャの破棄
	for (nCntPause = 0; nCntPause < PAUSE_TEX_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

}

//-------------------------------
// ポーズ画面の更新処理
//-------------------------------
void UpdatePause(void)
{
	g_nPauseTimer++;             // ポーズ画面の間数え続けるタイマー
	g_fPause_Cursor_X = (sinf(g_nPauseTimer * 0.1f) + 1.0f) * 15.0f;

	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += PAUSE_TEX_CURSOR * 4;
	// カーソル表示処理
	switch (g_nPauseSelect)
	{
	case PAUSE_RESUME:
		pVtx[0].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 275.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 275.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 350.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 350.0f, 0.0f);
		break;
	case PAUSE_RETRY:
		pVtx[0].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 375.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 375.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 450.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 450.0f, 0.0f);
		break;
	case PAUSE_EXIT:
		pVtx[0].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 475.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 475.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(300.0f + g_fPause_Cursor_X, 550.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(375.0f + g_fPause_Cursor_X, 550.0f, 0.0f);
		break;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();


	if (GetKeyboardTrigger(DIK_W) == true 
		|| GetKeyboardTrigger(DIK_UP) == true
		|| GetJoypadTrigger(JOYKEY_UP) == true
		|| GetJoypadStickUpTrigger() == true)
	{// 上を示すキーが押された

		if (g_nPauseSelect == PAUSE_RESUME)
		{//選択肢が一番上の場合
			g_nPauseSelect = PAUSE_EXIT;
		}
		else
		{
			g_nPauseSelect--;
		}

		PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
	}
	else if (GetKeyboardTrigger(DIK_S) == true 
		|| GetKeyboardTrigger(DIK_DOWN) == true
		|| GetJoypadTrigger(JOYKEY_DOWN) == true
		|| GetJoypadStickDownTrigger() == true)
	{// 下を示すキーが押された

		if (g_nPauseSelect == PAUSE_EXIT)
		{//選択肢が一番下の場合
			g_nPauseSelect = PAUSE_RESUME;
		}
		else
		{
			g_nPauseSelect++;
		}

		PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B)) && g_bEnterChecker == false)
	{//決定キーが押された

		switch (g_nPauseSelect)
		{
		case PAUSE_RESUME:
			if (g_bEnterChecker == false)
			{
				PlaySound(SOUND_LABEL_SE_ENTER);
				g_bPause = false;
			}

			break;

		case PAUSE_RETRY:
			
			SetFade(MODE_GAME);
			PlaySound(SOUND_LABEL_SE_ENTER);
			g_bEnterChecker = true;

			break;

		case PAUSE_EXIT:
			
			SetFade(MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_ENTER);
			g_bEnterChecker = true;
			break;
		}
	}
}

//-------------------------------
// ポーズ画面の描画処理
//-------------------------------
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_TEX_MAX; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}
//-------------------------
//  ポーズ情報を取得
//-------------------------
bool GetPause(void) 
{
	return g_bPause;
}
//---------------------------
// ポーズの反転処理
//---------------------------
void TogglePause(void)    
{
	g_bPause = g_bPause ^ 1;
}
//---------------------------
// ポーズの初期設定処理
//---------------------------
void SetPause(void)
{
	g_nPauseSelect = PAUSE_RESUME;         // 選択肢を初期化
}