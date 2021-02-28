//=============================================================================
//
// destinationヘッダ [destination.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _DESTNATION_H_
#define _DESTNATION_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"
//*****************************
// マクロ定義
//*****************************
#define DISTANCE_PLAYER_DDRIFT  -300                   // プレイヤーとの距離のドリフト時
#define DISTANCE_PLAYER_DEFAULT -200                   // プレイヤーとの距離の基本値
#define DISTANCE_PLAYER_INIT    -10                    // プレイヤーとの距離の初期

//*****************************
// クラス定義
//*****************************
class CModel;

//*****************************
// クラス定義
//*****************************

// 当たり判定クラス
class CDestination
{
public:
	//メンバ関数
	CDestination();
	~CDestination();
	static CDestination *Create(const int nPlayerNum, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = VEC3_ZERO);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 座標の取得・セット
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	// ロットの取得・セット
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; m_rotDest = rot; }
	// ロットの目標値のセット
	void SetRotDest(D3DXVECTOR3 rot) { m_rotDest = rot; }

	// 移動量の取得・セット
	D3DXVECTOR3  GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	// 重力フラグの取得・セット
	bool GetActiveGravity(void) { return m_bGravity; }
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	// プレイヤー番号の取得
	int GetPlayerNum(void) { return m_nPlayerNum; }

	// プレイヤーとの距離の取得・セット
	float GetDistancePlayer(void) { return m_fDistancePlayer; }
	void SetDistancePlayer(float fDistance);
	// プレイヤーとの距離の目標値の取得・セット
	float GetDistanceDest(void) { return m_fDistanceDest; }
	void SetDistanceDest(float fDistance) { m_fDistanceDest = fDistance; }

private:
	void MoveControll(void);
	void Direction(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;         // 座標
	D3DXVECTOR3 m_rot;         // ロット
	D3DXVECTOR3 m_rotDest;     // ロット目標値
	D3DXVECTOR3 m_move;        // 移動量

	int m_nPlayerNum;          // どのプレイヤーの目標か
	D3DXVECTOR3 m_gravityVec;  // 重力
	bool m_bGravity;           // 重力フラグ
	float m_fDistancePlayer;   // プレイヤーとの距離
	float m_fDistanceDest;     // プレイヤーとの目標値
	bool m_bGoal;              // ゴールフラグ
#ifdef _DEBUG
							   // デバッグ用

	void CreateMesh(void);

	LPD3DXMESH m_pMeshModel;	//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMatModel;	//マテリアル情報へのポインタ
	DWORD m_nNumMatModel;	//マテリアル情報の数
	CModel*pModel;
#endif
};

#endif