//-----------------------------------------
//
// プレイヤー処理の宣言 [player.h]
// Author：清野蒼翔
//
//-----------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_  //二重インクルード防止のマクロ

#define MAX_PLAYER_TEX_PATTERN (8)   // 
#define TEXTURE_WIDTH (20.0f)        // テクスチャの横の長さ
#define TEXTURE_HEIGHT (20.0f)       // テクスチャの縦の長さ
#define TEXTURE_WIDTH_AND_HEIGHT (20.0f)  // テクスチャの長さ(上二つを統一)


#define GRAZE_RADIUS (20.0f)        //グレイズ判定の半径

#define PLAYER_OFFSET (2.0f)        //1フレームあたりのプレイヤーの移動量 
#define PLAYER_OFFSET_SHIFT (0.7f)        //シフトが押されている際の移動量補正倍率 

#define MAX_BURST_MATER (100.0f)    //バーストメータの最大値

#define RESPAWN_TIME (3)  //リスポーン後の無敵時間(秒)

//「プレイヤーの状態」の列挙型定義
typedef enum
{
	PLAYERSTATE_NORMAL = 0,     //通常状態
	PLAYERSTATE_DEATH,          //死亡状態(描画をしなくなる)
	PLAYERSTATE_SPAWN_IMMUNE,   //リスポーン直後の無敵状態
	PLAYERSTATE_OVERHEAT,       //オーバーヒート
	PLAYERSTATE_GAMEOVER,       //ゲームオーバー

	PLAYERSTATE_MAX             //いらんかもここだけ(for文で使う気しない..)

}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;          //位置
	D3DXVECTOR3 rot;          //向き
	D3DXVECTOR3 move;         //移動量

	float fLengthPlayer;         //対角線の長さ
	float fAnglePlayer;          //対角線の角度
	PLAYERSTATE PlayerState;     //プレイヤーの状態

	int nRespawnTimer;      //リスポーンまでのタイマー
	int nSpawnImmuneTimer;  // 復活後の無敵時間カウントダウン
	bool bBurstMode;       //バーストモードであるかどうか
	bool bBurstShield;     //バーストシ－ルドがあるかどうか
	float nBulletCounter;    //プレイヤーの弾の発射間隔
	int nLifeStock;        //残機　　　　　    　難易度選択画面を作るからそこで選ばれたもの次第かなぁ。
	int nBombStock;        //ボム数              Bキーかな。押したら範囲内の敵の弾を全て削除し、敵にもダメージを与える処理で。
	float nBulletStock;    //持っている弾数　　　弾の残量が最大値から計算して何割かを取得し、その割合分テクスチャを長く読み込ませるとか
	float nBurstMater;       //バーストゲージ値
	int nCombo;            //コンボ数
	bool bBurstGaugeMax;    // バーストゲージが最大かどうかを判定

	//以下ミッションに使用予定
	int nKill;             // 総キル数
	int nOverheat;         // オーバーヒート回数
	int nGraze;            // グレイズ回数



}Player;


//プロトタイプ宣言

void InitPlayer(void);              // プレイヤーの初期化処理
void UninitPlayer(void);            // プレイヤーの終了処理
void UpdatePlayer(void);            // プレイヤーの更新処理
void DrawPlayer(void);              // プレイヤーの描画処理
Player *GetPlayer(void);
void HitPlayer(void);               // プレイヤーのヒット処理
void CheckCollisionEnemy(void);     // 敵との接触判定
void CheckCollisionBoss(void);     // ボスとの接触判定

#endif