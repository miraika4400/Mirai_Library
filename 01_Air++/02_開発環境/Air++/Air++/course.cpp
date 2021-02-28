//=============================================================================
//
// courseヘッダ [course.cpp]
// Author : 増澤 未来
//
//=============================================================================

//******************************
// インクルード
//******************************
#include "course.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "character.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH     "./data/Models/course002.x"         // モデルのパス
#define CUBE_TEX_PATH  "./data/Textures/cube_sky.dds" // キューブテクスチャのパス
#define SHADER_PATH    "./data/HLSL/Shader_course.fx"             // HLSLファイルのパス
#define ADJUST_HEIGHT 30  // キャラクターよりちょっと上からレイを出す

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CCourse::m_model = {};  //メッシュ情報へのポインタ
CScene::Shader CCourse::m_shader = {};  // シェーダー構造体

//******************************
// コンストラクタ
//******************************
CCourse::CCourse() :CModelShader(OBJTYPE_MAP)
{
	// 変数のクリア
}

//******************************
// デストラクタ
//******************************
CCourse::~CCourse()
{
}

//******************************
// クリエイト
//******************************
CCourse * CCourse::Create(void)
{
	// メモリの確保
	CCourse *pCourse;
	pCourse = new CCourse;

	// 初期化
	pCourse->Init();

	return pCourse;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CCourse::Load(void)
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

	// HLSLファイルの読み込み
	D3DXCreateEffectFromFile(pDevice, SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	D3DXCreateCubeTextureFromFile(pDevice, CUBE_TEX_PATH, &m_shader.pCubeTex);
	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CCourse::Unload(void)
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

//******************************
// 初期化処理
//******************************
HRESULT CCourse::Init(void)
{
	if (FAILED(CModelShader::Init()))
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

	// シェーダーのセット
	SetShader(m_shader);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CCourse::Uninit(void)
{
	CModelShader::Uninit();
}

//******************************
// 更新処理
//******************************
void CCourse::Update(void)
{
	// キャラクターとの当たり判定処理
	CollisionCharacter();

	CModelShader::Update();
}

//******************************
// 描画処理
//******************************
void CCourse::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// キャラクターとの当たり判定*
//******************************
void CCourse::CollisionCharacter(void)
{
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター数分ループ
		CCharacter *pChara = CGame::GetCharacter(nCnt);

		if (pChara != NULL)
		{
			// キャラクター座標の取得
			D3DXVECTOR3 charaPos = pChara->GetPos();
			// キャラクターよりちょっと高い位置から出す
			charaPos.y += ADJUST_HEIGHT;

			BOOL bHit = FALSE;     // レイが当たっているか
			float fDistance = 0.0f;// レイが当たっている距離

			 // レイ
			D3DXIntersect(m_model.pMesh,
				&charaPos,
				&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
				&bHit,
				NULL,
				NULL,
				NULL,
				&fDistance,
				NULL,
				NULL);

			if (bHit)
			{// レイが当たっていたら

				if (fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT - 1)
				{// 床とキャラクターの距離を一定以上に保つ

				 // キャラクターの座標の更新
					charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
					pChara->SetPos(charaPos);

					if (pChara->GetActiveGravity())
					{// 重力フラグが立っているとき
					 // 重力を切る
						pChara->SetActiveGravity(false);
					}
				}
				else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT + 50)
				{// ある程度の範囲だったらコースに吸い付ける

				 // キャラクターの座標の更新
					charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
					pChara->SetPos(charaPos);

					if (pChara->GetActiveGravity())
					{// 重力フラグが立っているとき
					 // 重力を切る
						pChara->SetActiveGravity(false);
					}
				}
				else
				{// 床から離れすぎてい時
					if (!pChara->GetActiveGravity())
					{// 重力フラグが立ってないとき
					 // 重力フラグを建てる
						pChara->SetActiveGravity(true);
					}
				}
			}
			else
			{// 自分の下に床がない
				if (!pChara->GetActiveGravity())
				{// 重力フラグが立ってないとき
				 // 重力フラグを建てる
					pChara->SetActiveGravity(true);
				}
			}
		}
	}

}
//******************************
// キャラクターとの当たり判定*
//******************************
void CCourse::CollisionCharacter(CCharacter * pChara)
{
	if (pChara != NULL)
	{
		// キャラクター座標の取得
		D3DXVECTOR3 charaPos = pChara->GetPos();
		// キャラクターよりちょっと高い位置から出す
		charaPos.y += ADJUST_HEIGHT;

		BOOL bHit = FALSE;     // レイが当たっているか
		float fDistance = 0.0f;// レイが当たっている距離

							   // レイ
		D3DXIntersect(m_model.pMesh,
			&charaPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{// レイが当たっていたら

			if (fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT - 1)
			{// 床とキャラクターの距離を一定以上に保つ

			 // キャラクターの座標の更新
				charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				pChara->SetPos(charaPos);

				if (pChara->GetActiveGravity())
				{// 重力フラグが立っているとき
				 // 重力を切る
					pChara->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT + 50)
			{// ある程度の範囲だったらコースに吸い付ける

			 // キャラクターの座標の更新
				charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				pChara->SetPos(charaPos);

				if (pChara->GetActiveGravity())
				{// 重力フラグが立っているとき
				 // 重力を切る
					pChara->SetActiveGravity(false);
				}
			}
			else
			{// 床から離れすぎてい時
				if (!pChara->GetActiveGravity())
				{// 重力フラグが立ってないとき
				 // 重力フラグを建てる
					pChara->SetActiveGravity(true);
				}
			}
		}
		else
		{// 自分の下に床がない
			if (!pChara->GetActiveGravity())
			{// 重力フラグが立ってないとき
			 // 重力フラグを建てる
				pChara->SetActiveGravity(true);
			}
		}
	}
}
