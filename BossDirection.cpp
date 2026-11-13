//---------------------------------------
//
// ボス演出の処理 [BossDirection.cpp]
// Author：清野蒼翔
//
//---------------------------------------

#include "BossDirection.h"
#include "Boss.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"    //ただマクロほしかっただけ。"GAME_DISPLAY_X"
#include "sound.h"
#include "score.h"



// ボス演出のテクスチャの列挙型定義
typedef enum
{
    BOSS_DIR_TEX_ALERT = 0,
    BOSS_DIR_TEX_ALERT_STRING1,
    BOSS_DIR_TEX_ALERT_STRING2,

    BOSS_DIR_TEX_MAX

}BOSS_DIR_TEXTURE;

// アラート演出
typedef enum
{
    BOSS_DIR_ALERT = 0,
    BOSS_DIR_ALERT_STRING,
    BOSS_DIR_ALERT_MAX

}ALERT_BOSS_DIR;

//グローバル変数宣言
BOSS_DIR_STATE g_DirState;                                                // 演出の種類
int g_nDirTimer;                                                          // 演出タイマー
float g_fWarningAlpha;                                                    // 透明度
float g_AlertTexV;                                                        // 警告のテクスチャ座標の開始位置(V値)

LPDIRECT3DTEXTURE9 g_apTextureBossDirection[BOSS_DIR_TEX_MAX] = {};       // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossDirection = NULL;                   // 頂点バッファへのポインタ
const char* g_aBossDirectionTexture_Path[BOSS_DIR_TEX_MAX] =
{
    "data//TEXTURE//BossAlert.png",
    "data//TEXTURE//BossAlert_str1.png",
    "data//TEXTURE//BossAlert_str2.png"

};                 //エフェクトテクスチャ


//--------------------------------
//  ボス演出の初期化処理
//--------------------------------
void InitBossDirection(void)
{
    //デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = GetDevice();



    g_DirState = BOSS_DIR_NOTHING;    // 初期状態に
    g_nDirTimer = 0;                  // タイマー
    g_fWarningAlpha = 0.0f;           // ボス出現時の警告文の透明度
    g_AlertTexV = 0.0f;
    

    //テクスチャを読み込み
    for (int nCnt = 0; nCnt < BOSS_DIR_TEX_MAX; nCnt++)
    {
        D3DXCreateTextureFromFile(pDevice,
            g_aBossDirectionTexture_Path[nCnt],
            &g_apTextureBossDirection[nCnt]);
    }


    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BOSS_DIR_ALERT_MAX,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffBossDirection,
        NULL);





    //頂点情報の設定
    VERTEX_2D* pVtx;

    // 頂点バッファをロックし、頂点データへのポインタを取得
    g_pVtxBuffBossDirection->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntDir = 0; nCntDir < BOSS_DIR_ALERT_MAX; nCntDir++)
    {
        switch (nCntDir)
        {
        case BOSS_DIR_TEX_ALERT:
            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(GAME_DISPLAY_X, 0.0f, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, 0.0f, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);

            //rhwの設定
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
            pVtx[0].tex = D3DXVECTOR2(0.0f, g_AlertTexV);
            pVtx[1].tex = D3DXVECTOR2(1.0f, g_AlertTexV);
            pVtx[2].tex = D3DXVECTOR2(0.0f, g_AlertTexV + 1.0f);
            pVtx[3].tex = D3DXVECTOR2(1.0f, g_AlertTexV + 1.0f);
            break;

        case BOSS_DIR_ALERT_STRING:
            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(GAME_DISPLAY_X, 0.0f, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, 0.0f, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - GAME_DISPLAY_X, SCREEN_HEIGHT, 0.0f);

            //rhwの設定
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
            break;
        }
        pVtx += 4;
    }
    //頂点バッファをアンロックする
    g_pVtxBuffBossDirection->Unlock();
}
//--------------------------------
//  ボス演出の終了処理
//--------------------------------
void UninitBossDirection(void)
{
    //テクスチャの破棄
    for (int nCnt = 0; nCnt < BOSS_DIR_TEX_MAX; nCnt++)
    {
        if (g_apTextureBossDirection[nCnt] != NULL)
        {
            g_apTextureBossDirection[nCnt]->Release();
            g_apTextureBossDirection[nCnt] = NULL;
        }
    }

    //頂点バッファの破棄
    if (g_pVtxBuffBossDirection != NULL)
    {
        g_pVtxBuffBossDirection->Release();
        g_pVtxBuffBossDirection = NULL;
    }
}



//--------------------------------
//  ボス演出の更新処理
//--------------------------------
void UpdateBossDirection(void)
{
    BOSS* pBoss = GetBoss();
    if (g_DirState != BOSS_DIR_NOTHING)
    {
        g_nDirTimer++;
    }
    switch (g_DirState)
    {
    case BOSS_DIR_APPEAR:

        //頂点情報の設定
        VERTEX_2D* pVtx;

        // 頂点バッファをロックし、頂点データへのポインタを取得
        g_pVtxBuffBossDirection->Lock(0, 0, (void**)&pVtx, 0);


        for (int nCntDir = 0; nCntDir < BOSS_DIR_ALERT_MAX; nCntDir++)
        {
            switch (nCntDir)
            {
            case BOSS_DIR_ALERT:
                //テクスチャ座標の開始位置(V値)の更新
                g_AlertTexV -= 0.004f;

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(g_AlertTexV, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(g_AlertTexV + 1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(g_AlertTexV, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(g_AlertTexV + 1.0f, 1.0f);
                break;

            case BOSS_DIR_ALERT_STRING:
                g_fWarningAlpha = (sinf(g_nDirTimer * 0.1f) + 1.0f) * 0.5f;

                //頂点カラーの設定
                pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fWarningAlpha);   //(r,g,b,a)
                pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fWarningAlpha);
                pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fWarningAlpha);
                pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fWarningAlpha);
            }
            pVtx += 4;
        }
        //頂点バッファをアンロックする
        g_pVtxBuffBossDirection->Unlock();

        if (g_nDirTimer >= 180)
        {
            g_DirState = BOSS_DIR_SLIDE_IN; // スライドインへ

            g_nDirTimer = 0;                // たいまーリセット
        }
        break;

    case BOSS_DIR_SLIDE_IN:

        pBoss->pos.y += 2.0f;

        if (pBoss->pos.y >= BOSS_SIZE_Y + 20.0f) // 定位置についたら戦闘開始
        {
            pBoss->state = BOSSSTATE_NORMAL;
            g_DirState = BOSS_DIR_NOTHING;

            switch (pBoss->nType)
            {
            case BOSS_TYPE_A:
                PlaySound(SOUND_LABEL_BOSSBGM_1);
                break;

            case BOSS_TYPE_B:
                PlaySound(SOUND_LABEL_BOSSBGM_2);
                break;

            case BOSS_TYPE_C:
                PlaySound(SOUND_LABEL_BOSSBGM_3);
                break;
            }
        }
        break;

    case BOSS_DIR_LEAVE:

        // ボスのテクスチャ内にランダムで爆発を生成
        if (g_nDirTimer % 6 == 0)
        {
            D3DXVECTOR3 exPos = pBoss->pos;
            //exPos.x += (rand() % (int)BOSS_SIZE_X) - (BOSS_SIZE_X / 2);
            //exPos.y += (rand() % (int)BOSS_SIZE_Y) - (BOSS_SIZE_Y / 2);
            exPos.x += (rand() % (int)BOSS_SIZE_X) - ((int)BOSS_SIZE_X / 2);
            exPos.y += (rand() % (int)BOSS_SIZE_Y) - ((int)BOSS_SIZE_Y / 2);
            SetExplosion(exPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); // 既存の爆発関数
            PlaySound(SOUND_LABEL_SE_EXPLOSION);  // 爆発音
        }

        
        if (g_nDirTimer >= 180)
        {
            AddScore(10000000);
            pBoss->bUse = false;
            g_DirState = BOSS_DIR_NOTHING;
        }
        break;
    }
}

//-------------------------------
//  ボス演出の描画処理
//-------------------------------
void DrawBossDirection(void)
{
    if (g_DirState == BOSS_DIR_APPEAR)
    {
        //デバイスの取得
        LPDIRECT3DDEVICE9 pDevice = GetDevice();

        //ボス情報の取得
        BOSS* pBoss = GetBoss();


        //頂点バッファをデータストリームに設定
        pDevice->SetStreamSource(0, g_pVtxBuffBossDirection, 0, sizeof(VERTEX_2D));

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_2D);

        for (int nCnt = 0; nCnt < BOSS_DIR_ALERT_MAX; nCnt++)
        {

            //テクスチャの設定
            switch (nCnt)
            {
            case BOSS_DIR_ALERT_STRING:
                switch (pBoss->nType)
                {//ボスの種類によって表示する警告文を決定
                case BOSS_TYPE_A:
                    pDevice->SetTexture(0, g_apTextureBossDirection[BOSS_DIR_TEX_ALERT_STRING1]);
                    break;

                case BOSS_TYPE_B:
                    pDevice->SetTexture(0, g_apTextureBossDirection[BOSS_DIR_TEX_ALERT_STRING2]);
                    break;

                case BOSS_TYPE_C:
                    pDevice->SetTexture(0, g_apTextureBossDirection[BOSS_DIR_TEX_ALERT_STRING2]);
                    break;
                }

                break;

            // 念の為例外処理
            default:
                pDevice->SetTexture(0, g_apTextureBossDirection[nCnt]);

                break;
            }

            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
        }
    }
}


//--------------------------------
//  ボス演出の状態を取得
//--------------------------------
int GetBossDirState(void)
{
    return g_DirState;
}

//-----------------------------------
// 登場演出スタート
//-----------------------------------
void StartBossAppearDirection(void)
{
    BOSS* pBoss = GetBoss();  //ボスの情報を取得

    StopSound();

    g_DirState = BOSS_DIR_APPEAR;
    g_nDirTimer = 0;
    pBoss->bUse = true;

    switch (pBoss->nType)
    {
    case BOSS_TYPE_A:
        PlaySound(SOUND_LABEL_SE_BOSS_ALERT1);

        break;

    case BOSS_TYPE_B:
        PlaySound(SOUND_LABEL_SE_BOSS_ALERT2);

        break;

    case BOSS_TYPE_C:
        PlaySound(SOUND_LABEL_SE_BOSS_ALERT2);

        break;
    }

    
}
//-------------------------
// 退場演出スタート
//-------------------------
void StartBossLeaveDirection(void)
{
    BOSS* pBoss = GetBoss();
    g_DirState = BOSS_DIR_LEAVE;

    //ここに全ての弾をfalseにリセットする処理を入れよう
    DeleteBullet();
    
    g_nDirTimer = 0;

    pBoss->state = BOSSSTATE_DEAD;
    StopSound();
    /*  え何これキモイ
    switch (pBoss->nType)
    {
    case BOSS_TYPE_A:
        StopSound();
        break;

    case BOSS_TYPE_B:
        StopSound();
        break;

    case BOSS_TYPE_C:
        StopSound();
        break;
    }
    */
}