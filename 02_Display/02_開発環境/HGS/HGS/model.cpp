//=============================================================================
//
// モデル処理 [model.cpp]
// Author :	増澤 未来
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//モデルクラスのコンストラクタ
//=============================================================================
CModel::CModel(int nPriority) :CScene(nPriority)
{
	// 変数のクリア
	m_model.pBuffMat = NULL;
	m_model.pMesh = NULL;
	m_model.nNumMat = 0;
	m_model.pos = D3DXVECTOR3();
	m_model.size = D3DXVECTOR3();
	m_model.rot = D3DXVECTOR3();
	m_model.nParent = -1;
	memset(&m_model.apTexture, 0, sizeof(m_model.apTexture));
	memset(&m_defMat, 0, sizeof(m_defMat));
}

//=============================================================================
//モデルクラスのデストラクタ
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
//モデルクラスのクリエイト処理
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//モデルクラスのポインタ変数
	CModel *pModel = NULL;

	//メモリ確保
	pModel = new CModel;

	//メモリが確保できていたら
	if (pModel != NULL)
	{
		//初期化処理呼び出し
		pModel->Init();
		pModel->m_model.pos = pos;
		pModel->m_model.size = size;
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pModel;
}

//=============================================================================
//モデルクラスの初期化処理
//=============================================================================
HRESULT CModel::Init()
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	m_model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向きの初期化
	m_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//大きさの初期化
	m_model.size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
//モデルクラスの終了処理
//=============================================================================
void CModel::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//モデルクラスの更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
//モデルクラスの描画処理
//=============================================================================
void CModel::Draw(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScail;
	D3DMATERIAL9 matDef;	//現在のマテリアル保持用
	D3DXMATERIAL*pMat;	//マテリアルデータへのポインタ

						//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_model.mtxWorld);

	//サイズを反映
	D3DXMatrixScaling(&mtxScail, m_model.size.x, m_model.size.y, m_model.size.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxScail);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y, m_model.rot.x, m_model.rot.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_model.mtxWorld);

	//現在のマテリアルを取得する
	pDevice->GetMaterial(&matDef);

	if (m_model.pBuffMat != NULL)
	{
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
		{
			//マテリアルのアンビエントにディフューズカラーを設定
			pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			// テクスチャ
			pDevice->SetTexture(0, m_model.apTexture[nCntMat]);
			//モデルパーツの描画
			m_model.pMesh->DrawSubset(nCntMat);

			pMat[nCntMat] = m_defMat[nCntMat];
		}
	}
	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//モデルクラスのメッシュ情報の取得
//=============================================================================
LPD3DXMESH CModel::GetMesh(void) const
{
	return m_model.pMesh;
}

//=============================================================================
//モデルクラスの位置情報の設定
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_model.pos = pos;
}

//=============================================================================
//モデルクラスの位置情報の取得
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) const
{
	return m_model.pos;
}

//=============================================================================
//モデルクラスの大きさ情報の設定
//=============================================================================
void CModel::SetSize(const D3DXVECTOR3 size)
{
	m_model.size = size;
}

//=============================================================================
//モデルクラスの大きさ情報の取得
//=============================================================================
D3DXVECTOR3 CModel::GetSize(void) const
{
	return m_model.size;
}

//=============================================================================
//モデルクラスの向きの設定
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_model.rot = rot;
}

//=============================================================================
//モデルクラスの向きの取得
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) const
{
	return m_model.rot;
}


//=============================================================================
// モデルの割り当て
//=============================================================================
void CModel::BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel)
{
	m_model.pMesh = pMeshModel;
	m_model.pBuffMat = pBuffMatModel;
	m_model.nNumMat = nNumMatModel;

	for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
	{
		m_defMat[nCntMat] = ((D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer())[nCntMat];

	}
}

//=============================================================================
// モデルのテクスチャ割り当て
//=============================================================================
void CModel::BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_model.apTexture[nIndex] = pTexture;
}

//=============================================================================
// ワールドマトリックスの設定
//=============================================================================
void CModel::SetxWorldMatrix(D3DXMATRIX mtxWorld)
{
	m_model.mtxWorld = mtxWorld;
}

//=============================================================================
// モデル情報の取得
//=============================================================================
CModel::Model * CModel::GetModelData(void)
{
	return &m_model;
}
