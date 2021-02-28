//=============================================================================
//
// attackヘッダ [attack.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ATTACK_H_
#define _ATTACK_H_

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

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CAttack : public CCharacter
{
public:
	//==========================
	// 列挙
	//==========================


	//==========================
	// メンバ関数
	//==========================
	CAttack();
	~CAttack();

	static CAttack *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nRank, int nUseID); // クラス生成*引数:座標,初期化回転角度,順位
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	void Move(void);        // 移動処理
	void DistManager(void); // 移動目標の管理
	void CollisionCharacter(void);// プレイヤー・NPCとの当たり判定*キャラクタークラスの更新で呼ばれてる
	void FallAction(void); // 落下時のアクション

	//==========================
	// メンバ変数
	//==========================
	static Model m_model;            // モデル構造体
	static Shader m_shader;          // シェーダー構造体
	CCollision * m_pDIstCollision;   // 移動目標値当たり判定
	CCollision * m_pTargetCollision; // 攻撃目標の当たり判定
	int m_nPointNum;                 // コースポイント配列管理用
	int m_nRank;                     // 放った人の順位
	int m_nLife;                     // 寿命
	int m_nUseID;                    // 使った人のオブジェクト番号
	int m_nRoot;                     // ルート
};
#endif