//=============================================================================
//
// playerヘッダ [player.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "character.h"

//*****************************
// マクロ定義
//*****************************

//*****************************
// 前方宣言
//*****************************
class CDestination;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CPlayer : public CCharacter
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	//メンバ関数
	//==========================
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum); // クラス生成
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード
	static CPlayer* GetPlayer(int nPlayerNum); // プレイヤー取得処理*引数：取得したいプレイヤー番号

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// ドリフトフラグの取得・セット
	bool GetDriftLeft(void) { return m_bDriftLeft; }
	void SetDriftLeft(bool bDrift) { m_bDriftLeft = bDrift; }
	bool GetDriftRight(void) { return m_bDriftRight; }
	void SetDriftRight(bool bDrift) { m_bDriftRight = bDrift; }

	// プレイヤー番号の取得処理
	int GetPlayerNum(void) { return m_nPlayerNum; }
	// プレイヤーの向き取得
	float GetDir(void) { return m_fDir; }
	// 移動目標の取得処理
	CDestination * GetDest(void) { return m_pDest; }

	// ゴール時の処理
	void GoalAction(void);
private:
	void Move(void);      // 移動処理
	void UseItem(void);   // アイテム使用の処理
	void Direction(void); // 向きの管理処理
	void Drift(void);     // ドリフト処理
	void FallAction(void); // 落下時のアクション

	//==========================
	//メンバ変数
	//==========================
	static Model m_model;   // モデル構造体
	static Shader m_shader; // シェーダー構造体
	CDestination * m_pDest; // 移動目標クラス
	int m_nPlayerNum;       // プレイヤーID
	float m_fDir;           // プレイヤーの正面
	bool m_bDriftLeft;      // ドリフト左
	bool m_bDriftRight;     // ドリフト右
};
#endif