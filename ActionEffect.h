//-------------------------------------------------------------
//
// 派手さを出すためのエフェクト処理の宣言 [ActionEffect.h]
// Author：清野蒼翔
//
//-------------------------------------------------------------
#ifndef _ACTION_EFFECT_H_
#define _ACTION_EFFECT_H_

#include "main.h"

// エフェクトに使用するテクスチャの列挙型定義
typedef enum
{
	ACTION_EFFECT_TEX_A = 0,
	ACTION_EFFECT_TEX_B,
	ACTION_EFFECT_TEX_C,
	ACTION_EFFECT_TEX_D,
	ACTION_EFFECT_TEX_E,
	ACTION_EFFECT_TEX_MAX,

}ACTION_EFFECT_TEXTURE_TYPE;


//エフェクトパターン構造体
typedef struct
{
	int nType;   //エフェクトの種類
	int nCounterAnim;  // アニメーションカウンター
	int nNowPatternAnim;  //エフェクトのテクスチャの現在のアニメーション数
	int nMaxPatternAnim;  //エフェクトのアニメーションの最大数 (マクロ定義で各TYPEに分けること)
	int nMoveTexTime;     //テクスチャアニメーションを進める間隔を設定
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 rot;  //向き

	float Tex_Hurfsize_x;  //テクスチャの半径
	float Tex_Hurfsize_y;  //テクスチャの半径
	bool bUse;   //使用されているか

}ACTION_EFFECT;

// プロトタイプ宣言
void InitActionEffect(void);                // 初期化処理
void UninitActionEffect(void);              // 終了処理
void UpdateActionEffect(void);              // 更新処理
void DrawActionEffect(void);                // 描画処理
void SetActionEffect(D3DXVECTOR3 pos, int EffectType, float fsize);      // セット処理

#endif