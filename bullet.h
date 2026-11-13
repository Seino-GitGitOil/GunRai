//-------------------------------------------
//
// 弾の処理に関する宣言[bullet.h]
// Author：清野蒼翔
//
//-------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_  //二重インクルード防止のマクロ

//マクロ定義
#define MAX_BULLET_GAUGE (50.0f)    //弾のゲージの最大数


//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,       //プレイヤーの弾
	BULLETTYPE_ENEMY,            //敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

//弾の挙動の種類
typedef enum
{
	BULLETKIND_NORMAL = 0,       // 普通の直進の弾
	BULLETKIND_SONIC,            // 高速弾 
	BULLETKIND_BIG,              // デカい弾 
	BULLETKIND_MAX
}BULLETKIND;


//弾構造体の定義
typedef struct
{ 
	D3DXVECTOR3 pos;   // 弾の位置
	D3DXVECTOR3 move;  // 弾の移動量
	D3DXCOLOR col;     // 弾の色
	int nLife;         // 弾の寿命
	BULLETTYPE type;   // 種類
	BULLETKIND kind;   // 弾の挙動
	float fBulletSize;  // 弾のサイズ
	bool bGraze;       // グレイズ判定を過去に受けたかどうかを判定
	int nGrazeCounter; // グレイズ成功後、被弾していない状態が続いているかの確認
	bool bGrazeChecker;// グレイズ範囲内に弾が入った場合、それが１０フレーム経つまでの間trueにしておく
	bool bUse;         // 弾が使用されているかどうか

}Bullet;

//プロトタイプ宣言
void InitBullet(void);                        // 弾の初期化処理
void UninitBullet(void);                      // 弾の終了処理
void UpdateBullet(void);                      // 弾の更新処理
void DrawBullet(void);                        // 弾の描画処理
void CollisionPlayer(Bullet* pBullet);        // プレイヤーとの判定処理
void GrazePlayer(Bullet* pBullet);            // プレイヤーとのグレイズ判定処理
void CollisionEnemy(Bullet* pBullet);         // 敵との判定処理
void CollisionBoss(Bullet* pBullet);           // ボスとの判定処理
void CollisionBomb(Bullet* pBullet);          // ボムとの判定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, BULLETTYPE type, BULLETKIND kind);  // 弾のセット処理
void DeleteBullet(void);                      // 全ての弾を消す処理

#endif