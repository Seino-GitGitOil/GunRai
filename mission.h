//-------------------------------------------------------------
//
// ミッションチェック処理の宣言 [mission.h]
// Author：清野蒼翔
//
//-------------------------------------------------------------
#ifndef _MISSION_H_
#define _MISSION_H_

#include "main.h"


// ミッションチェックの種類
typedef enum
{
	MISSION_CHECK_FAIL = 0,       
	MISSION_CHECK_CLEAR,  
	MISSION_CHECK_MAX

}MISSION_CHECK_TEX;

// ミッション一覧
typedef enum
{
	MISSION_NO_DEATH = 0,
	MISSION_NO_OVERHEAT,
	MISSION_200_COMBO,
	MISSION_3_GRAZE,
	MISSION_MAX

}MISSION_LIST;

typedef struct
{
	bool bMissionFail;    // もうミッション達成不可である場合trueに
	bool bMissionClear;   // ミッションをクリアした場合trueに

}Mission;

void InitCheckMission(void);
void UninitCheckMission(void);
void UpdateCheckMission(void);
void DrawCheckMission(void);


#endif