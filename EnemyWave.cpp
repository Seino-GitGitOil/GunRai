//---------------------------------------
//
// 敵のウェーブ処理[EnemyWave.cpp]
// Author：清野蒼翔
//
//---------------------------------------

#include "EnemyWave.h"
#include "enemy.h"
#include "Boss.h"
#include "game.h"
#include "main.h"
#include "score.h"
#include "player.h"
#include "RankSetter.h"


//プロトタイプ宣言
void WaveCheck(void);             //ウェーブ進行チェック　　これ多分この関数内でしか使わんからこれで良いと思われ

//グローバル変数宣言
ENEMY_SPAWN_INFO g_aSpawnTable[] = 
{
   //  ウェーブ数  タイミング       位置                     敵の種類
    
    //ウェーブ１
    
    { WAVE_ONE_FIRST, 60, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    
    { WAVE_ONE_FIRST, 180, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_FIRST, 240, D3DXVECTOR3(670.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_FIRST, 240, D3DXVECTOR3(610.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_FIRST, 360, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_FIRST, 360, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    
    { WAVE_ONE_SECOND, 60, D3DXVECTOR3(400.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 90, D3DXVECTOR3(460.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 120, D3DXVECTOR3(520.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 150, D3DXVECTOR3(580.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 180, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 210, D3DXVECTOR3(700.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 240, D3DXVECTOR3(760.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 270, D3DXVECTOR3(820.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_SECOND, 300, D3DXVECTOR3(880.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    
    
    { WAVE_ONE_THIRD, 60, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 60, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 120, D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 120, D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 150, D3DXVECTOR3(400.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_ONE_THIRD, 150, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_ONE_THIRD, 150, D3DXVECTOR3(880.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_ONE_THIRD, 180, D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 180, D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 240, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 240, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 300, D3DXVECTOR3(400.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 300, D3DXVECTOR3(880.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 360, D3DXVECTOR3(480.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 360, D3DXVECTOR3(800.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 420, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_ONE_THIRD, 460, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_ONE_THIRD, 460, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    
    //ウェーブ２
   
   
    { WAVE_TWO_FIRST, 60, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 60, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 60, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 120, D3DXVECTOR3(380.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 120, D3DXVECTOR3(900.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 180, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 240, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 300, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 300, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 420, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 450, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 480, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_FIRST, 510, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 510, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 570, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_FIRST, 590, D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 630, D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 690, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 690, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_FIRST, 750, D3DXVECTOR3(900.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_FIRST, 810, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    
    { WAVE_TWO_SECOND, 60, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 60, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 60, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 120, D3DXVECTOR3(400.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_SECOND, 120, D3DXVECTOR3(500.0f, 0.0f, 0.0f), ENEMY_TYPE_A },
    { WAVE_TWO_SECOND, 120, D3DXVECTOR3(600.0f, 0.0f, 0.0f), ENEMY_TYPE_A },

    { WAVE_TWO_SECOND, 150, D3DXVECTOR3(860.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_SECOND, 150, D3DXVECTOR3(900.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_SECOND, 150, D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 180, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 210, D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 240, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_TWO_SECOND, 300, D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_TWO_SECOND, 330, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    
    { WAVE_THREE_FIRST, 10,  D3DXVECTOR3(640.0f, 0.0f, 0.0f), ENEMY_TYPE_D },
    { WAVE_THREE_FIRST, 40,  D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_D },
    { WAVE_THREE_FIRST, 40,  D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_D },
    { WAVE_THREE_FIRST, 160, D3DXVECTOR3(380.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_THREE_FIRST, 160, D3DXVECTOR3(900.0f, 0.0f, 0.0f), ENEMY_TYPE_B },
    { WAVE_THREE_FIRST, 220, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_THREE_FIRST, 220, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_C },
    { WAVE_THREE_FIRST, 270, D3DXVECTOR3(440.0f, 0.0f, 0.0f), ENEMY_TYPE_E },
    { WAVE_THREE_FIRST, 270, D3DXVECTOR3(840.0f, 0.0f, 0.0f), ENEMY_TYPE_E },
    { WAVE_THREE_FIRST, 320, D3DXVECTOR3(540.0f, 0.0f, 0.0f), ENEMY_TYPE_E },
    { WAVE_THREE_FIRST, 320, D3DXVECTOR3(740.0f, 0.0f, 0.0f), ENEMY_TYPE_E },
 

    { WAVE_BOSS, 30, D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f - BOSS_SIZE_Y, 0.0f), BOSS_TYPE_A },   // 初期状態ではBOSSはAの方で

    { WAVE_MAX, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0 }

};                     // 敵のスポーン情報

WAVE_INFO g_WaveInfo;  // ウェーブの情報
int g_nCurrentWave;    // 現在のウェーブ数
int g_nSpawnedEnemy;   // 敵がスポーンした数を記録する変数

int g_nResult_End_Route;  // エンド分岐処理
int g_nAllSpawnedEnemy;   // スポーンした敵の全てをカウントする変数

//----------------------------------------
// ウェーブの初期化処理
//----------------------------------------
void InitWave(void)
{
	g_nCurrentWave = WAVE_ONE_FIRST;     // 現在のウェーブ数

	g_WaveInfo.nWaveTimer = 0;     // ウェーブごとにリセットするタイマー (単位：フレーム)
    g_nSpawnedEnemy = 0;           // ウェーブごとに敵がスポーンした数を記録する変数
    g_nResult_End_Route = END_NOTHING;
    g_nAllSpawnedEnemy = 0;

    
}

//-------------------------------
//    ウェーブの更新処理
//-------------------------------
void UpdateWave(void)
{
    g_WaveInfo.nWaveTimer++;

    for (int nCntWave = 0; g_aSpawnTable[nCntWave].wave < WAVE_MAX; nCntWave++)
    {
        
        if (g_aSpawnTable[nCntWave].wave == g_nCurrentWave && g_aSpawnTable[nCntWave].nSpawnTime == g_WaveInfo.nWaveTimer)
        {// 現在のウェーブかつ、タイマーの時間が一致したら敵を生成

            if (g_aSpawnTable[nCntWave].wave != WAVE_BOSS)
            {
                SetEnemy(g_aSpawnTable[nCntWave].pos, g_aSpawnTable[nCntWave].nType);
                g_nSpawnedEnemy++;
                g_nAllSpawnedEnemy++;
            }
            else if(g_aSpawnTable[nCntWave].wave == WAVE_BOSS)
            {// 最終ウェーブに到達した場合

                Player* pPlayer = GetPlayer();

                // 出現するボスを変更する条件式
                if (pPlayer->nKill == 0)
                {// プレイヤーのキル数が「０」の場合  =  αルート
                    g_aSpawnTable[nCntWave].nType = BOSS_TYPE_B;
                }
                else if (pPlayer->nKill >= g_nAllSpawnedEnemy)
                {// プレイヤーが全ての敵を殺している場合  =  Ωルート
                    
                    g_aSpawnTable[nCntWave].nType = BOSS_TYPE_C;
                }
                else
                {
                    g_aSpawnTable[nCntWave].nType = BOSS_TYPE_A;
                }

                SetBoss(g_aSpawnTable[nCntWave].pos, g_aSpawnTable[nCntWave].nType);
                g_nSpawnedEnemy++;
                g_nAllSpawnedEnemy++;
            }
        }
    }

    //ウェーブへ進む条件チェック
    WaveCheck();
}

//---------------------------------------
// 現在のウェーブ数を取得  　　要らんかったら消せ
//---------------------------------------
int GetCurrentWave(void)
{
	return g_nCurrentWave;   // 現在のウェーブ数を返す(今何ウェーブか、とか表示するときとかに使うと思うんでここで１増やしてしまおう)
}

//-------------------------------
// 到達したエンドルートを取得
//-------------------------------
int GetEndRoute(void)
{
    return g_nResult_End_Route;
}

//---------------------------------------------
// ウェーブ進行条件を満たしてるかのチェック
//---------------------------------------------
void WaveCheck(void)
{
    int nowWave = g_nCurrentWave;  // 今のウェーブ数を代入
    int nTargetSpawnCount = 0;     // 現在のウェーブに出現する予定の敵の総数をカウントする変数
    
    

    // 現在のウェーブの「予定総出現数」をカウント
    for (int nCnt = 0; g_aSpawnTable[nCnt].wave < WAVE_MAX; nCnt++)
    {
        if (g_aSpawnTable[nCnt].wave == nowWave)    //「現在のウェーブに出現すると設定されている情報」のみを配列から抜き取る
        {//見つかったら

            nTargetSpawnCount++;
        }
    }

    
    if (g_nSpawnedEnemy >= nTargetSpawnCount)
    {// 敵が全員出現している場合

        // 現在画面上に残っている敵の数をチェック
        if (nowWave != WAVE_BOSS)
        {
            int nActiveEnemyCount = 0;   // 生き残っている敵をカウントする変数

            Enemy* pEnemy = GetEnemy();  // enemy.cpp から敵情報を取得

            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
            {
                if (pEnemy[nCntEnemy].bUse == true)
                {
                    nActiveEnemyCount++;
                }
            }


            //「画面内の敵が０」なら次のウェーブへ移行する
            if (nActiveEnemyCount == 0)
            {
                g_nCurrentWave++;          // 次のウェーブへ進む
                g_nSpawnedEnemy = 0;       // スポーンした敵の数をリセット
                g_WaveInfo.nWaveTimer = 0; // タイマーをリセット
            }
        }
        else if (nowWave == WAVE_BOSS)
        {
            BOSS* pBoss = GetBoss();  //ボスの情報を取得
            Player* pPlayer = GetPlayer();
            int nScore = GetScore();  //スコアを取得

            if (pBoss->bUse != true)
            {// ボスが撃破されたとき

                g_nCurrentWave++;
                g_nSpawnedEnemy = 0;
                g_WaveInfo.nWaveTimer = 0;

                switch (pBoss->nType)
                {
                case BOSS_TYPE_A:
                    //Nルート
                    g_nResult_End_Route = END_NORMAL;
                    break;
                
                case BOSS_TYPE_B:
                    //αルート
                    g_nResult_End_Route = END_NOT_KILLED;
                    break;

                case BOSS_TYPE_C:
                    //Ωルート
                    g_nResult_End_Route = END_ALL_KILLED;
                    break;

                }

                



                UpdateRankingScore(nScore, g_nResult_End_Route);        //ランキングの更新・テキストファイルに保存

            }
        }
    }
}