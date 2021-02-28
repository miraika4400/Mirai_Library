//=============================================================================
//
// モデル処理 [model.h]
// Author : 増澤 未来
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_TEXTURE 64

//=============================================================================
//モデルクラス
//=============================================================================
class CModel : public CScene
{
public:
	//============
	//構造体
	//============
	// モデルデータ
	typedef struct
	{
		LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE]; // テクスチャ
		LPD3DXMESH pMesh;                          //メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMat;                     //マテリアル情報へのポインタ
		DWORD nNumMat;                             //マテリアル情報の数
		D3DXVECTOR3 pos;                           //位置
		D3DXVECTOR3 size;                          //大きさ
		D3DXVECTOR3 rot;                           //向き
		D3DXMATRIX mtxWorld;                       //ワールドマトリックス
		int nParent;                               // 親のインデックス
	}Model;

	//============
	// メンバ関数
	//============
	CModel(int nPliority = OBJTYPE_NONE);
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 size = D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPD3DXMESH GetMesh(void)const;

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void)const;
	void BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel);
	void BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture);
	void SetxWorldMatrix(D3DXMATRIX mtxWorld);
	Model *GetModelData(void);

private:
	//============
	// メンバ変数
	//============
	Model m_model;              // モデルデータ
	D3DXMATERIAL m_defMat[128]; // マテリアル保管用
};
#endif 