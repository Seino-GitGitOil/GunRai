//-------------------------------------------
//
// 敵の処理に関する宣言[enemy.h]
// Author：清野蒼翔
//
//-------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_  //二重インクルード防止のマクロ

#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)     // 敵の最大数
#define ENEMY_SIZE (30.0f)  // 敵の半径(デフォルト)
#define DEATH_BORDER_LINE (30)    // 敵が消える範囲の調整

//敵の種類　の列挙型定義
typedef enum
{
	ENEMY_TYPE_A = 0,     //まっすぐ進む
	ENEMY_TYPE_B,         //まっすぐの弾を撃つ鈍足
	ENEMY_TYPE_C,         //自機狙いの弾を撃つ
	ENEMY_TYPE_D,         //大きな弾を撃つ
	ENEMY_TYPE_E,         //自機に向かって突進
	ENEMY_TYPE_F,         //ぐるぐる弾を撃つ
	ENEMY_TYPE_G,         
	ENEMY_TYPE_H,
	ENEMY_TYPE_I,
	ENEMY_TYPE_J,
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

// 敵の行動パターン(流用する系ここ入れとこ)
typedef enum 
{
	ACTION_STRAIGHT_DOWN, // 下へ直進
	ACTION_TARGET_PLAYER, // プレイヤーに向かって直線移動
	ACTION_HOVER_SHOOT,   // 画面内で一時停止して射撃後、撤退
	ACTION_WAVE           // 左右にサイン波を描いて移動

} ENEMY_ACTION;


//敵の状態　の列挙型定義
typedef enum
{
	ENEMYSTATE_NORMAL = 0,     //通常状態
	ENEMYSTATE_DAMAGE,         //被ダメージ時の状態
	ENEMYSTATE_MAX
}ENEMYSTATE;


//敵構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;       // 位置
	D3DXVECTOR3 move;      // 移動量

	D3DXVECTOR3 rot;      // 角度
	float fLengthEnemy;         //対角線の長さ
	float fAngleEnemy;          //対角線の角度

	int nLife;             // 体力
	ENEMYSTATE state;      // 状態
	int nCounterState;     // 状態カウンター
	int nType;             // 種類
	bool bUse;             // 使用しているかどうか
	int nEnemyBulletCounter;  // 敵の弾の発射間隔
	int nEnemyActionCounter;  // 敵の行動の間隔
	bool bSufferBomb;      // ボムを受けたかどうか

}Enemy;

//プロトタイプ宣言
void InitEnemy(void);                      //敵の初期化処理
void UninitEnemy(void);                    //敵の終了処理
void UpdateEnemy(void);                    //敵の更新処理
void DrawEnemy(void);                      //敵の描画処理

void SetEnemy(D3DXVECTOR3 pos, int nType);  //敵のセット処理
Enemy* GetEnemy(void);                      //敵のポインタを渡す処理

void HitEnemy(int nCntEnemy, int nDamage);  //敵がダメージを受ける処理
void CollisionBomb_Enemy(Enemy* pEnemy);    // ボムとのあたり判定

#endif