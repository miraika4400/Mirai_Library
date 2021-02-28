////////////////////////////////////////////////////
//
//    タイトルクラスの処理[title.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "start_logo.h"

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

//**********************************
// マクロ定義
//**********************************
#define  TITLE_TEXTURE_PATH "data/Textures/title_screen.png" // テクスチャ

//=============================
// コンストラクタ
//=============================
CTitle::CTitle()
{
	m_pPolygon = NULL;
}

//=============================
// デストラクタ
//=============================
CTitle::~CTitle()
{
}

//=============================
// クリエイト
//=============================
CTitle * CTitle::Create(void)
{
	// メモリの確保
	CTitle *pTitle = new CTitle;
	// 初期化
	pTitle->Init();

	return pTitle;
}

//=============================
// 初期化処理
//=============================
HRESULT CTitle::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->BindTexture(m_pTexture);

	// タイトルロゴ
	CStartLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, START_LOGO_POS_Y, 0.0f),
		D3DXVECTOR3(START_LOGO_SIZE_X, START_LOGO_SIZE_Y, 0.0f));



	
	return S_OK;
}

//=============================
// 終了処理
//=============================
void CTitle::Uninit(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
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
void CTitle::Update(void)
{
	// ポリゴンの更新処理
	m_pPolygon->Update();

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || 
		CManager::GetMouse()->GetMouseTrigger(0) || 
		CManager::GetJoypad()->GetJoystickTrigger(3, 0)|| CManager::GetJoypad()->GetJoystickTrigger(11, 0))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	}
}


//=============================
// 描画処理
//=============================
void CTitle::Draw(void)
{
	// ポリゴンの描画処理
	m_pPolygon->Draw();
}
