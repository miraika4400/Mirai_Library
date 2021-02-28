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

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[TUTORIAL_NUM] = {};

//**********************************
// マクロ定義
//**********************************
#define  TITLE_TEXTURE_1_PATH "./data/Textures/tutorial_1.png" // テクスチャ
#define  TITLE_TEXTURE_2_PATH "./data/Textures/tutorial_2.png" // テクスチャ
#define  TITLE_TEXTURE_3_PATH "./data/Textures/tutorial_3.png" // テクスチャ

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
	CTutorial *pTitle = new CTutorial;
	// 初期化
	pTitle->Init();
	return pTitle;
}

//=============================
// 初期化処理
//=============================
HRESULT CTutorial::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_1_PATH, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_2_PATH, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_3_PATH, &m_pTexture[2]);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->BindTexture(m_pTexture[0]);
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
	// ポリゴンの更新処理
	m_pPolygon->Update();

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
		CManager::GetMouse()->GetMouseTrigger(0) ||
		CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		m_nNumTutorial++;
		if (m_nNumTutorial >= TUTORIAL_NUM)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
		else
		{
			m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
		}
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE) ||
		CManager::GetJoypad()->GetJoystickTrigger(2, 0))
	{
		m_nNumTutorial--;
		if (m_nNumTutorial < 0)
		{
			m_nNumTutorial = 0;
		}

		m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	}
}


//=============================
// 描画処理
//=============================
void CTutorial::Draw(void)
{
	// ポリゴンの描画処理
	m_pPolygon->Draw();
}
