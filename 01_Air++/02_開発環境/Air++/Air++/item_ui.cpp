////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "item_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "polygon.h"

//**********************************
// マクロ定義
//**********************************
#define TEXTURE_PATH_FRAME "./data/Textures/Item_ui_Frame.png"            // テクスチャのパス
#define FRAME_SIZE   D3DXVECTOR3( 80.0f, 80.0f, 0.0f)               // アイテムフレーム

// 位置
#define POS_PLAYER1_1 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // 画面分割してないとき
#define POS_PLAYER1_2 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define POS_PLAYER1_4 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define POS_PLAYER2_2 D3DXVECTOR3( 0                   + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define POS_PLAYER2_4 D3DXVECTOR3((SCREEN_WIDTH/2)     + 150.0f, 0                 + 140.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define POS_PLAYER3   D3DXVECTOR3( 0                   + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // プレイヤー3
#define POS_PLAYER4   D3DXVECTOR3((SCREEN_WIDTH/2)     + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // プレイヤー4

// リザルト時のサイズ
#define RESULT_NUMBER_SIZE D3DXVECTOR3( 15.0f*2.5f, 30.0f*2.5f, 0.0f)     // 数字サイズ*画像の解像度に合わせる

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CItemUi::m_pFrameTex = {};
LPDIRECT3DTEXTURE9 CItemUi::m_apTexture[CItem::ITEM_MAX] = {};
 char *CItemUi:: m_apItemPath[CItem::ITEM_MAX] = 
 {
	 { "./data/Textures/item_ui_speed.png" },
	 { "./data/Textures/item_ui_attack.png" },
	 { "./data/Textures/item_ui_trap.png" },
 };

//=============================
// コンストラクタ
//=============================
CItemUi::CItemUi() :CScene(OBJTYPE_UI)
{
	// 変数のクリア
	m_nPlayerNum = 0;                         // プレイヤー番号
	m_pos = VEC3_ZERO;                        // 中心座標
	m_pFramePolygon = NULL;                   // フレーム
	m_pItemPolygon = NULL;                    // アイテム本体
}

//=============================
// デストラクタ
//=============================
CItemUi::~CItemUi()
{
}

//=============================
// クリエイト
//=============================
CItemUi * CItemUi::Create(int nPlayerNum)
{
	// メモリの確保
	CItemUi *pItemUi = new CItemUi;
	// プレイヤー番号の代入
	pItemUi->m_nPlayerNum = nPlayerNum;
	// 初期化
	pItemUi->Init();
	return pItemUi;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CItemUi::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH_FRAME, &m_pFrameTex);

	for (int nCnt = 0; nCnt < CItem::ITEM_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apItemPath[nCnt], &m_apTexture[nCnt]);
	}
	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CItemUi::Unload(void)
{
	// テクスチャの解放
	if (m_pFrameTex != NULL)
	{
		m_pFrameTex->Release();
		m_pFrameTex = NULL;
	}
}

//=============================
// 初期化処理
//=============================
HRESULT CItemUi::Init(void)
{
	// 座標の設定

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();
	// プレイヤー人数と番号によって分岐
	switch (m_nPlayerNum)
	{
	case 0:
		// プレイヤー1

		if (nNumPlayer == 1)
		{// 分割なし
			m_pos = (POS_PLAYER1_1);
			
		}
		else if (nNumPlayer == 2)
		{// 二分割
			m_pos = (POS_PLAYER1_2);
			
		}
		else
		{// 四分割
			m_pos = (POS_PLAYER1_4);
			
		}
		break;
	case 1:
		// プレイヤー2

		if (nNumPlayer == 2)
		{// 二分割
			m_pos = (POS_PLAYER2_2);

		}
		else
		{// 四分割
			m_pos = (POS_PLAYER2_4);
		}
		break;
	case 2:
		// プレイヤー3
		m_pos = (POS_PLAYER3);

		break;
	case 3:
		// プレイヤー4
		m_pos = (POS_PLAYER4);

		break;
	default:
		break;
	}

	// ポリゴン生成
	m_pFramePolygon = CPolygon::Create(m_pos, FRAME_SIZE);
	m_pFramePolygon->BindTexture(m_pFrameTex);

	// ポリゴン生成
	m_pItemPolygon = CPolygon::Create(m_pos, FRAME_SIZE);
	m_pItemPolygon->BindTexture(NULL);

	return S_OK;
}

//=============================
// 終了処理
//=============================
void CItemUi::Uninit(void)
{
	if (m_pFramePolygon != NULL)
	{
		m_pFramePolygon->Uninit();
		delete m_pFramePolygon;
		m_pFramePolygon = NULL;
	}
	
	Release();
}


//=============================
// 更新処理
//=============================
void CItemUi::Update(void)
{
	// プレイヤー情報の取得
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);
	while (pPlayer != NULL)
	{
		if (pPlayer->GetPlayerNum() == m_nPlayerNum)
		{
			if (pPlayer->GetItem() != CItem::ITEM_NONE)
			{
				// テクスチャの割り当て
				m_pItemPolygon->BindTexture(m_apTexture[pPlayer->GetItem()]);
			}
			else
			{
				// テクスチャをNULLにする
				m_pItemPolygon->BindTexture(NULL);
			}
			break;
		}
	}

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// リザルト時消す
		Uninit();
	}
	
}


//=============================
// 描画処理
//=============================
void CItemUi::Draw(void)
{
	if (m_pFramePolygon != NULL)
	{
		// ポリゴン描画
		m_pFramePolygon->Draw();
	}

	if (m_pItemPolygon != NULL)
	{
		if (m_pItemPolygon->GetTexture() != NULL)
		{
			m_pItemPolygon->Draw();
		}
	}
	
}