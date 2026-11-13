//---------------------------------------
//
// ランクのセットやロード、セーブに関する処理[RankSetter.cpp]
// Author: 清野蒼翔
//
//---------------------------------------

#include "main.h"
#include "RankSetter.h"
#include "EnemyWave.h"
#include "score.h"
#include <stdio.h>

#define NUM_PLACE_RANKING (24)    //８桁×3人分


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRankingScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;
D3DXVECTOR3 g_posRankingScore;             // スコアの位置
RANKING_DATA g_aRanking[MAX_RANKING];      // 上位3位 + プレイヤーのスコア

const char* FILE_PATH = "data/score.txt";


//----------------------------------------------
//  ランキングスコアを表示する際の初期化処理
//----------------------------------------------
void InitRankingScore(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    VERTEX_2D* pVtx;

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice,
        "data//TEXTURE//RankScore.png",
        &g_pTextureRankingScore);

    g_posRankingScore = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //位置を初期化する


    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE_RANKING,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffRankingScore,
        NULL);

    //頂点情報の設定   ８桁分だね

    //頂点バッファをロックし、頂点データへのポインタを取得
    g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCnt = 0; nCnt < NUM_PLACE_RANKING; nCnt++)
    {
        // 頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posRankingScore.y - NUM_HARF_HEIGHT, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posRankingScore.y - NUM_HARF_HEIGHT, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (NUM_HARF_WIDTH * 2 * nCnt) - NUM_HARF_WIDTH, g_posRankingScore.y + NUM_HARF_HEIGHT, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (NUM_HARF_WIDTH * 2 * nCnt) + NUM_HARF_WIDTH, g_posRankingScore.y + NUM_HARF_HEIGHT, 0.0f);

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
    g_pVtxBuffRankingScore->Unlock();
}
//-------------------------------------
//  ランキングスコアの終了処理
//-------------------------------------
void UninitRankingScore(void)
{
    //テクスチャの破棄
    if (g_pTextureRankingScore != NULL)
    {
        g_pTextureRankingScore->Release();
        g_pTextureRankingScore = NULL;
    }

    //頂点バッファの破棄
    if (g_pVtxBuffRankingScore != NULL)
    {
        g_pVtxBuffRankingScore->Release();
        g_pVtxBuffRankingScore = NULL;
    }
}
//---------------------------------
//  ランキングスコアの描画処理
//---------------------------------
void DrawRankingScore(void)
{
    int NowMode = GetMode();

    if (NowMode == MODE_GAME || NowMode == MODE_RANKING)
    {
        //デバイスの取得
        LPDIRECT3DDEVICE9 pDevice = GetDevice();

        //頂点バッファをデータストリームに設定
        pDevice->SetStreamSource(0, g_pVtxBuffRankingScore, 0, sizeof(VERTEX_2D));

        //頂点フォーマットの設定
        pDevice->SetFVF(FVF_VERTEX_2D);

        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureRankingScore);

        for (int nCnt = 0; nCnt < NUM_PLACE_RANKING; nCnt++)
        {
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);      //描画を開始する頂点インデックス

        }
    }
}
//-------------------------------
//  ランキングスコアのセット処理
//-------------------------------
void SetRankingScore(D3DXVECTOR3 pos, float fScoreLength_x, float fScoreLength_y, float fSpace)
{
    int NowMode = GetMode();
    
    float nSpaceHeight = 0.0f;

    //頂点情報の設定
    VERTEX_2D* pVtx;

    //頂点バッファをロックし、頂点データへのポインタを取得
    g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

    /*
    switch (NowMode)
    {
    case MODE_GAME:
        MaxRank = MAX_RANKING - 1;
        break;

    case MODE_RANKING:
        MaxRank = MAX_RANKING;
        break;

    default:
        MaxRank = 0;
    }
    */
    for (int nCntRank = 0; nCntRank < MAX_RANKING - 1; nCntRank++)
    {//順位の数分繰り返す

        int nDev = 10000000;     // 一番最初の「割る」数　を設定

        

        for (int nCnt = 0; nCnt < 8; nCnt++)
        {//８桁分繰り返す

            // 頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(pos.x + (fScoreLength_x * 2 * nCnt) - fScoreLength_x, pos.y + (fScoreLength_y * 2 * nCntRank) - fScoreLength_y + nSpaceHeight, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(pos.x + (fScoreLength_x * 2 * nCnt) + fScoreLength_x, pos.y + (fScoreLength_y * 2 * nCntRank) - fScoreLength_y + nSpaceHeight, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(pos.x + (fScoreLength_x * 2 * nCnt) - fScoreLength_x, pos.y + (fScoreLength_y * 2 * nCntRank) + fScoreLength_y + nSpaceHeight, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(pos.x + (fScoreLength_x * 2 * nCnt) + fScoreLength_x, pos.y + (fScoreLength_y * 2 * nCntRank) + fScoreLength_y + nSpaceHeight, 0.0f);


            //テクスチャ座標の設定
            pVtx[0].tex = D3DXVECTOR2(0.1f * (g_aRanking[nCntRank].nScore % (nDev * 10) / nDev), 0.0f);
            pVtx[1].tex = D3DXVECTOR2(0.1f * (g_aRanking[nCntRank].nScore % (nDev * 10) / nDev + 1), 0.0f);
            pVtx[2].tex = D3DXVECTOR2(0.1f * (g_aRanking[nCntRank].nScore % (nDev * 10) / nDev), 1.0f);
            pVtx[3].tex = D3DXVECTOR2(0.1f * (g_aRanking[nCntRank].nScore % (nDev * 10) / nDev + 1), 1.0f);


            nDev /= 10;        //割る数・割られる数の桁を１減らす
            pVtx += 4;         //頂点データのポインタを４つ分進める
        }

        nSpaceHeight += fSpace;
    }

    //頂点バッファをアンロックする
    g_pVtxBuffRankingScore->Unlock();
}

//---------------------------------------------------
// ランキングの初期化（ファイルから読み込む）
//---------------------------------------------------
void LoadRanking(void)
{
    FILE* pFile = NULL;

    pFile = fopen(FILE_PATH, "r");
    if (pFile != NULL)
    {//ファイルが開けた場合

        for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
        {
            //ファイルから文字列を読み込む
            fscanf(pFile, "%d", &g_aRanking[nCnt].nScore);
            fscanf(pFile, "%d", &g_aRanking[nCnt].nEnd);
            
        }
        //ファイルを閉じる
        fclose(pFile);
    }
    else
    {//ファイルが開けなかった場合

        // 初回起動時など、ファイルが開けなかった場合の初期設定も兼ねて一応,,,
        for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
        {
            g_aRanking[nCnt].nScore = 0;
            g_aRanking[nCnt].nEnd = END_NOTHING;
        }
    }
}


//--------------------------------------------------
// ランキングのファイル保存
//--------------------------------------------------
void SaveRanking(void)
{
    FILE* pFile = NULL;

    //ファイルを開く
    pFile = fopen(FILE_PATH, "w");

    if (pFile != NULL)
    {//ファイルが開けた場合
        for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
        {
            //ファイルに文字列を書き出す
            fprintf(pFile, "%d\n", g_aRanking[nCnt].nScore);
            fprintf(pFile, "%d\n", g_aRanking[nCnt].nEnd);

        }
        //ファイルを閉じる
        fclose(pFile);
    }
}

// --------------------------------------------------
// 新しいスコアをランキングに反映(Updateって名にしたけどこれは毎フレーム実行しなくていいやんな)
// --------------------------------------------------
void UpdateRankingScore(int nNewScore, int nEndRoute)
{
    /*
    // 最下位のスコアより高い場合のみランキング更新処理を実行
    if (nNewScore > g_aRanking[MAX_RANKING - 1].nScore)
    {
        // 最下位（末尾）の枠に新しいデータをセット
        g_aRanking[MAX_RANKING - 1].nScore = nNew   Score;
        g_aRanking[MAX_RANKING - 1].nEnd = nEndRoute;

        // 降順バブルソート（スコアが高い順に並べ替え）
        for (int nCount = 0; nCount < MAX_RANKING - 1; nCount++)
        {
            for (int nCount2 = 0; nCount2 < MAX_RANKING - nCount - 1; nCount2++)
            {
                if (g_aRanking[nCount2].nScore < g_aRanking[nCount2 + 1].nScore)
                {//その値が次の項の値よりも少なかった場合

                    // 構造体ごと丸ごと入れ替え
                    RANKING_DATA aTemp = g_aRanking[nCount2];
                    g_aRanking[nCount2] = g_aRanking[nCount2 + 1];
                    g_aRanking[nCount2 + 1] = aTemp;
                }
            }
        }

        // 更新された内容をファイルへ保存
        SaveRanking();
    }*/

    // 最下位（末尾）の枠に新しいデータをセット
    g_aRanking[MAX_RANKING - 1].nScore = nNewScore;
    g_aRanking[MAX_RANKING - 1].nEnd = nEndRoute;

    // 降順バブルソート（スコアが高い順に並べ替え）
    for (int nCount = 0; nCount < MAX_RANKING - 1; nCount++)
    {
        for (int nCount2 = 0; nCount2 < MAX_RANKING - nCount - 1; nCount2++)
        {
            if (g_aRanking[nCount2].nScore < g_aRanking[nCount2 + 1].nScore)
            {//その値が次の項の値よりも少なかった場合

                // 構造体ごと丸ごと入れ替え
                RANKING_DATA aTemp = g_aRanking[nCount2];
                g_aRanking[nCount2] = g_aRanking[nCount2 + 1];
                g_aRanking[nCount2 + 1] = aTemp;
            }
        }
    }

    // 更新された内容をファイルへ保存
    SaveRanking();
}
