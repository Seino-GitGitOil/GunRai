//--------------------------------------
//
// タイトル画面処理の宣言[title.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "main.h"
#include "title.h"
#include "sound.h"
#include "fade.h"

#include "input.h"


//選択肢の列挙型定義
typedef enum
{
	MAINMENU_GAMESTART = 0,     // ゲームスタート
	MAINMENU_RANKING,           // ランキング
	MAINMENU_GAMEQUIT,          // シャットダウン

}MAINMANU;

// テクスチャの列挙型定義
typedef enum
{
	TITLE_TEXTURE_BG = 0,               // 背景
	TITLE_TEXTURE_GAMESTART,            // ゲームスタート
	TITLE_TEXTURE_RANKING,              // ランキング
	TITLE_TEXTURE_GAMEQUIT,             // ゲーム終了
	TITLE_TEXTURE_BATTLINGGATTLING,     // ゲーム名
	TITLE_TEXTURE_CURSOR,               // カーソル
	TITLE_TEXTURE_MAX

}TITLE_TEXTURE_NAME;






//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTitle[TITLE_TEXTURE_MAX] = {};          // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;                       // 頂点バッファへのポインタ
static bool g_bEnterChecker = false;                                  // 遷移中かどうかのフラグ
int g_nMainMenuSelect;                                                // 選択肢

float g_fTitle_Cursor_X;                   // カーソルのX座標をずらす値
int g_nTitleTimer;                   // タイトル画面の間数え続けるタイマー

const char* g_aTitleTexture_Path[TITLE_TEXTURE_MAX] =
{
	"data//TEXTURE//TitleBG.jpg",
	"data//TEXTURE//GameStart_str.png",
	"data//TEXTURE//Ranking_str.png",
	"data//TEXTURE//Quit_str.png",
	"data//TEXTURE//BattlingGattling_str.png",
	"data//TEXTURE//Cursor.png",

};                 //背景テクスチャ

//----------------------------------
//   タイトル画面の初期化処理
//----------------------------------
void InitTitle(void)
{
	g_bEnterChecker = false;   //エンターチェックを初期化
	g_nMainMenuSelect = MAINMENU_GAMESTART;   //選択肢を初期化
	g_fTitle_Cursor_X = 0.0f;  // カーソルのずれ値を初期化
	g_nTitleTimer = 0;   // タイトルタイマーを初期化
	

	LPDIRECT3DDEVICE9 pDevice;  //デバイスのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < TITLE_TEXTURE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aTitleTexture_Path[nCnt],
			&g_apTextureTitle[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_TEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCnt= 0; nCnt < TITLE_TEXTURE_MAX; nCnt++)
	{
		//頂点座標の設定
		switch (nCnt)
		{
		case TITLE_TEXTURE_BG:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;
		case TITLE_TEXTURE_GAMESTART:
			pVtx[0].pos = D3DXVECTOR3(400.0f, 275.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 275.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 350.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 350.0f, 0.0f);
			break;
		case TITLE_TEXTURE_RANKING:
			pVtx[0].pos = D3DXVECTOR3(400.0f, 375.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 375.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 450.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 450.0f, 0.0f);
			break;
		case TITLE_TEXTURE_GAMEQUIT:
			pVtx[0].pos = D3DXVECTOR3(400.0f, 475.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(880.0f, 475.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 550.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(880.0f, 550.0f, 0.0f);
			break;
		case TITLE_TEXTURE_BATTLINGGATTLING:
			pVtx[0].pos = D3DXVECTOR3(300.0f, 25.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(980.0f, 25.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(300.0f, 225.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(980.0f, 225.0f, 0.0f);
			break;
		case TITLE_TEXTURE_CURSOR:
			pVtx[0].pos = D3DXVECTOR3(300.0f, 275.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(375.0f, 275.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(300.0f, 350.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(375.0f, 350.0f, 0.0f);
			break;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	//サウンドの再生(戦闘用BGM設定)
	PlaySound(SOUND_LABEL_BGM000);
}

//----------------------------------
//   タイトル画面の終了処理
//----------------------------------
void UninitTitle(void)
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TITLE_TEXTURE_MAX; nCnt++)
	{
		if (g_apTextureTitle[nCnt] != NULL)
		{
			g_apTextureTitle[nCnt]->Release();
			g_apTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}

//----------------------------------
//   タイトル画面の更新処理
//----------------------------------
void UpdateTitle(void)
{
	g_nTitleTimer++;  //タイマーをインクリメント

	VERTEX_2D* pVtx;  //頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCnt = 0; nCnt < TITLE_TEXTURE_MAX; nCnt++, pVtx += 4)
	{
		switch (nCnt)
		{
		case TITLE_TEXTURE_GAMESTART:

			switch (g_nMainMenuSelect)
			{
			case MAINMENU_GAMESTART:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			
			default:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				break;
			}

			break;
		case TITLE_TEXTURE_RANKING:

			switch (g_nMainMenuSelect)
			{
			case MAINMENU_RANKING:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			default:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				break;
			}

			break;
		case TITLE_TEXTURE_GAMEQUIT:

			switch (g_nMainMenuSelect)
			{
			case MAINMENU_GAMEQUIT:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			default:
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
				break;
			}

			break;

		case TITLE_TEXTURE_CURSOR:
			
			g_fTitle_Cursor_X = (sinf(g_nTitleTimer * 0.1f) + 1.0f) * 15.0f;

			// カーソル表示処理
			switch (g_nMainMenuSelect)
			{
			case MAINMENU_GAMESTART:
				pVtx[0].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 275.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 275.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 350.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 350.0f, 0.0f);
				break;
			case MAINMENU_RANKING:
				pVtx[0].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 375.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 375.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 450.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 450.0f, 0.0f);
				break;
			case MAINMENU_GAMEQUIT:
				pVtx[0].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 475.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 475.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(300.0f + g_fTitle_Cursor_X, 550.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(375.0f + g_fTitle_Cursor_X, 550.0f, 0.0f);
				break;
			}

			break;
		}
	}

	
	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	int FadeCheck = GetFade();

	if (FadeCheck == FADE_NONE || FadeCheck == FADE_OUT)
	{
		
		//---------------------------------
		// スティックでの上下カーソル移動
		//---------------------------------
		
		// 上移動
		if (GetKeyboardTrigger(DIK_W) == true ||
			GetKeyboardTrigger(DIK_UP) == true ||
			GetJoypadTrigger(JOYKEY_UP) == true ||
			GetJoypadStickUpTrigger() == true)
		{
			if (g_nMainMenuSelect == MAINMENU_GAMESTART)
			{
				g_nMainMenuSelect = MAINMENU_GAMEQUIT;
			}
			else
			{
				g_nMainMenuSelect--;
			}

			PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
		}
		// 下移動 
		else if (GetKeyboardTrigger(DIK_S) == true 
			|| GetKeyboardTrigger(DIK_DOWN) == true 
			|| GetJoypadTrigger(JOYKEY_DOWN) == true 
			|| GetJoypadStickDownTrigger() == true)
		{
			if (g_nMainMenuSelect == MAINMENU_GAMEQUIT)
			{
				g_nMainMenuSelect = MAINMENU_GAMESTART;
			}
			else
			{
				g_nMainMenuSelect++;
			}

			PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
		}



		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B) == true)
		{//決定キーが押された

			switch (g_nMainMenuSelect)
			{
			case MAINMENU_GAMESTART:

				if (g_bEnterChecker == false)
				{
					//モード設定
					SetFade(MODE_TUTORIAL);
					PlaySound(SOUND_LABEL_SE_ENTER);
					g_bEnterChecker = true;
				}

				break;

			case MAINMENU_RANKING:
				PlaySound(SOUND_LABEL_SE_ENTER);
				SetFade(MODE_RANKING);

				break;

			case MAINMENU_GAMEQUIT:
				PlaySound(SOUND_LABEL_SE_ENTER);

				UninitTitle();
				PostQuitMessage(0);
				break;
			}
		}
	}

	
}

//----------------------------------
//   タイトル画面の描画処理
//----------------------------------
void DrawTitle(void)
{

	LPDIRECT3DDEVICE9 pDevice;  //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int nCnt = 0; nCnt < TITLE_TEXTURE_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTitle[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

}