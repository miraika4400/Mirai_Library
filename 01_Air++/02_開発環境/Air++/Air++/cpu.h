//=============================================================================
//
// cpuヘッダ [cpu.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _CPU_H_
#define _CPU_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "character.h"

//*****************************
// マクロ定義
//*****************************
#define CPU_ITEM_USE_RAND_MIN 20        // アイテムを使うまでのカウント乱数の最低値
#define CPU_ITEM_USE_RAND_AMPLITUDE 200 // アイテムを使うまでのカウント乱数のふり幅

//*****************************
// 前方宣言
//*****************************

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CCpu : public CCharacter
{
public:
	//==========================
	// 列挙
	//==========================
	typedef enum
	{
		CPU_TYPE_STAR = 0,
		CPU_TYPE_RUIN,
		CPU_TYPE_MAX,
	}CPUTYPE;

	//==========================
	// メンバ関数
	//==========================
	CCpu();
	~CCpu();

	static CCpu *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CPUTYPE cpuType , int nLevel); // クラス生成
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	// アイテムカウントのセット
	void SetItemCount(int nCnt) { m_nItemCnt = nCnt; }

private:
	void Move(void);        // 移動処理
	void DistManager(bool bChange = false); // 目標地点の管理関数*引数：trueの時無理やり目標地点を変える
	void UseItem(void);     // アイテム仕様処理
	void FallAction(void); // 落下時のアクション
	//==========================
	// メンバ変数
	//==========================
	static Model m_aModel[CPU_TYPE_MAX]; // モデル構造体
	static Shader m_shader;              // シェーダー構造体
	CPUTYPE m_cpuType;                   // CPUの種類
	CCollision * m_pDIstCollision;       // 移動目標値当たり判定
	int m_nCpuPointNum;                  // コースポイント配列管理用
	int m_nLevel;                        // CPUの強さ
	int m_nItemCnt;                      // アイテムを使うまでのカウント
	int m_nRoot;                         // ルート
};
#endif