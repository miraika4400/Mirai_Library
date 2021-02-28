////////////////////////////////////////////////////
//
//    tutorialの処理[tutorial.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//=============================
// インクルード
//=============================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "player.h"
#include "grid.h"
#include "enemy.h"

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[TUTORIAL_NUM] = {};
CPlayer *CTutorial::m_pPlayer = NULL;
CEnemy *CTutorial::m_pEnemy = NULL;

//**********************************
// マクロ定義
//**********************************
#define  TITLE_TEXTURE_1_PATH "./data/Textures/Tutorial.png" // テクスチャ
#define  TITLE_TEXTURE_2_PATH "./data/Textures/tutorial001.png" // テクスチャ
#define  TITLE_TEXTURE_3_PATH "./data/Textures/tutorial002.png" // テクスチャ

//=============================
// コンストラクタ
//=============================
CTutorial::CTutorial()
{
	m_pPolygon = NULL;
	m_nNumTutorial = 0;
}

//=============================
// デストラクタ
//=============================
CTutorial::~CTutorial()
{
}

//=============================
// クリエイト
//=============================
CTutorial * CTutorial::Create(void)
{
	// メモリの確保
	CTutorial *pTutorial = new CTutorial;
	// 初期化
	pTutorial->Init();
	return pTutorial;
}

//=============================
// 初期化処理
//=============================
HRESULT CTutorial::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// プレイヤーが使われていないとき
	//if (m_pPlayer == NULL)
	//{
	//	// インスタンス生成
	//	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f),
	//		D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f));
	//}

	//// グリッド生成
	//CGrid::CreateAll();

	//if (m_pEnemy == NULL)
	//{
	//	// エネミー生成
	//	m_pEnemy = CEnemy::Create(D3DXVECTOR3(940.0f, 180.0f, 0.0f), D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f));
	//}

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_1_PATH, &m_pTexture[0]);
	//D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_2_PATH, &m_pTexture[1]);
	//D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_3_PATH, &m_pTexture[2]);

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pPolygon->BindTexture(m_pTexture[0]);
	}
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CTutorial::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < TUTORIAL_NUM; nCntTex++)
	{
		// テクスチャの解放
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	//// プレイヤーが使われていたら
	//if (m_pPlayer != NULL)
	//{
	//	m_pPlayer = NULL;
	//}

	//if (m_pEnemy != NULL)
	//{
	//	m_pEnemy = NULL;
	//}

	if (m_pPolygon != NULL)
	{
		// ポリゴンの終了処理
		m_pPolygon->Uninit();

		// メモリの解放
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	// 開放処理
	Release();
}

//=============================
// 更新処理
//=============================
void CTutorial::Update(void)
{
	if (m_pPolygon != NULL)
	{

		// ポリゴンの更新処理
		m_pPolygon->Update();
	}
	//if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
	//	CManager::GetMouse()->GetMouseTrigger(0) ||
	//	CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	//{
	//	m_nNumTutorial++;
	//	if (m_nNumTutorial >= TUTORIAL_NUM)
	//	{
	//		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	//	}
	//	else
	//	{
	//		m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	//	}
	//}
	//if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE) ||
	//	CManager::GetJoypad()->GetJoystickTrigger(2, 0))
	//{
	//	m_nNumTutorial--;
	//	if (m_nNumTutorial < 0)
	//	{
	//		m_nNumTutorial = 0;
	//	}

	//	m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	//}
	if (CManager::GetJoypad()->GetJoystickTrigger(11, 0)|| CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	}
}

//=============================
// 描画処理
//=============================
void CTutorial::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		// ポリゴンの描画処理
		m_pPolygon->Draw();
	}
}

//=============================
// チュートリアル設定
//=============================
void CTutorial::SetGame(void)
{
}

//=============================
// プレイヤー情報
//=============================
CPlayer * CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}
