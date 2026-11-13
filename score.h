//-------------------------------------
//
//  スコア処理に関する宣言[score.h]
//  Author：清野蒼翔
//
//-------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_  //二重インクルード防止のマクロ

#include "enemy.h"

//マクロ定義
#define NUM_PLACE (8)     //スコアの桁数
#define NUM_HARF_WIDTH (15.0f)     //スコアの横幅の半分の長さ
#define NUM_HARF_HEIGHT (30.0f)     //スコアの縦幅の半分の長さ


//プロトタイプ宣言
void InitScore(void);                      //スコアの初期化処理
void UninitScore(void);                    //スコアの終了処理
void UpdateScore(void);                    //スコアの更新処理
void DrawScore(void);                      //スコアの描画処理

void SetScore(D3DXVECTOR3 pos, int nScore);             //スコアのセット処理
void AddScore(int nValue);             //スコアを加える処理
void AddKillScore(Enemy* pEnemy);      //キルスコアを加える処理
void AddBossHitScore();      //ボスにヒットした時のスコアを加える処理
int GetScore(void);

#endif
