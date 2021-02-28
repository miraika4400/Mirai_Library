//=============================================================================
//
// trapヘッダ [trap.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _TRAP_H_
#define _TRAP_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// マクロ定義
//*****************************

//*****************************
// 前方宣言
//*****************************
class CCollision;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CTrap : public CModel
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	//メンバ関数
	//==========================
	CTrap();
	~CTrap();
	static CTrap *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool CollisionCharacter(void); // プレイヤーとの当たり判定
	bool CollisionAttack(void);      // 攻撃オブジェクトとの当たり判定

	//==========================
	// メンバ変数
	//==========================
	static Model m_model;      // モデル構造体
	CCollision*m_pCollision;   // 当たり判定
};
#endif