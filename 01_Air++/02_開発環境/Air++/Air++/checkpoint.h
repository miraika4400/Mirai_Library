//=============================================================================
//
// checkpointヘッダ [checkpoint.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// マクロ定義
//*****************************
#define MAX_CHECKPOINT_NUM 256

//*****************************
// 前方宣言
//*****************************
class CCollision;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CCheckPoint : public CScene
{
public:
	//============
	// 状態列挙
	//============

	//============
	// メンバ関数
	//============
	CCheckPoint();

	~CCheckPoint();
	static CCheckPoint *Create(void); // クラス生成
	
	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// コリジョンの取得
	CCollision*GetCollision(int nInex) { return m_pCollision[nInex]; }
	// チェックポイント数の取得
	int GetCheckPointNum(void) { return m_nNumPoint; }

private:
	void SaveCheckpoint(void); // テキストファイルにチェックポイントを書き込む
	void LoadCheckpoint(void); // テキストファイルにチェックポイントを書き込む

	// メンバ変数
	int m_nNumPoint;                             // チェックポイントの数
	CCollision*m_pCollision[MAX_CHECKPOINT_NUM]; // コリジョンクラスのポインタ
	D3DXVECTOR3 m_pointPos[MAX_CHECKPOINT_NUM];  // チェックポイントの座標
};

#endif