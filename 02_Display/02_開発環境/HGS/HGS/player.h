#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// プレイヤークラスヘッダー [player.h]
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
#define DECISION_PLAYER_SIZE_X	(30.0f)		// 当たり判定
#define DECISION_PLAYER_SIZE_Y	(30.0f)		// 当たり判定
#define PLAYER_SIZE_X			(40.0f)		// プレイヤーのサイズ
#define PLAYER_SIZE_Y			(40.0f)		// プレイヤーのサイズ

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2d;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer : public CScene2d
{
public:
	typedef enum
	{
		// プレイヤーの状態
		PLAYER_STATE_NONE = 0,		// 初期値
		PLAYER_STATE_NORMAL,		// 通常状態
		PLAYER_STATE_REVIVE,		// 復活時
		PLAYER_STATE_LASER,			// レーザー発射時
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer();			// コンストラクタ
	~CPlayer();			// デストラクタ

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理

	static HRESULT Load(void);												// テクスチャのロード
	static void UnLoad(void);												// テクスチャのアンロード
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ポリゴン生成

	void HitDamage(int nCount);					// ダメージ処理
	void PlayerControl(void);					// プレイヤー制御
	void UpdateState(void);						// プレイヤー状態更新
	void Shot(void);							// 弾の発射関数

	static bool GetAlive(void) { return m_bAlive; };                          // 生存判定
private:
	PLAYER_STATE m_State;					// プレイヤーの状態
	int m_StateCount;						// プレイヤーの状態カウンター
	int m_nBulletCnt;						// バレット間隔のカウンター
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャのポインタ
	int m_nLife;							// 自機の体力
	int m_nStock;							// ストック
	D3DXVECTOR3 m_Inertia;					// 慣性
	D3DXVECTOR3 m_move;						// 移動
	static bool m_bAlive;                          // 生存判定

};

#endif