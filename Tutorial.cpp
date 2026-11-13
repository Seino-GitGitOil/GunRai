//--------------------------------------
//
// チュートリアル画面処理の宣言[Tutorial.cpp]
// Author：清野蒼翔
//
//--------------------------------------

#include "main.h"
#include "sound.h"
#include "fade.h"

#include "input.h"


// テクスチャの列挙型定義
typedef enum
{
	TUTORIAL_TEXTURE_1 = 0,
	TUTORIAL_TEXTURE_2,
	TUTORIAL_TEXTURE_3,
	TUTORIAL_TEXTURE_4,
	TUTORIAL_TEXTURE_CURSOR_LEFT,
	TUTORIAL_TEXTURE_CURSOR_RIGHT,
	TUTORIAL_TEXTURE_MAX

}TUTORIAL_TEXTURE;


typedef enum
{
	TUTORIAL_BUFFER_BG_1 = 0,
	TUTORIAL_BUFFER_BG_2,
	TUTORIAL_BUFFER_BG_3,
	TUTORIAL_BUFFER_BG_4,
	TUTORIAL_BUFFER_CURSOR_LEFT,
	TUTORIAL_BUFFER_CURSOR_RIGHT,
	TUTORIAL_BUFFER_MAX

}TUTORIAL_TEX_BUFFER;




//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTutorial[TUTORIAL_TEXTURE_MAX] = {};          // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;                       // 頂点バッファへのポインタ
bool g_bTutorialEnterChecker = false;                                  // 遷移中かどうかのフラグ
int g_NowPage;                                                       // 今のページ
float g_Alpha[TUTORIAL_BUFFER_MAX];                                 // 各バッファの透明度
const char* g_aTutorialTexture_Path[TUTORIAL_TEXTURE_MAX] =
{
	"data//TEXTURE//Tutorial01.png",
	"data//TEXTURE//Tutorial02.png",
	"data//TEXTURE//Tutorial03.png",
	"data//TEXTURE//Tutorial04.png",
	"data//TEXTURE//Cursor03.png",
	"data//TEXTURE//Cursor04.png",

};                 //背景テクスチャ

//----------------------------------
//   ランキング画面の初期化処理
//----------------------------------
void InitTutorial(void)
{
	g_bTutorialEnterChecker = false;   //エンターチェックを初期化
	g_NowPage = TUTORIAL_TEXTURE_1;  //現在のページ

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < TUTORIAL_TEXTURE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aTutorialTexture_Path[nCnt],
			&g_apTextureTutorial[nCnt]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIAL_BUFFER_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//頂点情報の設定
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_BUFFER_MAX; nCnt++)
	{
		g_Alpha[nCnt] = 0.0f;  //透明度を初期化

		//頂点座標の設定
		switch (nCnt)
		{
		case TUTORIAL_BUFFER_BG_1:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;

		case TUTORIAL_BUFFER_BG_2:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;

		case TUTORIAL_BUFFER_BG_3:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;

		case TUTORIAL_BUFFER_BG_4:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			break;

		case TUTORIAL_BUFFER_CURSOR_LEFT:
			pVtx[0].pos = D3DXVECTOR3(0.0f, 320.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(40.0f, 320.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(40.0f, 400.0f, 0.0f);
			break;

		case TUTORIAL_BUFFER_CURSOR_RIGHT:
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 320.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 320.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 400.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 400.0f, 0.0f);
			break;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	PlaySound(SOUND_LABEL_BGM_TUTORIAL);

}

//----------------------------------
//   タイトル画面の終了処理
//----------------------------------
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TUTORIAL_TEXTURE_MAX; nCnt++)
	{
		if (g_apTextureTutorial[nCnt] != NULL)
		{
			g_apTextureTutorial[nCnt]->Release();
			g_apTextureTutorial[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

}

//----------------------------------
//   タイトル画面の更新処理
//----------------------------------
void UpdateTutorial(void)
{
	VERTEX_2D* pVtx;  //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_NowPage)
	{
	case TUTORIAL_TEXTURE_1:
		g_Alpha[TUTORIAL_BUFFER_BG_1] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_2] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_3] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_4] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_LEFT] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_RIGHT] = 1.0f;
		break;

	case TUTORIAL_TEXTURE_2:
		g_Alpha[TUTORIAL_BUFFER_BG_1] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_2] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_3] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_4] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_LEFT] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_RIGHT] = 1.0f;
		break;

	case TUTORIAL_TEXTURE_3:
		g_Alpha[TUTORIAL_BUFFER_BG_1] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_2] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_3] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_4] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_LEFT] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_RIGHT] = 1.0f;
		break;

	case TUTORIAL_TEXTURE_4:
		g_Alpha[TUTORIAL_BUFFER_BG_1] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_2] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_3] = 0.0f;
		g_Alpha[TUTORIAL_BUFFER_BG_4] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_LEFT] = 1.0f;
		g_Alpha[TUTORIAL_BUFFER_CURSOR_RIGHT] = 0.0f;
		break;

	}
	for (int nCnt = 0; nCnt < TUTORIAL_BUFFER_MAX; nCnt++)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alpha[nCnt]);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alpha[nCnt]);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alpha[nCnt]);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alpha[nCnt]);

		pVtx += 4;
	}
	

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	if (g_bTutorialEnterChecker == false)
	{
		if (GetKeyboardTrigger(DIK_D) == true
			|| GetKeyboardTrigger(DIK_RIGHT) == true
			|| GetJoypadTrigger(JOYKEY_RIGHT) == true
			|| GetJoypadStickRightTrigger() == true)
		{
			if (g_NowPage == TUTORIAL_TEXTURE_4)
			{
				PlaySound(SOUND_LABEL_SE_ERROR);
			}
			else
			{
				g_NowPage++;
				PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);
			}

		}
		// 左移動
		else if (GetKeyboardTrigger(DIK_A) == true 
			|| GetKeyboardTrigger(DIK_LEFT) == true
			|| GetJoypadTrigger(JOYKEY_LEFT) == true 
			|| GetJoypadStickLeftTrigger() == true)
		{
			if (g_NowPage == TUTORIAL_TEXTURE_1)
			{
				PlaySound(SOUND_LABEL_SE_ERROR);
			}
			else
			{
				g_NowPage--;
				PlaySound(SOUND_LABEL_SE_CURSOR_MOVE);

			}
		}


		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_B) == true) && g_NowPage == TUTORIAL_TEXTURE_4)
		{//決定キーが押された

			//モード設定
			SetFade(MODE_GAME);
			PlaySound(SOUND_LABEL_SE_ENTER);
			g_bTutorialEnterChecker = true;
		}
	}

	
}

//----------------------------------
//   タイトル画面の描画処理
//----------------------------------
void DrawTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTORIAL_TEXTURE_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureTutorial[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

}