#ifndef _ENEMY_H_
#define _ENEMY_H_
//=============================================================================
//
// エネミークラスヘッダー [enemy.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene2d.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY				(256)			// 敵の最大数
#define ENEMY_SIZE_X			(25)			// エネミーのサイズ
#define ENEMY_SIZE_Y			(25)			// エネミーのサイズ
#define MAX_ENEMY_TEXTURE		(1)				// エネミーのテクスチャ
#define ENEMY_SCORE_NORMAL		(1)				// 通常スコア

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2d;
class CLight;

//=============================================================================
// エネミークラス
//=============================================================================
class CEnemy : public CScene2d
{
public:
	CEnemy();			// コンストラクタ
	~CEnemy();			// デストラクタ

	HRESULT Init(void);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理
	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード

	void HitDamage(int nCount);										// エネミーのヒット時
	static CEnemy * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// エネミー生成
	bool Collision(void);											// 当たり判定
	void Rotation(void);											// ポリゴンの回転
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_ENEMY_TEXTURE];		// テクスチャ情報のポインタ
	int m_nLife;							// 体力
	int m_nSpeed;							// 速さ
	float m_fRasian;
	float m_fDiagonal;
	float m_fAngle;
};

#endif