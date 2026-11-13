//-----------------------------------------
//
// ボムの処理の宣言 [bomb.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _BOMB_H_
#define _BOMB_H_  //二重インクルード防止のマクロ



//マクロ定義


//ボム構造体
typedef struct
{
    D3DXVECTOR3 pos;        // 位置
    float fRadius;          // 現在の半径
    float fMaxRadius;       // 爆発半径の最大値
    int nAttack;            // 攻撃力
    int nLife;          // 寿命
    bool bUse;              // 使用中であるかどうかのフラグ

}Bomb;




//プロトタイプ宣言

void InitBomb(void);              // ボムの初期化処理
void UninitBomb(void);            // ボムの終了処理
void UpdateBomb(void);            // ボムの更新処理
void DrawBomb(void);              // ボムの描画処理

Bomb* GetBomb(void);              // ボムの情報を取得する処理
void SetBomb(D3DXVECTOR3 pos);               // ボムのセット処理


#endif 