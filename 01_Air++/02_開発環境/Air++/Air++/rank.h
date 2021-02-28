//=============================================================================
//
// rankヘッダ [rank.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _RANK_H_
#define _RANK_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//クラス定義
//*****************************

// ランククラス
class CRank : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CRank();
	~CRank();

	static CRank *Create(); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	void RankManager(void);   // 順位管理
	float CheckProgress(int nCheckNum, D3DXVECTOR3 pos); // 進行度の算出

	//============
	// メンバ変数
	//============

};

#endif