////////////////////////////////////////////////////
//
//    ゲームクラスの処理[game.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//=============================
// インクルード
//=============================
#include "game.h"
#include "bg.h"
#include "number.h"
#include "score.h" 
#include "mouse.h"
#include "camera.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "player.h"
#include "enemy.h"
#include "grid.h"
#include "break_effect.h"
#include "ranking.h"

//=============================
// マクロ定義
//=============================
#define ENEMY_FIRST_INTER		(110)
#define ENEMY_SECOND_INTER		(85)
#define ENEMY_THIRD_INTER		(70)
#define ENEMY_FOURS_INTER		(60)
#define ENEMY_FIVE_INTER		(45)
#define ENEMY_SIX_INTER			(35)
#define ENEMY_SEVEN_INTER		(20)
#define ENEMY_EIGHT_INTER		(15)

//=============================
// 静的メンバ変数宣言
//=============================
CCamera *CGame::m_pCamera = NULL;   // カメラ
CPlayer *CGame::m_pPlayer = NULL;	// プレイヤー
CScore *CGame::m_pScore = NULL;		// スコア

//=============================
// コンストラクタ
//=============================
CGame::CGame()
{
	m_nEnemyFlame = 0;			// 敵の出るフレーム
	m_nGameFlame = 0;			// ゲームのカウンター
	m_bSpeedUp = false;
}

//=============================
// デストラクタ
//=============================
CGame::~CGame()
{

}

//=============================
// クリエイト
//=============================
CGame * CGame::Create(void)
{
	// メモリの確保
	CGame *pGame = new CGame;
	// 初期化
	pGame->Init();

	return pGame;
}

//=============================
// 初期化処理
//=============================
HRESULT CGame::Init(void)
{
	// プレイヤーの生成
	if (m_pPlayer == NULL)
	{
		// インスタンス生成
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f), 
			D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f));
	}

	// スコアの生成
	if (m_pScore == NULL)
	{
		// インスタンス生成
		m_pScore = CScore::Create();
	}

	// 敵の出るフレーム
	m_nEnemyFlame = ENEMY_FIRST_INTER;

	// ポーズの初期化
//	CManager::SetActivePause(false);

	CGrid::CreateAll();
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// プレイヤーが使われていたら
	if (m_pPlayer != NULL)
	{
		m_pPlayer = NULL;	// プレイヤー
	}

	// スコアが使われていたら
	if (m_pScore != NULL)
	{
		CRanking::SetRanking(m_pScore->GetScore());

		m_pScore = NULL;
	}

	// 開放処理
	Release();
}

//=============================
// 更新処理
//=============================
void CGame::Update(void)
{
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		//CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

	// ゲームの設定
	SetGame();

	if (CManager::GetMouse()->GetMouseTrigger(0))
	{
		//CGrid::BreakAll(CManager::GetMouse()->GetMousePos());
	}
	CGrid::Shake();
}

//=============================
// 描画処理
//=============================
void CGame::Draw(void)
{
}

//=============================================================================
// ゲームの設定
//=============================================================================
void CGame::SetGame(void)
{
	m_nGameFlame++;			// ゲームのカウンター

	// エネミーの出現間隔
	if (m_nGameFlame % m_nEnemyFlame == 0)
	{
		int nNum = rand() % 4;
		float fPosX = 0.0f, fPosY = 0.0f;
		switch (nNum)
		{
		case 0:
			fPosX = -ENEMY_SIZE_X;
			fPosY = (float)(rand() % SCREEN_HEIGHT);
			break;
		case 1:
			fPosX = (float)(rand() % SCREEN_WIDTH);
			fPosY = -ENEMY_SIZE_Y ;
			break;
		case 2:
			fPosX = SCREEN_WIDTH + ENEMY_SIZE_X;
			fPosY = (float)(rand() % SCREEN_HEIGHT);
			break;
		case 3:
			fPosX = (float)(rand() % SCREEN_WIDTH);
			fPosY = SCREEN_HEIGHT + ENEMY_SIZE_Y;
			break;
		default:
			break;
		}

		D3DXVECTOR3 pos = D3DXVECTOR3(fPosX, fPosY, 0.0f);

		// エネミーの出現
		CEnemy::Create(pos, D3DXVECTOR3(ENEMY_SIZE_X , ENEMY_SIZE_Y, 0.0f));
	}

	// 二段階目
	if (m_nGameFlame == 600)	// 10秒
	{
		m_nEnemyFlame = ENEMY_SECOND_INTER;
	}
	if (m_nGameFlame == 1200)	// 20秒
	{
		m_nEnemyFlame = ENEMY_THIRD_INTER;
	}
	if (m_nGameFlame == 2100)	// 35秒
	{
		m_nEnemyFlame = ENEMY_FOURS_INTER;
	}
	if (m_nGameFlame == 3000)	// 50秒
	{
		m_nEnemyFlame = ENEMY_FIVE_INTER;
		m_bSpeedUp = true;
	}
	if (m_nGameFlame == 4200)	// 70秒
	{
		m_nEnemyFlame = ENEMY_SIX_INTER;
	}
	if (m_nGameFlame == 5400)	// 90秒
	{
		m_nEnemyFlame = ENEMY_SEVEN_INTER;
	}
	if (m_nGameFlame == 6600)	// 110秒
	{
		m_nEnemyFlame = ENEMY_EIGHT_INTER;
	}
}