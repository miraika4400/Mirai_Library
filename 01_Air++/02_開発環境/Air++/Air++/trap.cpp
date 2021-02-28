////////////////////////////////////////////////////
//
//    Trapクラスの処理[Trap.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "trap.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "billboard.h"
#include "character.h"
#include "game.h"
#include "player.h"
#include "camera.h"
#include "attack.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH   "./data/Models/trap.x"               // モデルのパス
#define TEXTURE_PATH "./data/Textures/QuestionMark.png"   // クエスチョンマークテクスチャのパス
#define TRAP_RADIUS 25                                    // 半径
#define TRAP_SIZE D3DXVECTOR3(0.35f,0.35f,0.35f)          // サイズ*ボックス(拡大率)

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CTrap::m_model = {};   	     //メッシュ情報へのポインタ

//******************************
// コンストラクタ
//******************************
CTrap::CTrap() :CModel(OBJTYPE_ITEM)
{
	// 変数のクリア
	m_pCollision = NULL; // 当たり判定
}

//******************************
// デストラクタ
//******************************
CTrap::~CTrap()
{
}

//******************************
// クリエイト
//******************************
CTrap * CTrap::Create(const D3DXVECTOR3 pos)
{
	// メモリの確保
	CTrap *pTrap;
	pTrap = new CTrap;

	// 初期化
	pTrap->Init();

	// 各値の代入・セット
	pTrap->SetPriority(OBJTYPE_ITEM); // オブジェクトタイプ
	pTrap->SetPos(pos);               // 座標のセット

	// 当たり判定の生成
	pTrap->m_pCollision = CCollision::CreateSphere(pos, TRAP_RADIUS);

	return pTrap;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CTrap::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// コース数分ループ

	//Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.nNumMat,
		&m_model.pMesh);

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

//******************************
// テクスチャのアンロード
//******************************
void CTrap::Unload(void)
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
HRESULT CTrap::Init(void)
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
	SetSize(TRAP_SIZE);

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CTrap::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}

	CModel::Uninit();
}

//******************************
// 更新処理
//******************************
void CTrap::Update(void)
{
	// キャラクターとの当たり判定
	if(CollisionCharacter()) return;
	// 攻撃との当たり判定
	if (CollisionAttack()) return;
}

//******************************
// 描画処理
//******************************
void CTrap::Draw(void)
{
	CModel::Draw();
}

//******************************
// プレイヤーとの当たり判定
//******************************
bool CTrap::CollisionCharacter(void)
{
	// キャラクター数分ループ
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター情報の取得
		CCharacter*pChara = CGame::GetCharacter(nCnt);

		if (CCollision::CollisionSphere(m_pCollision, pChara->GetCollision()))
		{// 当たったとき

			if (!pChara->GetStan())
			{// キャラクターをスタン状態にする
				pChara->SetStan(true);
				if (pChara->GetIsPlayer())
				{// プレイヤーだった時
					// カメラのブレの設定
					CGame::GetCamera(((CPlayer*)pChara)->GetPlayerNum())->Shake(true);
					// SE再生
					CManager::GetSound()->Play(CSound::LABEL_SE_HIT);
				}
			}
			// 消す
			Uninit();

			return true;
		}
	}
	return false;
}

//*********************************
// 攻撃オブジェクトとの当たり判定
//*********************************
bool CTrap::CollisionAttack(void)
{
	CAttack*pAttack = (CAttack*)GetTop(OBJTYPE_ATTACK);

	while (pAttack != NULL)
	{
		if (m_pCollision != NULL&&pAttack->GetCollision() != NULL)
		{
			if (CCollision::CollisionSphere(m_pCollision, pAttack->GetCollision()))
			{// 当たっていた時

			 // 攻撃を消す
				pAttack->Uninit();
				// トラップを消す
				Uninit();

				// 処理の終了
				return true;
			}
		}
		
		pAttack = (CAttack*)pAttack->GetNext();
	}

	return false;
}
