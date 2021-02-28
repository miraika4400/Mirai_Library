//=============================================================================
//
// displayon [displayon.cpp]
// Author : 増澤 未来
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "display_on.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "sound.h"
#include "bullet.h"
#include "grid.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ANIM_SPEED 1              // アニメーション速度
#define MAX_ANIMATION_X 18        // アニメーション数 横
#define MAX_ANIMATION_Y 1         // アニメーション数 縦

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CDisplayON::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CDisplayON::CDisplayON() :CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0; // アニメーションカウント
	m_nAnimX = 0;   // アニメーションX軸
	m_nAnimY = 0;   // アニメーションY軸
}

//=============================================================================
// デストラクタ
//=============================================================================
CDisplayON::~CDisplayON()
{

}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CDisplayON::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "./data/Textures/Black Out.png",
		&m_pTexture);		// テクスチャへのポインタ

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CDisplayON::Unload(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// インスタンス生成
//=============================================================================
CDisplayON * CDisplayON::Create(void)
{
	// メモリ確保
	CDisplayON *pEnd = new CDisplayON;

	if (pEnd != NULL)
	{
		// 初期化処理
		pEnd->Init();
	}
	return pEnd;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDisplayON::Init(void)
{
	// 値の代入
	BindTexture(m_pTexture);

	// ポリゴン初期化
	CScene2d::Init();
	// テクスチャUV座標の初期化
	m_nAnimX = MAX_ANIMATION_X-1;
	m_nAnimY = 0;

	// UV座標の設定
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV座標セット
	SetTextureUV(uv);

	SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	SetSize(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void CDisplayON::Uninit(void)
{
	// 終了処理
	CScene2d::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CDisplayON::Update(void)
{
	// アニメーションカウントを進める
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_SPEED == 0)
	{
		// アニメーションX軸の加算
		m_nAnimX--;

		if (m_nAnimX <= 0)
		{
			// アニメーションX軸の初期化
			m_nAnimX = 0;
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}

		// UV座標の設定
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV座標セット
		SetTextureUV(uv);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDisplayON::Draw(void)
{
	// 描画処理
	CScene2d::Draw();
}