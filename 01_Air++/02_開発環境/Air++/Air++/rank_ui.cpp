////////////////////////////////////////////////////
//
//    FeverLogoの処理[FeverLogo.cpp]
//    Author:増澤 未来
//
////////////////////////////////////////////////////


//=============================
// インクルード
//=============================
#include "rank_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// マクロ定義
//**********************************
#define TEXTURE_PATH "./data/Textures/rank.png"    // テクスチャのパス
#define TEXTURE_PATH_RESULT "./data/Textures/rank_result.png"    // テクスチャのパス
#define MAX_ANIMATION_X 10                                    // アニメーション数 横
#define MAX_ANIMATION_Y 1                                     // アニメーション数 縦
#define SIZE D3DXVECTOR3( 70.0f, 70.0f, 0.0f)   // サイズ*画像の解像度に合わせる

// 位置
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 90 ,  SCREEN_HEIGHT    - 100 , 0.0f)  // 画面分割してないとき
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT/2) - 100 , 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 90 , (SCREEN_HEIGHT/2) - 80 , 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 90 ,  SCREEN_HEIGHT    - 100 , 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT/2) - 100 , 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 90 , (SCREEN_HEIGHT)   - 80 , 0.0f)  // プレイヤー3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT)   - 80 , 0.0f)  // プレイヤー4

// リザルト時のサイズ
#define RESULT_SIZE D3DXVECTOR3(200.0f, 200.0f, 0.0f)   // 画像の解像度に合わせる
// リザルト時の位置
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   - 60.0f, 0.0f)  // 画面分割してないとき
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // 画面を二つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // 画面を四つに分けているときのプレイヤー2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // プレイヤー3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // プレイヤー4

//**********************************
// 静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CRankUi::m_apTexture[RANK_UI_TEX_MAX] = {};

//=============================
// コンストラクタ
//=============================
CRankUi::CRankUi() :CScene2d(OBJTYPE_UI)
{
	// 変数のクリア
	m_nCntAnim = 0;           // アニメーションカウント
	m_nAnimX = 0;             // アニメーションX軸
	m_nAnimY = 0;             // アニメーションY軸
	m_nPlayerNum = 0;         // プレイヤー番号
	m_resultPos = VEC3_ZERO;  // リザルト時のポス
	m_resultSize = VEC3_ZERO; // リザルト時のサイズ
}

//=============================
// デストラクタ
//=============================
CRankUi::~CRankUi()
{
}

//=============================
// クリエイト
//=============================
CRankUi * CRankUi::Create(int nPlayerNum)
{
	// メモリの確保
	CRankUi *pRank = new CRankUi;
	// プレイヤー番号の代入
	pRank->m_nPlayerNum = nPlayerNum;
	// 初期化
	pRank->Init();
	return pRank;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CRankUi::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH_RESULT, &m_apTexture[1]);
	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CRankUi::Unload(void)
{
	for (int nCnt = 0; nCnt < RANK_UI_TEX_MAX; nCnt++)
	{
		// テクスチャの解放
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
	
}

//=============================
// 初期化処理
//=============================
HRESULT CRankUi::Init(void)
{
	CScene2d::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nAnimX = 0;     // アニメーションX軸
	m_nAnimY = 0;     // アニメーションY軸

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

	// サイズの設定
	SetSize(SIZE);

	m_resultSize = RESULT_SIZE;      // リザルト時のサイズ
									 // テクスチャの設定
	BindTexture(m_apTexture[RANK_UI_TEX_NORMAL]);

	// 座標の設定

	// プレイヤー人数の取得
	int nNumPlayer = CGame::GetPlayerNum();
	// プレイヤー番号によって分岐
	switch (m_nPlayerNum)
	{
	case 0:
		// プレイヤー1

		if (nNumPlayer == 1)
		{// 分割なし
			SetPos(POS_PLAYER1_1);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if (nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER1_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// 四分割
			SetPos(POS_PLAYER1_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// プレイヤー2

		if (nNumPlayer == 2)
		{// 二分割
			SetPos(POS_PLAYER2_2);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// 四分割
			SetPos(POS_PLAYER2_4);
			// リザルト時の座標
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// プレイヤー3
		SetPos(POS_PLAYER3);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// プレイヤー4
		SetPos(POS_PLAYER4);
		// リザルト時の座標
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}


	return S_OK;
}

//=============================
// 終了処理
//=============================
void CRankUi::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// 更新処理
//=============================
void CRankUi::Update(void)
{
	if (CGame::GetState() == CGame::GAME_NORMAL)
	{
		CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);

		while (pPlayer != NULL)
		{
			if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
			{
				m_nAnimX = pPlayer->GetRankData().nRank - 1;     // アニメーションX軸

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
			pPlayer = (CPlayer*)pPlayer->GetNext();
		}
	}
	else if(CGame::GetState() == CGame::GAME_RESULT)
	{// リザルト状態の時
		
		if (GetTexture() != m_apTexture[RANK_UI_TEX_RESULT])
		{// テクスチャの切り替え
			BindTexture(m_apTexture[RANK_UI_TEX_RESULT]);
		}

		SetPos(m_resultPos);
		SetSize(m_resultSize);
	}
}


//=============================
// 描画処理
//=============================
void CRankUi::Draw(void)
{
	CScene2d::Draw();
}