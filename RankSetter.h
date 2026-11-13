//---------------------------------------------------------
//
// ランクのセット、ロード、セーブ処理の宣言[RankSetter.h]
// Author：清野蒼翔
//
//---------------------------------------------------------

#ifndef _RANK_SETTER_H_
#define _RANK_SETTER_H_

#include "main.h"

#define MAX_RANKING (4) // 1位～3位、それにプラスでプレイヤーのスコアまでほぞんしたいからね
//  こんがらがりそうだからメモ    [0] = １位, [1] = ２位 [2] = ３位  [3] = Player

typedef enum
{
    
    END_NORMAL = 0,          // 通常ルート
    END_ALL_KILLED,          // 全殺しルート
    END_NOT_KILLED,          // 不殺ルート
    END_NOTHING              // ルート未到達(ゲームクリアならず)

}NUM_END_ROUTE;



// ランキングデータ構造体
typedef struct
{
    int nScore;     // スコア（8桁の整数）
    int nEnd;       // 到達ルート

} RANKING_DATA;

// プロトタイプ宣言
void InitRankingScore(void);          // 初期化
void UninitRankingScore(void);        // 終了
void DrawRankingScore(void);          // 描画
void SetRankingScore(D3DXVECTOR3 pos, float fScoreLength, float fScoreLength_y, float fSpace);           // スコアをセット

void SaveRanking(void);            // ファイルへの保存
void LoadRanking(void);            // ファイルからの読み込み
void UpdateRankingScore(int nNewScore, int nRoute); // 新規スコアの順位反映


#endif
