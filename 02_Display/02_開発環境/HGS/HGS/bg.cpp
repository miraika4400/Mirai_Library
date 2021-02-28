//===================================================
//
//    背景クラスの処理[bg.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"
#include "camera.h"
#include "game.h"

//*****************************
// マクロ定義
//*****************************

// テクスチャのパス
#define BG_TEXTURE_PATH1 "./data/Textures/bg001.png"
#define BG_TEXTURE_PATH2 "./data/Textures/bg000.png"
#define BG_TEXTURE_PATH3 "./data/Textures/bg002.png"

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CBg::m_apTexture[BG_TEXTURE_NUM] = {};

//==================================
// コンストラクタ
//==================================
CBg::CBg()
{
	// Oクリア
	memset(m_afTexMoveU, 0, sizeof(m_afTexMoveU));
	memset(m_afTexMoveV, 0, sizeof(m_afTexMoveV));
	memset(m_apScene3d, 0, sizeof(m_apScene3d));
}

//==================================
// デストラクタ
//==================================
CBg::~CBg()
{
}

//==================================
// クリエイト
//==================================
CBg * CBg::Create(void)
{
	// メモリの確保
	CBg *pBg = new CBg;
	// 初期化処理
	pBg->Init();

	return pBg;
}

//==================================
// ロード
//==================================
HRESULT CBg::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH1, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH2, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_PATH3, &m_apTexture[2]);

	return S_OK;
}

//==================================
// アンロード
//==================================
void CBg::Unload(void)
{
	// テクスチャ数分、テクスチャの開放処理
	for (int nCntTex = 0; nCntTex < BG_TEXTURE_NUM; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CBg::Init(void)
{
	for (int nCntBg = 0; nCntBg < BG_PARTS_NUM; nCntBg++)
	{
		// ポリゴン生成
		m_apScene3d[nCntBg] = CScene3d::Create();
		if (m_apScene3d[nCntBg] != NULL)
		{
			// 中心座標の設定
			m_apScene3d[nCntBg]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 10));
			// サイズの設定
			m_apScene3d[nCntBg]->SetSize(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0));
			// オブジェクトタイプの設定
			m_apScene3d[nCntBg]->SetPriority(OBJTYPE_BG);

			// テクスチャ割り当て
			m_apScene3d[nCntBg]->BindTexture(m_apTexture[nCntBg]);
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CBg::Uninit(void)
{
	// 背景のパーツ数分開放処理
	for (int nCntBg = 0; nCntBg < BG_PARTS_NUM; nCntBg++)
	{
		if (m_apScene3d[nCntBg] != NULL)
		{
			m_apScene3d[nCntBg]->Uninit();
			m_apScene3d[nCntBg] = NULL;
		}
	}

	// 開放処理
	Release();
}

//==================================
// 更新処理
//==================================
void CBg::Update(void)
{
}

//==================================
// 描画処理
//==================================
void CBg::Draw(void)
{
}
