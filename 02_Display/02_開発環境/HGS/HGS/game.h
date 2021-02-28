//=============================================================================
//
// gameヘッダ [game.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//=============================
// 前方宣言
//=============================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;
class CSound;
class CCamera;
class CPlayer;
class CScore;

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CGame : public CScene
{
public:
	//*****************************
	// 列挙定義
	//*****************************


	//*****************************
	// メンバ関数
	//*****************************
	CGame();
	~CGame();

	static CGame *Create(void); // クラス生成

	HRESULT Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画
	void SetGame(void);			// ゲームの設定

	static CCamera *GetCamera(void) { return m_pCamera; }	// カメラの取得処理
	static CPlayer *GetPlayer(void) { return m_pPlayer; }	// プレイヤーの情報
	static CScore *GetScore(void) { return m_pScore; }
	bool GetSpeedUp(void) { return m_bSpeedUp; }
private:
	//*****************************
	// メンバ変数
	//*****************************
	static CCamera *m_pCamera;	// カメラ
	static CPlayer *m_pPlayer;	// プレイヤー
	static CScore *m_pScore;	// スコア
	int m_nEnemyFlame;			// 敵の出るフレーム
	int m_nGameFlame;			// ゲームのカウンター
	bool m_bSpeedUp;			// スピードアップ
};

#endif