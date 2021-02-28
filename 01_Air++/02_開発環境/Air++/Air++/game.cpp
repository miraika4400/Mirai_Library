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
#include "light.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "player.h"
#include "course.h"
#include "checkpoint.h"
#include "character.h"
#include "cpu_point.h"
#include "cpu.h"
#include "rank.h"
#include "item_point.h"
#include "item.h"
#include "start.h"
#include "joypad.h"

//=============================
// マクロ定義
//=============================
#define RESULT_COUNT 250 // リザルトへの遷移カウント
#define INIT_POS_X 400   // キャラクター配置初期X位置
#define INIT_POS_Z 0     // キャラクター配置初期Y位置
#define SPACE_X 280      // キャラクター配置Xの間隔
#define SPACE_Z 140      // キャラクター配置Yの間隔
#define X_RESET_NUM 4    // キャラクター配置Xをリセットする間隔

//=============================
// 静的メンバ変数宣言
//=============================
CCamera *CGame::m_apCamera[MAX_PLAYER_NUM] = {};           // カメラクラスポインタ
CLight  *CGame::m_pLight = NULL;                           // ライトクラスポインタ
CCharacter * CGame::m_apCharacter[MAX_CHARACTER_NUM] = {}; // キャラクタークラスポインタ
int CGame::m_nNumCaracter = 0;                             // 生成したキャラクターの数
int CGame::m_nNumPlayer = 1;                               // プレイヤー数
CCpuPoint *CGame::m_pCpuPoint = NULL;                      // CPU用コースデータ
CCheckPoint *CGame::m_pCheckPoint = NULL;                  // チェックポイント
CItemPoint *CGame::m_pItemPoint = NULL;                    // アイテムポイント
CCourse    *CGame::m_pCourse = NULL;                       // コースポインタ
CGame::GAME_STATE CGame::m_gameState = CGame::GAME_NORMAL; // ゲームの状態

//=============================
// コンストラクタ
//=============================
CGame::CGame()
{
	// 変数のクリア
	m_nNumCaracter = 0;
	m_nCntResult= 0;
	m_bResult = false;
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
	// カーソルを消す
	ShowCursor(FALSE);

	// ポーズの初期化
	CManager::SetActivePause(false);

	// ライトクラスの生成
	m_pLight = new CLight;
	// ライトクラスの初期化
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}
	// リザルトカウントの初期化
	m_nCntResult = 0;
	// リザルトフラグの初期化
	m_bResult = false;
	// ゲームの状態の初期化
	m_gameState = GAME_NORMAL;

	// 背景の生成
	CBg::Create();

	// 順位管理クラスの生成
	CRank::Create();

	// コース生成
	m_pCourse = CCourse::Create();

	// チェックポイントの生成
	m_pCheckPoint = CCheckPoint::Create();

	// CPU用のコースデータ
	m_pCpuPoint = CCpuPoint::Create();
	
	// アイテム座標
	m_pItemPoint = CItemPoint::Create();


	// キャラクターの生成・配置
	float fPosX = INIT_POS_X;
	float fPosZ = INIT_POS_Z;
	for (int nCnt = 0; nCnt < MAX_NPC_NUM; nCnt++)
	{
		if (m_nNumCaracter % X_RESET_NUM == 0)
		{
			fPosX = INIT_POS_X;
		}

		// CPU生成
		m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), (CCpu::CPUTYPE)(rand() % CCpu::CPU_TYPE_MAX), rand() % 10 + 1);
		// キャラクター数加算
		m_nNumCaracter++;
		fPosX -= SPACE_X;
		fPosZ += SPACE_Z;

	}
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		// カメラクラスの生成
		m_apCamera[nCnt] = CCamera::Create(nCnt);
		// プレイヤー生成
		m_apCharacter[m_nNumCaracter] = CPlayer::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), 0);
		// キャラクター数加算
		m_nNumCaracter++;
		fPosX -= SPACE_X;
		fPosZ += SPACE_Z;
	}
	

	// スタートのカウントダウンの生成
	CStart::Create();
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CGame::Uninit(void)
{
	// カメラクラスの解放処理
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Uninit();
		}
	}

	// ライト
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// 開放処理
	Release();
}


//=============================
// 更新処理
//=============================
void CGame::Update(void)
{
	// マウスカーソルの位置固定
	CManager::GetMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
#ifdef _DEBUG
	// デバッグ用画面遷移コマンド
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

#endif // _DEBUG

	if (m_gameState == GAME_NORMAL)
	{
		// アイテムを回転させる
		CItem::RotationItem();

		// ゴール状態のチェック
		CheckGoal();
	}
	else
	{

		if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
			CManager::GetMouse()->GetMouseTrigger(0) ||
			CManager::GetJoypad()->GetJoystickTrigger(3, 0) ||
			CManager::GetJoypad()->GetJoystickTrigger(11, 0))
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}

	// カメラクラスの更新処理
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Update();
		}
	}
}

//=============================
// 描画処理
//=============================
void CGame::Draw(void)
{
	// カメラのセット
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}

//=============================
// プレイヤーがゴールしているかチェック
//=============================
void CGame::CheckGoal(void)
{
	if (!m_bResult)
	{
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			// 一人でもゴールしてなかったら処理終了
			if (!CPlayer::GetPlayer(nCnt)->GetGoalFlag()) return;
		}

		// ここまで処理が来たらすべてのプレイヤーがゴールしている
		m_bResult = true;
	}
	else
	{
		// カウントを進める
		m_nCntResult++;

		if (m_nCntResult > RESULT_COUNT)
		{// 一定のカウント超えたら
			// リザルトに移行
			m_gameState = GAME_RESULT;
		}
	}
}
