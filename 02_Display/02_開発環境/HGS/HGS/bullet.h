#ifndef _BULLET_H_
#define _BULLET_H_
//=============================================================================
//
// バレットクラスヘッダー [bullet.h]
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
#define MAX_BULLET				(512)		// 弾の最大数
#define BULLET_SIZE_X			(20)		// 弾の横幅
#define BULLET_SIZE_Y			(20)		// 弾の縦幅
#define MAX_BULLET_TEXTURE		(1)			// テクスチャの最大数
#define BULLET_SPEED			(15.0f)		// スピード

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2d;

//=============================================================================
// バレットクラス
//=============================================================================
class CBullet : public CScene2d
{
public:
	CBullet();			// コンストラクタ
	~CBullet();			// デストラクタ

	HRESULT Init(void);						// 初期化処理
	void Uninit(void);																// 終了処理
	void Update(void);																// 更新処理
	void Draw(void);																// 描画処理
	static HRESULT Load(void);																// テクスチャロード
	static void UnLoad(void);																// テクスチャアンロード
	bool Collision(void);										// 当たり判定

	static CBullet * Create(																// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		int nLife);
private:
	D3DXVECTOR3 m_move;												// 移動量
	int m_nLife;													// 制限距離
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BULLET_TEXTURE];		// テクスチャ情報のポインタ
};

#endif