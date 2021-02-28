//=============================================================================
//
// itemヘッダ [item.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ITEM_H_
#define _ITEM_H_

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
class CBillboard;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CItem : public CModel
{
public:
	//==========================
	// 列挙
	//==========================
	// アイテムタイプ
	typedef enum
	{

		ITEM_NONE = -1,    // 無
		ITEM_ACCELERATION, // 加速
		ITEM_ATTACK,       // 攻撃
		ITEM_TRAP,         // トラップ
		ITEM_MAX           // アイテムタイプ最大数
	}ITEM_TYPE;

	//==========================
	//メンバ関数
	//==========================
	CItem();
	~CItem();
	static CItem *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static void RotationItem(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void CollisionCharacter(void); // プレイヤーとの当たり判定

	//==========================
	// メンバ変数
	//==========================
	static Model m_model;                 // モデル構造体
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	static D3DXVECTOR3 m_rot;             // 全アイテムの向きを統一する

	CCollision*m_pCollision;   // 当たり判定
	CBillboard* m_pBillboard;  // ボックスの中の?マーク
	bool m_bActive;            // アイテムが有効か
	int  m_nCntRePop;          // アイテムリポップカウント
	bool m_bExpansion;         // サイズ拡大フラグ
};
#endif