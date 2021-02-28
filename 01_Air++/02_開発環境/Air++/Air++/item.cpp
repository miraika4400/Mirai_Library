////////////////////////////////////////////////////
//
//    Itemクラスの処理[Item.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////

//******************************
// インクルード
//******************************
#include "Item.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "billboard.h"
#include "character.h"
#include "game.h"
#include "cpu.h"
#include "itembox_effect.h"
#include "sound.h"

//*****************************
// マクロ定義
//*****************************
#define MODEL_PATH   "./data/Models/ItemBox.x"            // モデルのパス
#define TEXTURE_PATH "./data/Textures/QuestionMark.png"   // クエスチョンマークテクスチャのパス
#define ITEM_RADIUS 60                                    // 半径
#define ITEM_BOX_SIZE D3DXVECTOR3(0.4f,0.4f,0.4f)         // サイズ*ボックス(拡大率)
#define QUESTION_SIZE D3DXVECTOR3(30.0f,30.0f,0.0f)       // サイズ*?マーク
#define ITEM_ROTASION_VEC D3DXVECTOR3(0.02f,0.03f,0.008f) // アイテムの回転速度
#define ITEM_REPOP_COUNT 100                              // アイテムのリポップのカウント
#define ITEM_SIZE_RATE_UP   0.15f                         // アイテムサイズ"拡大"時の係数
#define ITEM_SIZE_RATE_DOWN 0.2f                          // アイテムサイズ"縮小"時の係数
#define ITEM_SIZE_DIST D3DXVECTOR3(1.0f,1.0f,1.0f)        // アイテムサイズ拡大時目標値
#define ITEM_SIZE_CHANGE D3DXVECTOR3(0.8f,0.8f,0.8f)      // サイズがこの値を超えたら縮小に移行する

//*****************************
// 静的メンバ変数宣言
//*****************************
CModel::Model   CItem::m_model = {};   	     //メッシュ情報へのポインタ
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL; // テクスチャ
D3DXVECTOR3 CItem::m_rot = VEC3_ZERO;        // アイテムの向き

//******************************
// コンストラクタ
//******************************
CItem::CItem() :CModel(OBJTYPE_ITEM)
{
	// 変数のクリア
	m_pCollision = NULL; // 当たり判定
	m_bActive = true;    // アイテムが有効か
	m_nCntRePop = 0;     // アイテムリポップカウント
	m_bExpansion = false;// サイズ拡大フラグ
}

//******************************
// デストラクタ
//******************************
CItem::~CItem()
{
}

//******************************
// クリエイト
//******************************
CItem * CItem::Create(const D3DXVECTOR3 pos)
{
	// メモリの確保
	CItem *pItem;
	pItem = new CItem;

	// 初期化
	pItem->Init();

	// 各値の代入・セット
	pItem->SetPriority(OBJTYPE_ITEM); // オブジェクトタイプ
	pItem->SetPos(pos);               // 座標のセット

	// 当たり判定の生成
	pItem->m_pCollision = CCollision::CreateSphere(pos, ITEM_RADIUS);

	// はてなマークの生成
	pItem->m_pBillboard = CBillboard::Create(pos, QUESTION_SIZE);
	// テクスチャの割り当て
	pItem->m_pBillboard->BindTexture(m_pTexture);

	return pItem;
}

//******************************
// テクスチャのロード
//******************************
HRESULT CItem::Load(void)
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

	// ?マークテクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);
	return S_OK;
}

//******************************
// テクスチャのアンロード
//******************************
void CItem::Unload(void)
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

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// アイテムの回転処理
//******************************
void CItem::RotationItem(void)
{
	m_rot += ITEM_ROTASION_VEC;
}

//******************************
// 初期化処理
//******************************
HRESULT CItem::Init(void)
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
	SetSize(ITEM_BOX_SIZE);

	// 変数の初期化
	m_bActive = true;    // アイテムが有効か
	m_nCntRePop = 0;     // アイテムリポップカウント

	return S_OK;
}

//******************************
// 終了処理
//******************************
void CItem::Uninit(void)
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
void CItem::Update(void)
{
	if (m_bActive)
	{// 有効状態の時
		// キャラクターとの当たり判定
		CollisionCharacter();
		// 回転のセット
		SetRot(m_rot);
	}
	else
	{
		m_nCntRePop++;
		if (m_nCntRePop > ITEM_REPOP_COUNT)
		{
			// カウントの初期化
			m_nCntRePop = 0;

			// 有効にする
			m_bActive = true;

			// はてなマークの生成
			m_pBillboard = CBillboard::Create(GetPos(), QUESTION_SIZE);
			// テクスチャの割り当て
			m_pBillboard->BindTexture(m_pTexture);

			SetSize(ITEM_BOX_SIZE);
		}
		else
		{
			// サイズの取得
			D3DXVECTOR3 size = GetSize();
			
			if (m_bExpansion)
			{// サイズ拡大範囲
				size += (ITEM_SIZE_DIST - size)*ITEM_SIZE_RATE_UP;
				
				if (size.x >= ITEM_SIZE_CHANGE.x&&size.y >= ITEM_SIZE_CHANGE.y&&size.z >= ITEM_SIZE_CHANGE.z)
				{
					m_bExpansion = false;
				}
			}
			else
			{// サイズ縮小範囲
				size += (VEC3_ZERO - size)*ITEM_SIZE_RATE_DOWN;
			}
			SetSize(size);

			// アイテム取得時は回転を速くする
			SetRot(GetRot() + ITEM_ROTASION_VEC * 5);
		}
	}
	
}

//******************************
// 描画処理
//******************************
void CItem::Draw(void)
{
	CModel::Draw();
}

//******************************
// プレイヤーとの当たり判定
//******************************
void CItem::CollisionCharacter(void)
{
	// キャラクター数分ループ
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// キャラクター情報の取得
		CCharacter*pChara = CGame::GetCharacter(nCnt);

		if (CCollision::CollisionSphere(m_pCollision, pChara->GetCollision()))
		{// 当たったとき
			// アイテムを無効にする
			m_bActive = false;
			// サイズ拡大フラグを立てる
			m_bExpansion = true;
			// はてなマークを消す
			m_pBillboard->Uninit();

			// 破片エフェクトの生成
			CItemBoxEffect::SetEffect(GetPos(), -pChara->GetRot().y + D3DXToRadian(90));

			if (pChara->GetItem() == ITEM_NONE)
			{// キャラがアイテムを持っていないとき
				// ランダムでアイテムの取得
				pChara->SetItem((ITEM_TYPE)(rand() % ITEM_MAX));

				if (!pChara->GetIsPlayer())
				{// NPCだった時
					//	アイテムカウントのセット
					((CCpu*)pChara)->SetItemCount((rand() % CPU_ITEM_USE_RAND_AMPLITUDE) + CPU_ITEM_USE_RAND_MIN);
				}
			}

			if (pChara->GetIsPlayer())
			{// プレイヤーだった時
				// SE再生
				CManager::GetSound()->Play(CSound::LABEL_SE_ITEM);
			}
			break;
		}
	}
	
}
