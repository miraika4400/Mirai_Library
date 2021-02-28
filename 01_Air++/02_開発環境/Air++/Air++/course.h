//=============================================================================
//
// courseヘッダ [course.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _COURSE_H_
#define _COURSE_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "model_shader.h"

//*****************************
// マクロ定義
//*****************************
#define HOVER_HEIGHT 50   // キャラクターがどれくらい浮いているか

//*****************************
// 前方宣言
//*****************************
class CCharacter;

//*****************************
// クラス定義
//*****************************

// パーティクルクラス
class CCourse : public CModelShader
{
public:
	//==========================
	// 列挙
	//==========================

	//==========================
	//メンバ関数
	//==========================
	CCourse();
	~CCourse();

	static CCourse *Create(void); // クラス生成
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

	void CollisionCharacter(CCharacter* pChara);  // キャラクターの当たり判定*直接呼び出す

private:
	void CollisionCharacter(void);                // キャラクターの当たり判定*更新で呼び出す
	

	//==========================
	//メンバ変数
	//==========================
	static Model m_model;   // モデル構造体
	static Shader m_shader; // シェーダー構造体


};
#endif