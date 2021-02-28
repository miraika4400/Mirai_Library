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
#define MAX_MATERIAL 64 // テクスチャ最大数

//=============================================================================
//モデルクラス
//=============================================================================
class CModel : public CScene
{
public:
	//============
	//列挙
	//============
	// 汎用モデルタイプ
	typedef enum
	{
		GENERAL_MODEL_SPHERE = 0, // 球
		GENERAL_MODEL_BOX,        // ボックス
		GENERAL_MODEL_CONE,       // 円錐
		GENERAL_MODEL_MAX         // タイプ数
	}GENERAL_MODEL_TYPE;

	//============
	//構造体
	//============
	// モデルデータ
	typedef struct
	{
		LPDIRECT3DTEXTURE9 apTexture[MAX_MATERIAL]; // テクスチャ
		LPD3DXMESH pMesh;                           // メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMat;                      // マテリアル情報へのポインタ
		DWORD nNumMat;                              // マテリアル情報の数
		D3DXVECTOR3 pos;                            // 位置
		D3DXVECTOR3 size;                           // 大きさ
		D3DXVECTOR3 rot;                            // 向き
		D3DXMATRIX mtxWorld;                        // ワールドマトリックス
		D3DXMATERIAL defMat[MAX_MATERIAL];          // マテリアル保管用
		D3DXCOLOR diffuseCol[MAX_MATERIAL];         // カラー*拡散
		int nParent;                                // 親のインデックス
	}Model;

	//============
	// メンバ関数
	//============
	CModel(int nPliority = OBJTYPE_NONE);
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos , GENERAL_MODEL_TYPE modelType  ,D3DXVECTOR3 size = D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // クラス生成*座標,汎用モデルタイプ,サイズ
	static HRESULT Load(void); // モデル読み込み
	static void Unload(void);  // モデルアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画
	LPD3DXMESH GetMesh(void)const; // メッシュの取得処理

	void BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel); // モデルの割り当て
	void BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture);                 // テクスチャの割り当て

	// ゲッター・セッター
	// 座標
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	// サイズ
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	// 回転
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void)const;
	// ワールドマトリックス
	void SetxWorldMatrix(D3DXMATRIX mtxWorld);
	Model *GetModelData(void);

private:
	//============
	// メンバ変数
	//============
	static Model m_aGeneralModel[GENERAL_MODEL_MAX]; // 汎用モデル
	Model m_model;                                   // モデルデータ
};
#endif 