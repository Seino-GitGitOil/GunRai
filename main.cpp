//---------------------------------------------------
//
// シューティング：メイン処理[main.cpp]
// Author：清野蒼翔
//
//---------------------------------------------------

#include "main.h"
#include "input.h"

#include "sound.h"

#include "title.h"
#include "Tutorial.h"
#include "game.h"
#include "result.h"
#include "Ranking.h"

#include "RankSetter.h"

#include "fade.h"



//マクロ定義
#define CLASS_NAME     "WindowsClass"                          // ウインドウクラスの名前
#define WINDOW_NAME  "銃雷 -GAN RAI-"          // ウインドウの名前（キャプションに表示）

#define ID_BUTTON_FINISH (101)               //終了ボタンのID
#define ID_BUTTON_COPY   (102)               //コピーボタンのID
#define ID_EDIT_INPUT    (111)               //入力ウインドウのID
#define ID_EDIT_OUTPUT   (121)               //出力ウインドウのID

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);    // 終了処理
void Update(void);    // 更新処理
void Draw(void);      // 描画処理

void DrawFPS(void);   // FPSの表示

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;               
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;   

LPD3DXFONT g_pFont = NULL;   // フォントへのポインタ
int g_nCountFPS = 0;         // FPSカウンタ

bool g_bDispDebug = false; // デバッグ表示のON/OFF

MODE g_mode = MODE_TITLE;   //現在のモード　(初期値は０でタイトル画面を表示)


//------------------------------
//        メイン関数            
//------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex =      //　wcex・・・ウインドウクラスの構造体
	{
		sizeof(WNDCLASSEX),                 //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                         //ウインドウのスタイル
		WindowProc,                         //ウインドウプロシージャ
		0,                                  //０にする(通常は使用しない)
		0,                                  //０にする(通常は使用しない)
		hInstance,                          //インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),    //タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),        //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),         //クライアント領域の背景色
		NULL,                               //メニューバー
		CLASS_NAME,                         //ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)     //ファイルのアイコン
	};

	HWND hWnd;
	MSG msg;
	
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };   //画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウ生成
	hWnd = CreateWindowEx(0,              //拡張ウインドウスタイル
		CLASS_NAME,                       //ウインドウクラスの名前
		WINDOW_NAME,                      //ウインドウの名前
		WS_OVERLAPPEDWINDOW,              //ウインドウスタイル
		125.0f,                           //ウインドウの左上X座標
		25.0f,                            //ウインドウの左上Y座標
		/*
		(rect.right - rect.left),         //ウインドウの幅
		(rect.bottom - rect.top),         //ウインドウの高さ
		*/
		1600,         //ウインドウの幅
		1000,         //ウインドウの高さ
		NULL,                             //親ウインドウのハンドル
		NULL,                             //メニューハンドルまたは子ウインドウID
		hInstance,                        //インスタンスハンドル
		NULL);                            //ウインドウ作成データ

	DWORD dwCurrentTime;   //現在時刻
	DWORD dwExecLastTime;  //最後に処理した時刻

	DWORD dwFrameCount;    //フレームカウント
	DWORD dwFPSLastTime;   //最後にFPSを計測した時刻


	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;                  //初期化する
	dwExecLastTime = timeGetTime();     //現在時刻を取得
	

	if (FAILED(Init(hInstance, hWnd, TRUE)))   // TRUEでウインドウ　FALSEで全画面
	{//初期化処理

		return -1;
	}

   //ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	dwFrameCount = 0;    //フレームカウント
	dwFPSLastTime = timeGetTime();   //最後にFPSを計測した時刻

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//ウインドウズの処理
			
			if (msg.message == WM_QUIT)
			{//WM_メッセージを受け取ったらメッセージループを抜ける
				break;
			}

			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{//DirectXの処理
			
			dwCurrentTime = timeGetTime();   //現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPS計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;      //FPSを測定した時刻を保存
				dwFrameCount = 0;                   //フレームカウントをクリア

			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の１秒経過

				dwExecLastTime = dwCurrentTime;  //処理開始の時刻 [現在時刻] を保存

				//更新処理
				Update();
				
				//描画処理
				Draw();

				dwFrameCount++;    //フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();
	
	//分解能を戻す
	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}
//-------------------------------
// ウインドウプロシージャ
//-------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;  //返り値を格納
	switch (uMsg)
	{
	case WM_KEYDOWN: //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了してよろしいですか？", "終了確認", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;

		}
		break;

	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_CLOSE:   //閉じるボタン(×ボタン)押下のメッセージ
		//ウインドウを破棄する(WM_DESTROYメッセージを送る)
		DestroyWindow(hWnd);
		/*
		nID = MessageBox(hWnd, "終了してよろしいですか？", "終了確認", MB_YESNO);
		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;    //０を返さないと終了してしまうのでね～
		}
		*/
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);   //既定の処理を返す
};




//-------------------------------
// 初期化処理
//-------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	//各種オブジェクトの初期化処理をここに
	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);         //テクスチャの繰り返しの設定(U)
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);         //テクスチャの繰り返しの設定(V)

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	

	//デバッグ表示用フォントの生成
	D3DXCreateFont(
		g_pD3DDevice,
		18,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}


	//サウンドの初期化処理
	InitSound(hWnd);

	//フェードの設定
	InitFade(g_mode);

	//ランキングスコアの初期化処理
	InitRankingScore();

	LoadRanking();

	//プレイヤー
	return S_OK;
}




//-------------------------------
//        終了処理
//-------------------------------
void Uninit(void)
{
	//タイトル画面の終了処理
	UninitTitle();
	
	//チュートリアル画面の終了処理
	UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//ランキング画面の終了処理
	UninitRanking();

	//ランキングスコアの終了処理
	UninitRankingScore();

	
	//サウンドの終了処理
	UninitSound();

	//キーボードの終了処理
	UninitKeyboard();

	//デバッグ表示用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}


	//ジョイパッドの終了処理
	UninitJoypad();

	//フェードの終了処理
	UninitFade();



	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}


//-------------------------------
// 更新処理
//-------------------------------
void Update(void)
{
	int FadeCheck = GetFade();

	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:  //タイトル画面
		UpdateTitle();
		break;
		
	case MODE_TUTORIAL: //チュートリアル画面
		UpdateTutorial();
		break;
		
	case MODE_GAME:   //ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT: //リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING: //ランキング画面
		UpdateRanking();
		break;
	}

	//フェードの更新処理
	UpdateFade();
	

	

}

//-------------------------------
// 描画処理
//-------------------------------
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;

		case MODE_RANKING:
			DrawRanking();
			break;
		}

		

		//フェードの描画処理
		DrawFade();

		


#ifdef _DEBUG        //デバッグの時のみ表示
		//FPSの表示
		DrawFPS();
#endif 

		

		//描画の終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------
// デバイスの取得
//--------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//--------------------------
// モードの設定
//--------------------------
void SetMode(MODE mode)
{
	switch (g_mode)
	{//現在の画面(モード)の終了処理

	case MODE_TITLE:       //タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:    //チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:        //ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:      //リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:     //ランキング画面
		UninitRanking();
		break;
	}

	 
	switch (mode)
	{//新しい画面(モード)の初期化処理

	case MODE_TITLE:       //タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:    //チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:        //ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:      //リザルト画面
		InitResult();
		break;

	case MODE_RANKING: //ランキング画面
		InitRanking();
		break;
	}
	g_mode = mode;    //モードを保存する
}

//--------------------------
// デバイスの取得
//--------------------------
MODE GetMode(void)
{
	return g_mode;   //現在のモードが何かを返す
}

//---------------------------
// FPSの表示
//---------------------------
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}