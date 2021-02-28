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
#define MODEL_PATH "./data/Models/bg.x" // モデルのパス
#define SIZE       D3DXVECTOR3(110.0f,110.0f ,110.0f) // サイズ
#define ADD_ROT D3DXVECTOR3(0.0f,0.0005f,0.0f)         // ゆっくり回す
//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model CBg::m_model = {};  //メッシュ情報へのポインタ


//==================================
// コンストラクタ
//==================================
CBg::CBg() : CModel(OBJTYPE_BG)
{
	
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

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// テクスチャ読み込み
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	return S_OK;
}

//==================================
// アンロード
//==================================
void CBg::Unload(void)
{
	//メッシュの破棄
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CBg::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// モデル割り当て
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//テクスチャの割り当て
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// サイズの設定
	SetSize(SIZE);
	
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CBg::Uninit(void)
{
	CModel::Uninit();
}

//==================================
// 更新処理
//==================================
void CBg::Update(void)
{
	// ゆっくり回す
	SetRot(GetRot() + ADD_ROT);
}

//==================================
// 描画処理
//==================================
void CBg::Draw(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// ライトを消す
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	CModel::Draw();

	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
