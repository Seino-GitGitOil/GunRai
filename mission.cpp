//-----------------------------------------
//
// ミッション達成か否かの処理[mission.cpp]
// Author：清野蒼翔
//
//-----------------------------------------

#include "game.h"
#include "player.h"
#include "lifeUI.h"
#include "BossDirection.h"
#include "mission.h"


#define MAX_MISSION (4)
#define CHECKER_WIDTH (275.0f)
#define CHECKER_HEIGHT (55.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureMissionCheck[MISSION_CHECK_MAX] = {};  // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissionCheck = NULL;               // 頂点バッファへのポインタ
Mission g_aMissionCheck[MAX_MISSION];      // ミッションチェック

int g_GrazeTimer;   //グレイズ後一定時間死んでいないかの確認

const char* g_aMissionCheckTexture_Path[MISSION_CHECK_MAX] =
{
	"data//TEXTURE//MissionFail.png",
	"data//TEXTURE//MissionClear.png"


};                 //エフェクトテクスチャ


//-----------------------------
//   ミッションの初期化処理
//-----------------------------
void InitCheckMission(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < MISSION_CHECK_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			g_aMissionCheckTexture_Path[nCnt],
			&g_apTextureMissionCheck[nCnt]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MISSION,                   //sizeof(VERTEX_2D) * [弾一つ描画するのに必要な頂点数] * MAX_MissionCheck
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMissionCheck,
		NULL);


	g_GrazeTimer = 0;

	for (int nCnt = 0; nCnt < MAX_MISSION; nCnt++)
	{// 構造体の内部データを初期化
		g_aMissionCheck[nCnt].bMissionFail = false;
		g_aMissionCheck[nCnt].bMissionClear = false;

	}


	//頂点情報の設定
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMissionCheck->Lock(0, 0, (void**)&pVtx, 0);

	float nAddHeight = 0.0f;

	for (int nCntMissionCheck = 0; nCntMissionCheck < MAX_MISSION; nCntMissionCheck++)
	{

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(50.0f, 480.0f + (CHECKER_HEIGHT * nCntMissionCheck), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f + CHECKER_WIDTH, 480.0f + (CHECKER_HEIGHT * nCntMissionCheck), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(50.0f, 480.0f + CHECKER_HEIGHT * (nCntMissionCheck + 1), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f + CHECKER_WIDTH, 480.0f + CHECKER_HEIGHT * (nCntMissionCheck + 1), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   //(r,g,b,a)
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;         //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMissionCheck->Unlock();


}
//-----------------------------
//   ミッションの終了処理
//-----------------------------
void UninitCheckMission(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < MISSION_CHECK_MAX; nCnt++)
	{
		if (g_apTextureMissionCheck[nCnt] != NULL)
		{
			g_apTextureMissionCheck[nCnt]->Release();
			g_apTextureMissionCheck[nCnt] = NULL;
		}
	}


	// 頂点バッファの破棄
	if (g_pVtxBuffMissionCheck != NULL)
	{
		g_pVtxBuffMissionCheck->Release();
		g_pVtxBuffMissionCheck = NULL;
	}
}
//-----------------------------
//   ミッションの更新処理
//-----------------------------
void UpdateCheckMission(void)
{
	Player* pPlayer = GetPlayer();
	int BossDirState = GetBossDirState();

	for (int nCnt = 0; nCnt < MAX_MISSION; nCnt++)
	{
		if (g_aMissionCheck[nCnt].bMissionClear == false && g_aMissionCheck[nCnt].bMissionFail == false)
		{//既にどっちかがtrueならいじらなくていいからね

			switch (nCnt)
			{
			case MISSION_NO_DEATH:
				if (pPlayer->nLifeStock < MAX_PLAYER_LIFE)
				{
					g_aMissionCheck[nCnt].bMissionFail = true;
					continue;
				}
				else if (BossDirState == BOSS_DIR_LEAVE)
				{//ボスの退場時に生きているなら
					g_aMissionCheck[nCnt].bMissionClear = true;

				}

				break;

			case MISSION_NO_OVERHEAT:
				if (pPlayer->nOverheat > 0)
				{//一度でもオーバーヒートをしたなら
					g_aMissionCheck[nCnt].bMissionFail = true;
					continue;
				}
				else if (BossDirState == BOSS_DIR_LEAVE)
				{//ボスの退場時にオーバーヒート回数が0なら
					g_aMissionCheck[nCnt].bMissionClear = true;

				}
				break;

			case MISSION_200_COMBO:
				if (pPlayer->nCombo >= 200)
				{
					g_aMissionCheck[nCnt].bMissionClear = true;
					continue;
				}
				else if (BossDirState == BOSS_DIR_LEAVE)
				{//ボスの退場時まで一度もコンボ数が200に至っていないなら
					g_aMissionCheck[nCnt].bMissionFail = true;

				}

				break;

			case MISSION_3_GRAZE:
				if (pPlayer->nGraze >= 3)
				{
					g_GrazeTimer++;
					
					if (pPlayer->PlayerState != PLAYERSTATE_DEATH)
					{//死んでいない場合
						
						if (g_GrazeTimer >= 6)
						{//グレイズしてから一定時間経った場合
							g_aMissionCheck[nCnt].bMissionClear = true;
							continue;
						}
						
					}
					else
					{//グレイズはしていたけど死んだなら

						g_GrazeTimer = 0;
					}
					
				}
				else if (BossDirState == BOSS_DIR_LEAVE)
				{//ボスの退場時にグレイズ回数が3を下回っているなら
					g_aMissionCheck[nCnt].bMissionFail = true;

				}
				break;
			}
		}
		
			

		
	}


	
}
//-----------------------------
//   ミッションの描画処理
//-----------------------------
void DrawCheckMission(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMissionCheck, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMissionCheck = 0; nCntMissionCheck < MAX_MISSION; nCntMissionCheck++)
	{
		if (g_aMissionCheck[nCntMissionCheck].bMissionClear == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMissionCheck[MISSION_CHECK_CLEAR]);
		}
		else if (g_aMissionCheck[nCntMissionCheck].bMissionFail == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMissionCheck[MISSION_CHECK_FAIL]);
			
		}
		else
		{
			continue;
		}

		
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMissionCheck * 4, 2);      //描画を開始する頂点インデックス
	}
}